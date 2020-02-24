# Rubik Cube Solver
## About the project

This is my final project for the Introduction to Robotics course. It is a robot that scans and solves a Rubik Cube.

## Project specifications

I will split this project in two parts: hardware and software.

### Hardware

#### Hardware components

1. Arduino Uno
2. 2 x MG996R Servo motors
3. Logitech C525 webcam
4. Wires
5. Breadboard

#### Other components

1. Lots of icecream sticks
2. Piece of hardwood
3. 10 x L shaped metal

#### How the hardware works

The hardware is quite simple. I use two servo motors: one for the arm, one for the cube holder.

The arm has two functionalities: it can hold the top and middle layer of the cube still, or it can push the cube.

The cube holder has only one functionality: it rotates the cube. If the arm locks the top and middle layer, then the cube holder rotates and the bottom layer moves.

#### Mechanism

The mechanism I used is inspired from a [youtube video](https://www.youtube.com/watch?v=NRRSYEWIQ_w&t=1s) to which I added the webcam feature. Building the arm and everything was more about trial and error than calculating distances and stuff like that. For most pieces, I had at lease one more prototype before I stuck to what the project looks like now.

### Software





## Image 

![Project image](project.jpg)

## Video 

https://youtu.be/QNpVnlfoA0Q

## Used libraries

1. [LiquidMenu](https://github.com/VaSe7u/LiquidMenu) is a really easy to use library to make quick menus on a 16x2 LCD Display. I recommend it for quick PoC, but proves to be quite memory heavy for bigger projects.
2. LedControl is used for controlling the LED's on the two matrixes.
3. LiquidCrystal is used for controlling the LCD.
