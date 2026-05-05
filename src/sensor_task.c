#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* ── What this file does: ────────────────────────────
   Sensor task — generates a fake sensor reading
   every 200ms and sends it to a queue.

   In a real ECU this would read an actual sensor
   like temperature, pressure, wheel speed etc.
   Here we simulate it with a counter 0-100.
   ──────────────────────────────────────────────── */

/* Queue handle — defined in main.c, used here
   extern means "this exists somewhere else"        */
extern QueueHandle_t sensor_queue;

/* Semihosting print — same as Day 1               */
#define SYS_WRITE0 0x04
static void print(const char *s) {
    __asm volatile (
        "mov r0, %[op]\n"
        "mov r1, %[arg]\n"
        "bkpt 0xAB\n"
        :
        : [op] "r" (SYS_WRITE0), [arg] "r" (s)
        : "r0", "r1", "memory"
    );
}

/* ── Sensor task function ────────────────────────
   Every FreeRTOS task:
   1. Takes void *params as argument
   2. Returns void
   3. Has a while(1) loop — never exits
   4. Calls vTaskDelay to give CPU to other tasks  */
void sensor_task(void *params) {
    (void)params; /* suppress unused warning        */

    uint32_t reading = 0;

    print("Sensor task started\n");

    while (1) {
        /* Simulate sensor reading 0 to 100
           Goes up by 10 each time, wraps at 100   */
        reading = (reading + 10) % 110;

        /* Send reading to queue
           &reading = address of our data
           0        = don't wait if queue full      */
        if (xQueueSend(sensor_queue, &reading, 0) == pdPASS) {
            print("Sensor: sent reading\n");
        }

        /* Wait 200ms before next reading
           Task moves to BLOCKED state
           CPU free for other tasks during this time */
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}