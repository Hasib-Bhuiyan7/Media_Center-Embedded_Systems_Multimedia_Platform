\# Media Center — Embedded Systems Multimedia Platform



A complete embedded multimedia platform developed on the \*\*Keil MCB1700 (LPC1768 ARM Cortex-M3)\*\* development board using \*\*Keil µVision\*\*, integrating real-time graphics, USB audio streaming, joystick-driven interfaces, ADC/DAC peripherals, and interactive games into a single state-machine-based embedded application.



This project was designed as the final capstone for \*\*COE718: Embedded Systems Design\*\*, combining nearly every major concept covered throughout the course into one cohesive system. The Media Center operates through a fully interactive LCD-based graphical interface where the user navigates different modules using the on-board joystick. The overall architecture follows a modular FSM-driven implementation, allowing smooth transitions between subsystems while maintaining responsive real-time performance on constrained embedded hardware.



\---



\## Project Overview



The Media Center consists of four primary subsystems:



\* \*\*Photo Gallery\*\*

\* \*\*USB MP3 Player\*\*

\* \*\*Snake Game\*\*

\* \*\*Block Dodger Arcade Game\*\*



All modules are unified under a centralized menu system implemented in `main.c` (`Blinky.c`) using event-driven control and joystick navigation. The project integrates:



\* LCD graphical rendering

\* USB Audio Class communication

\* ADC-based volume control

\* DAC audio output

\* Timer interrupts

\* Real-time joystick input handling

\* Embedded game logic

\* Bitmap image processing

\* Finite State Machine (FSM) architecture



The project emphasizes low-level embedded interaction between hardware peripherals and software modules while maintaining a clean and modular software structure.



\---



\# System Architecture



The Media Center is built around a centralized menu-driven control system. Upon startup, the LCD displays an introductory splash screen followed by the main “Media Hub Menu”. The joystick is used to navigate between subsystems and launch each feature dynamically.



The system architecture follows a modular structure:



\* `Blinky.c` → Main FSM/Menu Controller

\* `Gallery.c` → Bitmap Rendering Engine

\* `usbmain.c` → USB Audio Streaming + ADC/DAC Control

\* `Snake.c` → Snake Game Engine

\* `blockDodger.c` → Arcade Dodger Game Engine



The menu logic continuously polls joystick states through GPIO input and updates highlighted LCD selections in real-time. Each subsystem operates independently and returns control back to the main FSM upon exit.



\---



\# Photo Gallery Module



The Photo Gallery subsystem implements bitmap image rendering on the MCB1700 GLCD display. Since the development board cannot directly interpret `.bmp` files, all images were converted into embedded C-array image data using \*\*GIMP\*\* and stored as RGB565 formatted pixel arrays.



The gallery module:



\* Loads multiple image arrays into memory

\* Uses joystick LEFT/RIGHT controls to scroll through images

\* Dynamically renders images using `GLCD\_Bitmap()`

\* Maintains image scaling and formatting for LCD compatibility

\* Implements lightweight image switching with minimal overhead



Example bitmap resources include:



\* Basketball image

\* Soccer ball image

\* Car image

\* Bird image



The rendering pipeline demonstrates low-level graphical manipulation and embedded LCD interfacing while handling memory limitations on the LPC1768 platform.



\---



\# USB MP3 Player



The MP3 Player subsystem streams real-time audio from a PC to the development board through the USB Audio Class interface. This module was built from the LPC17xx USB Audio example framework and extensively modified for integration into the Media Center environment.



Core functionality includes:



\* USB audio streaming from host PC

\* DAC-driven speaker output

\* Interrupt-driven audio playback

\* ADC-controlled volume adjustment through the on-board potentiometer

\* LCD splash screen and playback interface

\* Dynamic USB connect/disconnect handling



The system configures:



\* ADC channels for potentiometer sampling

\* DAC output for speaker playback

\* TIMER0 interrupts for continuous audio synchronization

\* USB IRQ handling for real-time packet processing



