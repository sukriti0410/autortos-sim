#ifndef PRINT_HELPER_H
#define PRINT_HELPER_H

#include <stdint.h>

static void itm_init(void) { }  /* empty — keeps main.c happy */

/* ── Semihosting SYS_WRITE0 ─────────────────────────
   syscall 4 = SYS_WRITE0
   prints null-terminated string
   Uses SVC instruction instead of BKPT
   SVC works correctly from FreeRTOS task context     */
static void print(const char *s) {
    __asm volatile (
        "mov r1, %0\n"
        "mov r0, #4\n"
        "bkpt 0xAB\n"
        :
        : "r" (s)
        : "r0", "r1", "memory"
    );
}

#endif