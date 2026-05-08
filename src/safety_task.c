#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "print_helper.h"

/* ── Borrow result_queue from main.c ────────────────
   Safety only needs result_queue
   Receives ALERT or NORMAL from processor             */
extern QueueHandle_t result_queue;

#define ALERT 1U

/* ── Safety task ─────────────────────────────────────
   Most critical task — priority 4 (highest)
   Responds to ALERT immediately
   Preempts ALL other tasks the moment ALERT arrives
   In real car: cut fuel, apply brakes, fire airbag    */
void safety_task(void *params) {
    (void)params;
    uint32_t status;

    print("Safety task started\n");

    while (1) {
        /* Block forever — uses zero CPU while waiting
           Wakes IMMEDIATELY when processor sends data
           Priority 4 means nothing can delay this     */
        if (xQueueReceive(result_queue,
                          &status,
                          portMAX_DELAY) == pdPASS) {

            if (status == ALERT) {
                print("Safety: *** SAFE STATE TRIGGERED ***\n");
            }
        }
    }
}