Audio packets are buffered into circular buffers (`DataBuf`) while TIMER0 ISR logic continuously processes samples and outputs scaled analog signals to the DAC. Volume is dynamically adjusted using ADC readings from the potentiometer, allowing real-time analog audio control directly through hardware.



The implementation required synchronization delays, interrupt management, and careful buffer handling to maintain stable audio playback on constrained embedded resources.



\---



\# Snake Game Engine



The Snake Game subsystem is a complete real-time LCD game implemented entirely from scratch using joystick-based movement and frame-by-frame graphical updates.



The game engine includes:



\* Direction-based snake movement

\* Dynamic body growth

\* Randomized food spawning

\* Real-time collision detection

\* Score tracking

\* Difficulty selection (Easy/Hard)

\* Continuous GLCD animation updates

\* Boundary and self-collision logic



The snake body is stored as coordinate arrays, where each frame shifts body segments forward while recalculating head coordinates based on joystick input. Illegal 180-degree turns are prevented through directional state tracking.



The rendering system redraws snake segments and food glyphs every update cycle using GLCD character rendering functions. Food collisions increase both score and snake length, while wall or self-collisions trigger a game-over state and restart logic.



The entire gameplay loop demonstrates:



\* Real-time embedded graphics

\* State management

\* Timer-driven updates

\* Input polling

\* Dynamic memory-style coordinate handling



\---



\# Block Dodger Arcade Game



A second custom arcade-style game was implemented to further extend the multimedia platform and demonstrate more advanced LCD animation techniques.



The Block Dodger subsystem features:



\* Real-time obstacle generation

\* Randomized block spawning

\* Multiple block geometries and sizes

\* Dynamic speed scaling

\* Animated color cycling

\* Collision detection

\* Score accumulation

\* Continuous LCD redraw logic



The player is represented as a movable horizontal bar controlled via joystick LEFT/RIGHT movement. Falling blocks spawn at randomized positions with varying dimensions and descend toward the player at increasing speeds.



A custom pixel-based rectangle rendering function (`DrawRectFilled`) was implemented using `GLCD\_PutPixel()` to create animated game objects directly at the pixel level.



The game continuously:



\* Clears previous object frames

\* Updates object coordinates

\* Cycles object colors

\* Redraws updated geometry

\* Checks collisions in real-time



This subsystem demonstrates low-level graphics programming, procedural animation, collision systems, and dynamic game-state management within an embedded environment.



\---



\# Embedded Systems Concepts Applied



This project integrates a large portion of the embedded systems concepts covered throughout the course, including:



\* ARM Cortex-M3 embedded programming

\* Real-time system design

\* FSM/state-machine implementation

\* Interrupt-driven execution

\* ADC/DAC interfacing

\* USB communication protocols

\* LCD graphical rendering

\* GPIO joystick interfacing

\* Embedded game-loop architecture

\* Memory-conscious bitmap handling

\* Peripheral synchronization

\* Hardware/software integration



The overall system was designed with modularity in mind, allowing independent subsystem development while maintaining centralized system control through the main FSM architecture.



\---



\# Development Environment



\* \*\*Board:\*\* Keil MCB1700 / LPC1768

\* \*\*IDE:\*\* Keil µVision

\* \*\*Language:\*\* Embedded C

\* \*\*Processor:\*\* ARM Cortex-M3

\* \*\*Graphics Library:\*\* GLCD

\* \*\*Input Devices:\*\* Joystick, Potentiometer

\* \*\*Audio Interface:\*\* USB Audio Class + DAC

\* \*\*Image Conversion:\*\* GIMP → RGB565 C Arrays



\---



\# Final Notes



The Media Center project serves as a complete embedded multimedia prototype that combines graphical interfaces, audio streaming, peripheral interfacing, interrupt-driven execution, and interactive gameplay into a single unified platform. Beyond implementing the project specifications, the development process involved designing scalable software architecture, managing hardware limitations, debugging real-time synchronization issues, and integrating multiple subsystems into one responsive embedded environment.



This project ultimately became a practical demonstration of applied embedded systems engineering on ARM-based hardware, emphasizing both low-level hardware interaction and structured software design.



