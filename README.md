# Middlewares and Drivers

This project holds a simple middleware interface for each common internal peripheral
used in embedded projects for Tiva C. This middlewares were tested on FreeRTOS.

This is work in progress.

# Future work

For now, each driver works using polling. In the next phase it will include an _open_ method with
nonblocking capabilities which uses interrupts per peripheral (if possible).

