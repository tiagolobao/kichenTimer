/** ------------------------------------------------------------
 * @file main.c
 * 
 * @author Tiago Lobao
 * 
 * @brief The main file contains the initialization of the taks
 * And pre-configuration of MCU/FreeRTOS
 * 
 ------------------------------------------------------------ */

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

int main(void)
{
    xTaskCreate(
        task_debuglog,
        (const char *)"debuglog",
        256,
        NULL,
        3,
        NULL
    );

    xTaskCreate(
        task_ultrasonicmodule,
        (const char *)"ultrasonicmodule",
        256,
        NULL,
        3,
        NULL
    );

    xTaskCreate(
        task_display,
        (const char *)"display",
        256,
        NULL,
        3,
        NULL
    );

    xTaskCreate(
        task_radio,
        (const char *)"radio",
        256,
        NULL,
        3,
        NULL
    );

    vTaskStartScheduler();

    for(;;)
    {
        // Idle task
    }
}


void task_ultrasonicmodule(void *pvParameters)
{
    for(;;){}
}
void task_display(void *pvParameters)
{
    for(;;){}
}
void task_radio(void *pvParameters)
{
    for(;;){}
}