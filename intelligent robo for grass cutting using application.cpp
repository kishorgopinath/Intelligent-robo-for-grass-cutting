

#include <AFMotor.h>  //Adafruit motor sheild for DC and SERVO MOTOR
#include <NewPing.h>  //Ultrasonics sensor 
#include <Servo.h>    //Servo

#define TRIG_PIN A0  //trigger pin for ultrasonics sensor
#define ECHO_PIN A1  //eco pin for ultrasonics sensor
#define MAX_DISTANCE 200 
#define MAX_SPEED 200 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

AF_DCMotor leftmotor(2, MOTOR12_8KHZ); //leftmotor connected to port (M2)motor
AF_DCMotor rightmotor(3, MOTOR12_8KHZ); //rightmotor connected to port (M3)motor
AF_DCMotor grasscutter(4, MOTOR12_8KHZ);//grasscutter  motor connected to port (M4)motor 
Servo neckservo;  //SERVO 9th PIN arduino
Servo rackservo;  //SERVO 10th PIN arduino


// tcs 3200 Pin definitions

const int S0 = 53;  
const int S1 = 52;  
const int S2 = 24;  
const int S3 = 26;  
const int sensorOut = 22;   
// Variables  
int red = 0;  
int green = 0;  
int blue = 0;

boolean goesForward=false;
int distance = 100;
int speedSet = 0;
int distanceR = 0;
int distanceL =  0;
 
char bt;

void setup() {
  
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

   Serial.begin(9600);
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,HIGH);

  grasscutter.setSpeed(255);
 
  Serial.begin(9600);//baud rate for arduino mega 2650
  Serial3.begin(9600);//baud rate for bluetooth module HC-05
  neckservo.attach(9); 
  rackservo.attach(10); 
  rackservo.write(0);//initial position of rack servo
}

  void  obstacledetection()

{   neckservo.write(115); 
  delay(10);
  distance = readPing();
  delay(10);
  distance = readPing();
  delay(10);
  distance = readPing();
  delay(10);
  distance = readPing();
  delay(10);
 
 if(distance<=35)//maximum obstacle detection from robo 35cm
 {
  moveStop();
  delay(20);
  moveBackward();
  delay(1500);
  moveStop();
  delay(20);
  distanceR = lookRight();
  delay(100);
  distanceL = lookLeft();
  delay(100);

  if(distanceR>=distanceL)
  {
    turnLeft();
    moveStop();
  }else
  {
    turnRight();
    moveStop();
  }
 }else
 {
  colordetection();
  moveForward();
  
 }
 distance = readPing();

}

void colordetection()
{
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  red = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  red = map(red, 25,72,255,0);
  // Printing the value on the serial monitor
  Serial.print("red= ");//printing name
  //Serial.print(frequency);//printing RED color frequency
   Serial.print(red);
  Serial.print("  ");

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  green = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  green = map(green, 30,90,255,0);
  // Printing the value on the serial monitor
  Serial.print("green= ");//printing name
  //Serial.print(frequency);//printing RED color frequency
  Serial.print(green);
  Serial.print("  ");
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  blue = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  blue = map(blue, 25,70,255,0);
  // Printing the value on the serial monitor
  Serial.print("blue= ");//printing name
 // Serial.print(frequency);//printing RED color frequency
  Serial.print(blue);
  Serial.println("  ");
   
  if (green > red && green > blue)  
  {  
   Serial.println(" - (Green Color)");  
  grasscutter.run(FORWARD);
 
 }  
  else{
  Serial.println();  
  }  
  delay(300);
  grasscutter.run(RELEASE);
}



void loop() {

if(Serial3.available()>0 )
   bt = Serial3.read();//tx3 and rx3 of arduino mega 2650
   Serial.println(bt);
   colordetection();
 { if(bt == 'S'){  colordetection(); obstacledetection();}
   else if(bt=='s'){moveStop();}
   else if(bt=='a'){rackservo.write(20);}
   else if(bt=='c'){rackservo.write(40);}
   else if(bt=='d'){rackservo.write(60);}
  else if(bt=='e'){rackservo.write(80);}
  else if(bt=='g'){rackservo.write(100);}
  else if(bt=='h'){rackservo.write(120);}
  else if(bt=='k'){rackservo.write(10);}
  else if(bt=='l'){rackservo.write(30);}
  else if(bt=='m'){rackservo.write(50);}
  else if(bt=='n'){rackservo.write(70);}
  else if(bt=='o'){rackservo.write(90);}
  else if(bt=='p'){rackservo.write(110);}
  else if(bt=='j'){rackservo.write(0);}
  else if (bt=='F'){goFRONT();colordetection();}
  else if (bt=='B'){goBACK();colordetection();}
  else if (bt=='R'){goRIGHT();}
  else if (bt=='L'){goLEFT();}


 }
}
int lookRight()
{
    neckservo.write(50); 
    delay(500);
    int distance = readPing();
    delay(100);
    neckservo.write(115); 
    return distance;
}

int lookLeft()
{
    neckservo.write(170); 
    delay(100);
    int distance = readPing();
    delay(100);
    neckservo.write(115); 
    return distance;
    delay(10);
}

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

void moveStop() {
 leftmotor.run(RELEASE);
  rightmotor.run(RELEASE);
  } 
  
void moveForward() {

 if(!goesForward)
  {
    goesForward=true;
    leftmotor.run(FORWARD); 
    rightmotor.run(FORWARD);     
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    leftmotor.setSpeed(speedSet);
    rightmotor.setSpeed(speedSet);
   }
  }
}

void moveBackward() {
    goesForward=false;
    leftmotor.run(BACKWARD);
    rightmotor.run(BACKWARD);  
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftmotor.setSpeed(speedSet);
    rightmotor.setSpeed(speedSet);
  }
}  

void turnRight() {
 leftmotor.setSpeed(200);
  rightmotor.setSpeed(255);
  leftmotor.run(FORWARD);
  rightmotor.run(BACKWARD);     
  delay(1500);
  leftmotor.setSpeed(150);
  rightmotor.setSpeed(150);
  leftmotor.run(FORWARD);      
  rightmotor.run(FORWARD);    
} 
 
void turnLeft() {
  leftmotor.setSpeed(255);
  rightmotor.setSpeed(200);  
  leftmotor.run(BACKWARD);
  rightmotor.run(FORWARD);   
  delay(1500);
  leftmotor.setSpeed(150);
  rightmotor.setSpeed(150);
  leftmotor.run(FORWARD);
  rightmotor.run(FORWARD);
} 

void goFRONT()
{
  leftmotor.setSpeed(210);
  rightmotor.setSpeed(210);  
  leftmotor.run(FORWARD);
  rightmotor.run(FORWARD);
}

void goBACK()
{
  leftmotor.setSpeed(210);
  rightmotor.setSpeed(210);  
  leftmotor.run(BACKWARD);
  rightmotor.run(BACKWARD);
}

void goRIGHT()
{
  leftmotor.setSpeed(250);
  rightmotor.setSpeed(200);  
  leftmotor.run(BACKWARD);
  rightmotor.run(FORWARD);
}

void goLEFT()
{
  leftmotor.setSpeed(200);
  rightmotor.setSpeed(250);  
  leftmotor.run(FORWARD);
  rightmotor.run(BACKWARD);
}