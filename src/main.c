#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "print_helper.h"

/* ── Forward declarations ────────────────────────────
   Functions defined in other .c files
   Compiler trusts us — linker connects them later     */
void sensor_task(void *params);
void processor_task(void *params);
void safety_task(void *params);

/* ── Queue handles ───────────────────────────────────
   Defined HERE — memory allocated here in main.c
   Other files use extern to borrow these
   All tasks share the SAME queue objects              */
QueueHandle_t sensor_queue;
QueueHandle_t result_queue;

int main(void) {
    itm_init();  /* initialise ITM trace — must be first
                    enables character output from tasks  */

    print("AutoRTOS-Sim starting...\n");

    /* ── Create queues ───────────────────────────────
       Must happen BEFORE tasks start
       10 = max items, sizeof(uint32_t) = 4 bytes each */
    sensor_queue = xQueueCreate(10, sizeof(uint32_t));
    result_queue = xQueueCreate(10, sizeof(uint32_t));

    if (sensor_queue == NULL || result_queue == NULL) {
        print("ERROR: queue creation failed\n");
        while (1) {}
    }

    /* ── Create 3 tasks ──────────────────────────────
       All go to READY state immediately
       Nobody runs until vTaskStartScheduler()
       Priority: Safety(4) > Processor(3) > Sensor(2)  */
    xTaskCreate(sensor_task,    "Sensor",    256, NULL, 2, NULL);
    xTaskCreate(processor_task, "Processor", 256, NULL, 3, NULL);
    xTaskCreate(safety_task,    "Safety",    256, NULL, 4, NULL);

    /* ── Start FreeRTOS ──────────────────────────────
       Hands CPU to FreeRTOS permanently
       Configures SysTick every 1ms
       This line NEVER returns                         */
    print("Starting FreeRTOS scheduler...\n");
    vTaskStartScheduler();

    /* Only reached if heap too small                  */
    print("ERROR: scheduler returned\n");
    while (1) {}
    return 0;
}