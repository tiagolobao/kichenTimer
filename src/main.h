/** ------------------------------------------------------------
 * @file main.h
 * 
 * @author Tiago Lobao
 * 
 * @brief The main file header contains the main functions for tasks
 * 
 ------------------------------------------------------------ */

/* Application tasks */
void task_debuglog(void *pvParameters);
void task_ultrasonicmodule(void *pvParameters);
void task_display(void *pvParameters);
void task_radio(void *pvParameters);

