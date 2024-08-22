# Project Overview

This project runs on an Arduino Uno using the ChibiOS/RT operating system.

## Setup Instructions

1. **Makefile Adjustments**  
   You may need to modify the `Makefile` according to your specific setup. Common adjustments include:
   - Setting the correct `tty` for your Arduino board.
   - Adjusting the relative path to the ChibiOS main directory.

2. **Library Installation**  
   Ensure that the necessary files are correctly placed in your ChibiOS directory:
   - Navigate to `$(CHIBIOS)/os/hal/lib/`.
   - Create a new folder named `lcd`.
   - Copy `liquidcrystal.c`, `liquidcrystal.h`, and `lcd.mk` into the `lcd` directory.

## Requirements
- **Hardware:** Arduino Uno
- **Software:** ChibiOS/RT
