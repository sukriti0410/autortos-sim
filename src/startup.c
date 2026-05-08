#include <stdint.h>

extern uint32_t _stack_top;
extern int main(void);

/* ── FreeRTOS V11 handler names ──────────────────────
   FreeRTOS V11 port.c defines these exact names
   Must match exactly or scheduler never starts        */
extern void vPortSVCHandler(void);
extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);

void Reset_Handler(void);
void Default_Handler(void);

void Default_Handler(void) { while (1) {} }

void Reset_Handler(void) {
    main();
    while (1) {}
}

/* ── Vector table ────────────────────────────────────
   Position 11 = vPortSVCHandler   (FreeRTOS task start)
   Position 14 = xPortPendSVHandler (FreeRTOS context switch)
   Position 15 = xPortSysTickHandler (FreeRTOS tick)    */
__attribute__((section(".isr_vector")))
void (*const vector_table[])(void) = {
    (void (*)(void))&_stack_top,  /*  0: stack pointer  */
    Reset_Handler,                 /*  1: reset          */
    Default_Handler,               /*  2: NMI            */
    Default_Handler,               /*  3: HardFault      */
    Default_Handler,               /*  4: MemManage      */
    Default_Handler,               /*  5: BusFault       */
    Default_Handler,               /*  6: UsageFault     */
    0,                             /*  7: reserved       */
    0,                             /*  8: reserved       */
    0,                             /*  9: reserved       */
    0,                             /* 10: reserved       */
    vPortSVCHandler,               /* 11: SVCall ✅      */
    Default_Handler,               /* 12: DebugMon       */
    0,                             /* 13: reserved       */
    xPortPendSVHandler,            /* 14: PendSV ✅      */
    xPortSysTickHandler,           /* 15: SysTick ✅     */
};