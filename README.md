# snek

Two-player snake game built for Arduino UNO with an 8x8 LED matrix display and dual analog joystick input.

## Hardware

- Arduino UNO
- MAX7219 LED matrix (8x8, controlled via SPI)
- 2x analog joysticks (4 analog pins: A0–A3)

## Features

- Two-player competitive gameplay on a shared 32x32 grid
- SPI-driven LED matrix rendering via LedControl library
- Joystick input with direction-reversal prevention
- Collision detection (boundary, food, snake-to-snake)
- Game state machine (menu, playing, game over)
- Tail tracking with configurable max snake length

## Pin Configuration

| Component | Pin |
|-----------|-----|
| LED DIN | 7 |
| LED CS | 6 |
| LED CLK | 5 |
| Joystick 1 X/Y | A0/A1 |
| Joystick 2 X/Y | A2/A3 |

Built with Arduino C++ for a computer engineering course at the University of Pittsburgh.
