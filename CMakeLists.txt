cmake_minimum_required(VERSION 3.16)
project(autortos-sim C ASM)

set(CMAKE_C_STANDARD 99)

# ── FreeRTOS kernel sources ───────────────────────
set(FREERTOS_DIR ${CMAKE_SOURCE_DIR}/FreeRTOS-Kernel)

set(FREERTOS_SOURCES
    ${FREERTOS_DIR}/tasks.c
    ${FREERTOS_DIR}/queue.c
    ${FREERTOS_DIR}/list.c
    ${FREERTOS_DIR}/timers.c
    ${FREERTOS_DIR}/portable/GCC/ARM_CM3/port.c
    ${FREERTOS_DIR}/portable/MemMang/heap_4.c
)

# ── Your application sources ──────────────────────
set(APP_SOURCES
    src/startup.c
    src/main.c
    src/sensor_task.c
    src/processor_task.c
    src/safety_task.c
)

# ── Build target ──────────────────────────────────
add_executable(autortos-sim.elf
    ${APP_SOURCES}
    ${FREERTOS_SOURCES}
)

# ── Include paths ─────────────────────────────────
target_include_directories(autortos-sim.elf PRIVATE
    src/
    ${FREERTOS_DIR}/include
    ${FREERTOS_DIR}/portable/GCC/ARM_CM3
)

# ── Compiler flags ────────────────────────────────
target_compile_options(autortos-sim.elf PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -Wall
    -Wextra
    -Os
)

# ── Linker flags ──────────────────────────────────
target_link_options(autortos-sim.elf PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -T ${CMAKE_SOURCE_DIR}/src/link.ld
    -Wl,--gc-sections
    -Wl,--start-group
    -lc
    -lgcc
    -Wl,--end-group
)