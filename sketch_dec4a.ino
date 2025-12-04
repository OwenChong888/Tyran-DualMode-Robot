#include <Servo.h>
#include <SoftwareSerial.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define in1 12 // LEFT
#define in2 11 // LEFT
#define in3 7 // RIGHT
#define in4 6 // RIGHT
#define enA 3
#define enB 5

#define ir1 A0 //leftmost
#define ir2 A1
#define ir3 A2 //middle
#define ir4 A3
#define ir5 2 //rightmost

#define RST_PIN -1
#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;


int Left_Speed = 55;
int Right_Speed = 75;

String currentAction = "Stopped"; 

Servo myServo;
Servo myServo2;
const int servoPin = 4;
const int servoPin2 = 13;

SoftwareSerial BT(8,9); //pin8 TX, pin9 RX
char command;

enum RobotMode{
  Manual,
  Auto1, //haven't select destination
  Auto2
};

RobotMode currentMode = Manual; // initialize
int targetIntersection; 
int currentIntersection = 0; 
bool isCrossing = false; 

void updateDisplay() {
  oled.clear();
  oled.set1X();
  oled.setCursor(0, 0);

  switch (currentMode) {
    case Manual:
      oled.set2X();
      oled.setCursor(0, 1);
      oled.println(F("MANUAL MODE"));
      oled.set1X();
      oled.setCursor(0, 4);
      oled.print(F("Action: "));
      oled.println(currentAction);
      oled.println(F("Use BT for Control"));
      
      break;
    
    case Auto1:
      oled.set2X();
      oled.setCursor(0, 1);
      oled.println(F("AUTO MODE"));
      oled.set1X();
      oled.setCursor(0, 4);
      oled.println(F("Select Dest"));
      oled.setCursor(0, 5);
      oled.println(F("(1-3)"));
      break;

    case Auto2:
      oled.set2X();
      oled.setCursor(0, 1);
      oled.print(F("Target: "));
      oled.println(targetIntersection);
      oled.set1X();
      oled.setCursor(0, 4);
      oled.print(F("Action: "));
      oled.println(currentAction);
      break;
 }
}

void handleCommand(char cmd) {
  // Mode Switching
  if (cmd == 'M') {
    currentMode = Manual;
    stop();
    Serial.println("Mode: MANUAL");
    currentAction = "Stopped";

  } 
  else if (cmd == 'X') {
    currentMode = Auto1;
    stop();
    Serial.println("Mode: AUTO (Select Destination 1-3)");
    currentAction = "Waiting";
  }
  
  // Destination Selection (Only works if in AUTO modes)
  if (currentMode == Auto1 || currentMode == Auto2) {
    if (cmd == '1') { startAutoDrive(1); }
    if (cmd == '2') { startAutoDrive(2); }
    if (cmd == '3') { startAutoDrive(3); }
  }

  // Manual Driving Controls (Only works in MANUAL mode)
  if (currentMode == Manual) {
    if (cmd == 'F') {forward();currentAction="Forward";}
    else if (cmd == 'B') {backward();currentAction="Backward";}
    else if (cmd == 'L') {left();currentAction="Turn Left";}
    else if (cmd == 'R') {right();currentAction="Turn Right";}
    else if (cmd == 'S') {stop();currentAction="Stopped";}

    else if(cmd == 'G') {
      myServo.write(40);
      myServo2.write(40);
      Serial.println("Grip");
      currentAction="Grip";
    }
    else if(cmd == 'O') {
      myServo.write(90);
      myServo2.write(0);
      Serial.println("Open");
      currentAction="Open";
    }
  }
}

void startAutoDrive(int dest) {
  targetIntersection = dest;
  currentIntersection = 0;
  isCrossing = false;
  currentMode = Auto2;
  Serial.print("Going to Destination: ");
  Serial.println(dest);
  currentAction="Go To Dest";
}

void setup() {
  Serial.begin(9600);
  
  #if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else 
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif 
  oled.setFont(System5x7);
  oled.clear();
  oled.print(F("Hello world!"));

  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);     
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(enB,OUTPUT);

  pinMode(ir1,INPUT);  
  pinMode(ir2,INPUT);  
  pinMode(ir3,INPUT);
  pinMode(ir4,INPUT);
  pinMode(ir5,INPUT);  

  myServo.attach(servoPin); 
  myServo2.attach(servoPin2);

  myServo.write(90);
  myServo2.write(0);


  Serial.println("Bluetooth connected, default mode: Manual");
  BT.begin(9600);

}

void loop() {
  if(BT.available()){
    command = BT.read();
    handleCommand(command);
  }

  switch (currentMode) {
    case Manual:
      break;
    
    case Auto1:
      stop();
      break;// Select 1, 2, 3

    case Auto2:
    linefollowing();
    break;
  }
  updateDisplay();
}

// line following logic
void linefollowing() {
  int IR1 = digitalRead(ir1);
  int IR2 = digitalRead(ir2);
  int IR3 = digitalRead(ir3);
  int IR4 = digitalRead(ir4);
  int IR5 = digitalRead(ir5);

  // Middle sensor detects black line
  if(IR1==1 && IR2==1 && IR3==0 && IR4==1 && IR5==1){
    forward();
    currentAction = "Forward";
  }
  // Left sensor detects black line
  else if((IR1==1 && IR2==0 && IR3==1 && IR4==1 && IR5==1) ||
    (IR1==0 && IR2==1 && IR3==1 && IR4==1 && IR5==1)){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, 60);
    analogWrite(enB, 70);
    currentAction = "Turn Left";
  }
  // Right sensor detects black line
  else if((IR1==1 && IR2==1 && IR3==1 && IR4==1 && IR5==0)  ||
    (IR1==1 && IR2==1 && IR3==1 && IR4==0 && IR5==1) ||
    (IR1==1 && IR2==1 && IR3==1 && IR4==0 && IR5==0) ||
    (IR1==1 && IR2==1 && IR3==0 && IR4==0 && IR5==0)){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);

    analogWrite(enA, 60);
    analogWrite(enB, 70);
    currentAction = "Turn Right";
  }

  else if((IR1==0 && IR2==0 && IR3==0 && IR4==1 && IR5==1) ||
    (IR1==0 && IR2==0 && IR3==0 && IR4==0 && IR5==1) ||
    (IR1==0 && IR2==0 && IR3==1 && IR4==1 && IR5==1)){
    currentIntersection = currentIntersection + 1;
    if(currentIntersection != targetIntersection){
      forward();
      currentAction = "Cross Intersection";
      delay(400);
    }
      
    else{
      left();
      delay(400);
      oled.clear();
      oled.set1X();
      oled.setCursor(0, 16);
      oled.println("DESTINATION");
      oled.setCursor(0, 40);
      oled.print("REACHED:DEST ");
      oled.println(targetIntersection);
      currentAction = "Reached"; 
      currentMode = Manual;
    }
  }

  // All sensors detect black line
  else if(IR1==0 && IR2==0 && IR3==0 && IR4==0 && IR5==0){
    stop();
    currentAction = "Stopped";
  }
}

//direction function
void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, Left_Speed);
  analogWrite(enB, Right_Speed);
}

void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, Left_Speed);
  analogWrite(enB, Right_Speed);
}

void right() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  analogWrite(enA, Left_Speed);
  analogWrite(enB, Right_Speed);
}

void left() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, Left_Speed);
  analogWrite(enB, Right_Speed);
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  analogWrite(enA, 0);
  analogWrite(enB, 0);
}