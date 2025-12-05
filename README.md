# Tyran — Dual-Mode Autonomous Robot

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
![Arduino](https://img.shields.io/badge/Arduino-Uno-blue)

Tyran — Dual-Mode Autonomous Robot

A versatile Arduino-based mobile robot capable of autonomous navigation (line following + destination searching) and manual Bluetooth control with a servo gripper.
Designed for flexibility, expandability, and educational robotics projects.

🚀 Features
🔹 Autonomous Mode

Line following using 5 channel IR sensors

Destination detection logic

🔹 Manual Mode

Bluetooth HC-05 remote control

Servo-controlled gripper (2× SG90) for picking and placing

Smooth proportional motor speed control

🔹 Display System

OLED screen for:

Real-time status feedback

Current mode

Signals to the user

🧠 System Overview

Tyran operates in two switches modes:

Mode	Description
Autonomous	Follows line path, searches for specific patterns/destinations, executes defined tasks.
Manual	Controlled via Bluetooth app; joystick-like movement + gripper control.
🔧 Hardware Used
Component	Description
Arduino UNO	Main microcontroller
L298N Motor Driver	Drives dual DC motors
OLED 128×64	Display system
HC-05	Bluetooth module
SG90 Servo × 2	Gripper mechanism
18650 Battery ×5	Power supply
2-Wheel Motor Chassis	Base platform
IR Sensors	Line tracking
🪛 Pin Connections

(Adjust according to your code — here is a template)

Motor Driver (L298N)
Function	Arduino Pin
ENA	3
IN1	12
IN2	11
IN3	7
IN4	6
ENB	5
Bluetooth HC-05
HC-05 Pin	Arduino Pin
TX	8 / SoftwareSerial RX
RX	9 / SoftwareSerial TX
Servos
Servo	Pin
Gripper Servo 1	12
Gripper Servo 2	4
OLED

▶️ How to Use
1. Install Required Libraries

SSD1306Ascii

Servo

SoftwareSerial (built-in)

2. Upload the Code

Open tyran_robot.ino → Select Arduino UNO → Upload.

3. Power the Robot

Use 3 × 18650 batteries for motor and arduino
Use 2 x 18650 batteries for servo 
(through a proper holder + 5V regulator if needed).

4. Control Modes

Button A / Switch → Autonomous Mode

Button X / Bluetooth → Manual Mode

Use any Bluetooth controller app to send commands.

🖥️ Environment Setup & Installation
1. Install Arduino IDE

Download from the official Arduino website.
Supports Windows, macOS, and Linux.

2. Install Required Libraries

Open:
Arduino IDE → Tools → Manage Libraries

Install:

SSD1306Ascii

Servo

SoftwareSerial is built-in

3. Select the Correct Board

In Arduino IDE:

Tools → Board → Arduino AVR Boards → Arduino Uno

4. Select the COM Port

Tools → Port → COMx (Arduino Uno)

If not detected, install CH340 driver (for Uno clones).

5. Upload the Code

Clone/download this repository

Open ./code/tyran_robot.ino

Click:

✔ Verify

→ Upload

▶️ How to Use the Robot
Autonomous Mode

Switch robot to autonomous

Robot will follow lines using IR sensors

OLED displays mode and debug info

Manual Mode

Pair phone with HC-05 (default PIN: 1234 or 0000)

Open any Bluetooth controller app

Control movement + servo gripper

🎥 Photo

<img width="1280" height="960" alt="image" src="https://github.com/user-attachments/assets/fb14a535-4a5b-4f88-9da0-cfb7d94e3cf4" />


