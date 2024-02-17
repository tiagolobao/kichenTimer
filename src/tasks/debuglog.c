
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "uart.h"

void task_debuglog(void *pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();

	DDRB |= _BV(DDB5);
    uart_init();

    for(;;)
    {
        const char msg[] = "Blinked!";
        int i;

    	PORTB |=  _BV(PORTB5);       // main (red PB5) LED on. Arduino LED on
		vTaskDelayUntil( &xLastWakeTime, ( 500 / portTICK_PERIOD_MS ) );

		PORTB &= ~_BV(PORTB5);       // main (red PB5) LED off. Arduino LED off
		vTaskDelayUntil( &xLastWakeTime, ( 500 / portTICK_PERIOD_MS )  );
        
        for ( i=0 ; i<sizeof(msg) ; i++ )
        {
            while ( !uart_send_byte(msg[i]) )
            {
                // try again
            }
        }
    }
}