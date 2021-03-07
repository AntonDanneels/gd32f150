# gd32f150

This is my personal repository for the gd32f150 microcontroller, with selfmade breakout board and peripheral library with the purpose of learning low level bringup.

The gd32f150 is a small & cheap microcontroller from GigaDevice that has USB device support & all other standard peripherals you would expect from a modern ARM mcu.

## Breakout board

A very simple 2-layer PCB is provided in the `breakout` directory.

## Library 

Generic code to use the peripherals is located in the `lib` directory. Current supported features:

| Feature       | State                |
| ------------- | -------------        |
| SPI           | Initial bringup done |
| GPIO          | Initial bringup done |
| RCU           | Initial bringup done |
| RTC           | Initial bringup done |
| PMU           | Initial bringup done |
| EXTI          | Initial bringup done |
| I2C           | WIP                  |
| ADC           | Planned              |
| USB           | Planned              |

