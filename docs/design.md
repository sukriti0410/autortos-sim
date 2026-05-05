# AutoRTOS-Sim — Design Document

## What This Project Does
Simulates a 3-task automotive ECU running FreeRTOS on 
QEMU-emulated ARM Cortex-M3.

## The 3 Tasks

### Sensor Task (Priority 2)
- Generates fake sensor reading every 200ms
- Sends reading to sensor_queue
- Simulates: wheel speed sensor, temperature sensor etc.

### Processor Task (Priority 3)
- Receives reading from sensor_queue
- If reading > 80 → sends ALERT to result_queue
- If reading <= 80 → sends NORMAL to result_queue
- Simulates: ECU decision-making logic

### Safety Task (Priority 4)
- Receives from result_queue
- If ALERT → prints "SAFE STATE TRIGGERED"
- Simulates: automotive safety response

## Data Flow
Sensor → sensor_queue → Processor → result_queue → Safety

## Task Priorities
Safety    = 4 (highest — must always respond first)
Processor = 3 (medium)
Sensor    = 2 (lowest — just reading data)

## Queue Design
sensor_queue  — holds uint32_t readings (0-100)
result_queue  — holds uint32_t status (0=NORMAL, 1=ALERT)

## Hardware
- CPU: ARM Cortex-M3
- Board: mps2-an385 (emulated by QEMU)
- Clock: 25MHz