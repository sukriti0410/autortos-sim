#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "print_helper.h"

/* ── Borrow both queues from main.c ─────────────────
   Reads from sensor_queue
   Writes to result_queue                              */
extern QueueHandle_t sensor_queue;
extern QueueHandle_t result_queue;

#define ALERT  1U  /* reading dangerous                */
#define NORMAL 0U  /* reading safe                     */

/* ── Processor task ──────────────────────────────────
   ECU brain — analyses sensor readings
   Decides ALERT or NORMAL
   Priority 3 = medium                                 */
void processor_task(void *params) {
    (void)params;
    uint32_t reading;
    uint32_t status;

    print("Processor task started\n");

    while (1) {
        /* Block until sensor sends data
           Task uses zero CPU while waiting             */
        if (xQueueReceive(sensor_queue,
                          &reading,
                          portMAX_DELAY) == pdPASS) {

            /* Threshold check                         */
            if (reading > 80) {
                status = ALERT;
                print("Processor: ALERT\n");
            } else {
                status = NORMAL;
                print("Processor: NORMAL\n");
            }

            /* Send decision to safety task            */
            xQueueSend(result_queue, &status, 0);
        }
    }
}