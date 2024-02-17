/**
 * @file uart.c
 * @brief functions to use UART/Serial comminication for atMega328p

 * @author Devilbinder (Binder Tronics)
 * Modified by Tiago Lobao
 * 
 * @license 
 * Copyright (c) 2021 Devilbinder (Binder Tronics) https://bindertronics.com/
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/ 


#include "FreeRTOS.h"
#include "uart.h"

#include "task.h"
#include <stdint.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Local Macros
#define incrementRingBufferIndex(i,max) ++i; if(i>=max) i=0
#define bufferNotFull(cnt,max) (cnt < max)

// buffers definitions
volatile static uint8_t rx_buffer[RX_BUFFER_SIZE] = {0};
volatile static uint16_t rx_count = 0;

volatile static uint8_t tx_buffer[TX_BUFFER_SIZE] = {0};
volatile static uint16_t tx_count = 0;

// ----------------------------------------------------------
ISR(USART_RX_vect)
{
    volatile static uint16_t rx_write_pos = 0;

    if( bufferNotFull(rx_count,RX_BUFFER_SIZE) ){
        rx_buffer[rx_write_pos] = UDR0;
        rx_count++;
        incrementRingBufferIndex(rx_write_pos,RX_BUFFER_SIZE);
    }
}

// ----------------------------------------------------------
//volatile static uint8_t busy = 0;
ISR(USART_TX_vect)
{
    volatile static uint16_t tx_read_pos = 0;

    switch(tx_count){
    default:
        UDR0 = tx_buffer[tx_read_pos];
        incrementRingBufferIndex(tx_read_pos,TX_BUFFER_SIZE);
    case 1:
        tx_count--;
    case 0:
        break;
        // do nothing
    }

}

// ----------------------------------------------------------
void uart_init(void)
{
    uint32_t baud = UART_BAUD_RATE_UART;
    uint8_t high_speed = UART_HIGH_SPEED_UART;

    uint8_t speed = 16;
    
    if(high_speed != 0){
        speed = 8;
        UCSR0A |= 1 << U2X0;
    }
    
    baud = (F_CPU/(speed*baud)) - 1;
    
    UBRR0H = (baud & 0x0F00) >> 8;
    UBRR0L = (baud & 0x00FF);
    
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << TXCIE0) | (1 << RXCIE0);
    
}

// ----------------------------------------------------------
void uart_deinit(void)
{
    UCSR0B |= (0 << TXEN0) | (0 << RXEN0) | (0 << TXCIE0) | (0 << RXCIE0);
}

// ----------------------------------------------------------
uint16_t uart_free_spaces(void)
{
    return TX_BUFFER_SIZE - tx_count;
}

// ----------------------------------------------------------
uint8_t uart_send_byte(uint8_t c)
{
    volatile static uint16_t tx_write_pos = 0;
    bool result = true;

    if(tx_count == 0){
        UDR0 = c;
        tx_count++;
    }
    else if( bufferNotFull(tx_count,TX_BUFFER_SIZE) ){
        taskENTER_CRITICAL();
        tx_buffer[tx_write_pos] = c;
        tx_count++;
        incrementRingBufferIndex(tx_write_pos,TX_BUFFER_SIZE);
        taskEXIT_CRITICAL();
    }
    else{
        result = false;
    }
    
    return result;
}

// ----------------------------------------------------------
uint16_t uart_read_count(void){
    return rx_count;
}

// ----------------------------------------------------------
uint8_t uart_read(void){
    static uint16_t rx_read_pos = 0;
    uint8_t data = 0;
    
    if( rx_count == 0 )
        return 0xFF; // ERR, compatibility with curses

    data = rx_buffer[rx_read_pos];
    rx_count--;
    incrementRingBufferIndex(rx_read_pos,RX_BUFFER_SIZE);

    return data;
}

// ----------------------------------------------------------
void uart_flush(void)
{
    while( rx_count > 0){
        (void)uart_read();
    }
}