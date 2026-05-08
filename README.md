# Connect 4 Playing Robot

An autonomous robot designed to play Connect 4 against a human opponent using piece sensing, game AI, and electromechanical actuation. 

## Project Goals
- Detect and map Connect 4 board state using sensors
- Implement game-playing AI using minimax algorithm and heuristics
- Physical PID actuation to place game pieces
- Enable human-vs-robot interface and gameplay

## How it Works
**Mechanical Design:**\
The piece cart is made up of two parts: the tray, free to rotate about a single rod, and the servo car which tilts the free-moving tray to slide the piece into the column funnels. This cart is attached to a GT2 timing belt which is pulled by a cheap DC motor geared down with a ratio of 25:1.<br>
**Control System:**\
Since I was using a cheap DC motor, I needed to create a custom encoding setup. Using a quadrature encoder with a 16 slot encoder wheel attached to the output shaft of the gearbox, I was able to achieve a resolution of about 0.625 mm with the PID control system.\
**Sensing:**\
An IR sensor is embedded in each funnel of the robot, and the signals are fed directly into the ADC inputs of the Arduino. The digital data is put through an Exponential Moving Average (EMA) filter to cut off the high frequency noise, then through a first difference filter to for edge detection. Frequency response, pole-zero, and other filter analysis was done in MATLAB (see `src/External/filter/`).\
**Gameplay:**\
The robot makes its move-making decisions using a pruned Minimax algorithm with a linear game-state evaluator. The Minimax code is run in optimized C++ code and threaded to parallelize branch traversal. This approach was able to achieve a base search depth of 7 moves, and depth is dynamically increased as the search tree gets smaller throughout the game.\
**Embedded:**\


## Current Progress
**Completed:** 
- Mechanical design and PID controls
- Game logic and bitboard representation
- AI minimax algorithm, optimized threaded C++ code on Raspberry Pi
- IR sensor circuitry & digital filtering for move detection
- Raspberry Pi and Arduino UART communication
- Full system integration

**In Progress / Unfinished:**
- Minimax heuristic NES optimization

**Planned Future Work:**
-  Custom PCB design
-  Design piece magazine and automatic loader for fully autonomous gameplay

## Code Structure
- `Connect4_Robot_main.py` - Main python file to run system
- `build.sh` - Compiles C++ code into `src/build/`, user must move .so file into `src/` once built
- `tests/` - Scripts used to for testing and debugging of various features, not documented
- `src/` - Contains all files used directly, files for compiling C++ code
- `src/old/` - Scripts and definitions no longer in use, old python implementations
- `src/External/arduino/` - Embedded Arduino C++ code for motor control and reading sensors
- `src/External/game_ai/` - Python-based Connect 4 game logic and minimax AI, including NES weight optimization algorithm
- `src/External/filter/` - MATLAB script for IR sensing filter frequency response, phase response, pole-zero, sample signal filtering analysis

## Media
_Mechanical prototype. Click for video demo (11/13/2025)_

[![Piece dropping prototyping](media/connect-4-prototype-thumbnail.jpg)](https://youtu.be/RXw5a7y7fcY)

_Sensor circuitry prototyping (11/19/2025)_

![Sensor prototyping photo](media/sensors-prototyping.jpg)

## Status 
**Not Currently In Development**
