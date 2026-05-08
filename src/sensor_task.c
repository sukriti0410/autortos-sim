#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "print_helper.h"

/* ── Borrow sensor_queue from main.c ─────────────────
   extern = defined somewhere else
   Linker connects this to main.c's sensor_queue       */
extern QueueHandle_t sensor_queue;

/* ── Sensor task ─────────────────────────────────────
   Simulates a hardware sensor — reads value every 200ms
   Sends reading to sensor_queue for processor to use
   Priority 2 = lowest — runs when others are blocked  */
void sensor_task(void *params) {
    (void)params;
    uint32_t reading = 0;

    print("Sensor task started\n");

    while (1) {
        /* Simulate reading: 10,20,30...100,10,20...
           Crosses threshold 80 regularly for demo      */
        reading = (reading + 10) % 110;

        /* Put reading in queue
           0 = don't wait if queue full                 */
        if (xQueueSend(sensor_queue, &reading, 0) == pdPASS) {
            print("Sensor: sent reading\n");
        }

        /* Sleep 200ms — moves to BLOCKED
           CPU free for Processor and Safety tasks      */
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}