 #include <SoftwareSerial.h>
#include <Servo.h>


#define ENB 9

#define LEFT_B 8

#define LEFT_F 7

#define ENA 5

#define RIGHT_F 3

#define RIGHT_B 4

#define TRIG_F 23
#define ECHO_F 22

#define TRIG_B 25
#define ECHO_B 24
//
#define TRIG_R 35
#define ECHO_R 34
//
#define TRIG_L 29
#define ECHO_L 28
 

#define ENCODER_B_A 53

#define ENCODER_B_B 52

#define ENCODER_A_A 51

#define ENCODER_A_B 50


byte encoderBLast;

bool encoderBDir = true;

int encoderBCount = 0;


byte encoderALast;

bool encoderADir = true;

int encoderACount = 0;

const int servoPin = 10;

int state = 80;
int angle = 45;

Servo servo;





void setup() {


  Serial.begin(57600);
  Serial1.begin(9600);

  servo.attach(servoPin);
  servo.write(angle);
  delay(1000);



  

  pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_F, INPUT);

  pinMode(TRIG_B, OUTPUT);
  pinMode(ECHO_B, INPUT);

   pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);

   pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);



  pinMode(ENB, OUTPUT);
  pinMode(LEFT_B, OUTPUT);
  pinMode(LEFT_F, OUTPUT);

 pinMode(ENA, OUTPUT);
  pinMode(RIGHT_F, OUTPUT);
  pinMode(RIGHT_B, OUTPUT);

  pinMode(ENCODER_B_A, INPUT);
  pinMode(ENCODER_B_B, INPUT);

  pinMode(ENCODER_A_A, INPUT);
  pinMode(ENCODER_A_B, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_B_A), ISR_encoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_A), ISR_encoderA, CHANGE);
   if (Serial1.available()) {       
    Serial.write(Serial1.read());
  }
  if (Serial.available()) {         
    Serial1.write(Serial.read());
  }
  Serial.println("Arduino Bluetooth Car is ready to start");

}

 

unsigned long lastTime = 0;

bool logging = false;

char value;
char cmd=0;



void loop() {
  long duration_f, duration_b,duration_r, duration_l;
 long  distance_f, distance_b, distance_r, distance_l;

servo.write(angle);
delay(50);

//if(angle == 80) state = -40;
//
//else if (angle == 0) state = 40;
//angle += state;
//delay(500);


// 전방 초음파
 digitalWrite(TRIG_F, LOW);
 delayMicroseconds(2);
 digitalWrite(TRIG_F, HIGH);
 delayMicroseconds(10);
 digitalWrite(TRIG_F, LOW);
 duration_f = pulseIn (ECHO_F, HIGH);
 distance_f = duration_f * 17 / 1000; 

 Serial.println(duration_f ); //초음파가 반사되어 돌아오는 시간을 보여줍니다.

  Serial.print("\nDIstance_f : ");

  Serial.print(distance_f); //측정된 물체로부터 거리값(cm값)을 보여줍니다.

  Serial.println(" Cm");
  delay(200);

  
 //후방 초음파
  digitalWrite(TRIG_B, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_B, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_B, LOW);
   duration_b = pulseIn (ECHO_B, HIGH);
   distance_b = duration_b * 17 / 1000; 
   
  Serial.println(duration_b ); //초음파가 반사되어 돌아오는 시간을 보여줍니다.

  Serial.print("\nDIstance_b : ");

  Serial.print(distance_b); //측정된 물체로부터 거리값(cm값)을 보여줍니다.

  Serial.println(" Cm");
  delay(200);

  
// 오른쪽 초음파
  digitalWrite(TRIG_R, LOW);
delayMicroseconds(2);
  digitalWrite(TRIG_R, HIGH);
delayMicroseconds(10);
  digitalWrite(TRIG_R, LOW);  
 duration_r = pulseIn (ECHO_R, HIGH);
distance_r = duration_r * 17 / 1000; 
  Serial.println(duration_r ); //초음파가 반사되어 돌아오는 시간을 보여줍니다.
  Serial.print("\nDIstance_r : ");
  Serial.print(distance_r); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
  Serial.println(" Cm");
  delay(200); //1초마다 측정값을 보여줍니다.


// 왼쪽 초음파
  digitalWrite(TRIG_L, LOW);
delayMicroseconds(2);
  digitalWrite(TRIG_L, HIGH);
delayMicroseconds(10);
  digitalWrite(TRIG_L, LOW);  
 duration_l = pulseIn (ECHO_L, HIGH);
distance_l = duration_l * 17 / 1000; 
  Serial.println(duration_l ); //초음파가 반사되어 돌아오는 시간을 보여줍니다.

  Serial.print("\nDIstance_l : ");
  Serial.print(distance_l); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
  Serial.println(" Cm");
  delay(200); //1초마다 측정값을 보여줍니다.
  
  Serial.println("---------------------------");
  
if(Serial1.available()){
value = Serial1.read();
Serial.print(value);
}


switch(value){
  case 'f':
  forward();
  break;
  case 's':
  stop1();
  break;
  case 'b':
  backward();
  break;
  case 'l':
  left();
  break;
  case 'r':
  right();
  break;
  
  case 'w': // 주차 우회전 정방향
  p_right2();
  break;
  case 'k':
  p_left2(); // 주차 좌회전 정방향
  break;

  case 'n':
  p_left2_r(); // 주차 좌회전 역방향
  break;
  
  case 'm':
  p_right2_r(); // 주차 우회전 역방향
  break;


  case 'x': // 평행주차
  if(distance_f > 10)
  {
  forward();
  delay(5000);
  }
  if(distance_r > 10 )
  {
     stop1();
    delay(1000);
    p_left2();
    delay(2000);
    backward();
    delay(3000);
    p_left2_r();
    delay(2000);
    
    

  }
  else if(distance_b < 5)
  {
    stop1();
    
  }
break;

  case 't': // t주차
  
  if(distance_b > 10)
  {
  forward();
  delay(4000);
  }
  
  if(distance_r > 10 )
  {
     stop1();
    delay(1000);
    p_left2();
    delay(1000);
    backward();
    delay(1500);
    p_right2_r();
    delay(1000);
    stop1();
  }

  else if(distance_b < 5)
  {
    stop1();
  }

  
   
  break;
  
  

//  else if (distance_r < 10 && distance_b > 5)
//  {
//    forward();
//  }
//
//  else if (distance_r < 10 && distance_b < 5)
//  {
//    stop1();
//  }
//  break;

  
  case 'a':
 if (distance_f < 8){
   stop1();
   break;
    }

else if (8 < distance_f && distance_f < 30) {

     forward();
     break;    
     }

else if (distance_f >= 30){
       backward();
       break;
       
    } 
    break;
}



}

void ISR_encoderA() {

  byte encoderAA = digitalRead(ENCODER_A_A);
  
  if (encoderALast == LOW && encoderAA == HIGH) 
  {
    byte encoderAB = digitalRead(ENCODER_A_B);
    if (encoderAB == LOW) 
    {
      encoderADir = true;
    }
    else if (encoderAB == HIGH) 
    {
      encoderADir = false;
    }
  }
  
  encoderALast = encoderAA;
  if (encoderADir)
  {
    encoderACount++;
  } else 
  {
    encoderACount--;
  }
}

void ISR_encoderB() {
  byte encoderBA = digitalRead(ENCODER_B_A);
  
  if (encoderBLast == LOW && encoderBA == HIGH) 
  {
    byte encoderBB = digitalRead(ENCODER_B_B);
    if (encoderBB == LOW) 
    {
      encoderBDir = true;      
    } else if (encoderBB == HIGH)
    {
      encoderBDir = false;
    }
  }

  encoderBLast = encoderBA;

  if (encoderBDir) 
  {
    encoderBCount++;
  } else 
  {
    encoderBCount--;
  }
}

void forward(){
      Serial.println("forward");
      digitalWrite(LEFT_F, HIGH); // 정방향
      digitalWrite(LEFT_B, LOW); // 역방향
      digitalWrite(RIGHT_F, HIGH); // 정방향
      digitalWrite(RIGHT_B, LOW); // 역방향
      analogWrite(ENB, 255);
      analogWrite(ENA, 255);
      logging = true;
}
void backward(){
      Serial.println("backward");
      digitalWrite(LEFT_F, LOW);
      digitalWrite(LEFT_B, HIGH);
      digitalWrite(RIGHT_F, LOW);
      digitalWrite(RIGHT_B, HIGH);
      analogWrite(ENB, 255);
      analogWrite(ENA, 255);
      logging = true;
}
void left(){
      Serial.println("left");
      digitalWrite(LEFT_F, LOW);
      digitalWrite(LEFT_B, HIGH);
      digitalWrite(RIGHT_F, HIGH);
      digitalWrite(RIGHT_B, LOW);
      analogWrite(ENB, 255);
      analogWrite(ENA, 255);
      logging = true;
}
void right(){ 
      Serial.println("right");
      digitalWrite(LEFT_F, HIGH); //LEFT
      digitalWrite(LEFT_B, LOW); // LEFT
      digitalWrite(RIGHT_F, LOW); //RIGHT
      digitalWrite(RIGHT_B, HIGH); //RIGHT
      analogWrite(ENB, 255);
      analogWrite(ENA, 255);
}
void stop1(){
     digitalWrite(LEFT_B, LOW);
     digitalWrite(LEFT_F, LOW);
     digitalWrite(RIGHT_B, LOW);
     digitalWrite(RIGHT_F, LOW);
     analogWrite(ENB, 0);
     analogWrite(ENA, 0);
}


void p_left2(){
  Serial.println("p_left");
      digitalWrite(LEFT_F, LOW);
      digitalWrite(LEFT_B, LOW);
      
      digitalWrite(RIGHT_F, HIGH);
      digitalWrite(RIGHT_B, LOW);
      analogWrite(ENB, 0);
      analogWrite(ENA, 255);
      logging = true;
}

void p_left2_r(){
  Serial.println("p_left");
      digitalWrite(LEFT_F, LOW);
      digitalWrite(LEFT_B, LOW);
      
      digitalWrite(RIGHT_F, LOW);
      digitalWrite(RIGHT_B, HIGH);
      analogWrite(ENB, 0);
      analogWrite(ENA, 255);
      logging = true;
}


void p_right2(){
  Serial.println("p_right");
      digitalWrite(LEFT_F, HIGH);
      digitalWrite(LEFT_B, LOW);
      
      digitalWrite(RIGHT_F, LOW);
      digitalWrite(RIGHT_B, LOW);
      analogWrite(ENB, 255);
      analogWrite(ENA, 0);
  
}

void p_right2_r(){
  Serial.println("p_right");
      digitalWrite(LEFT_F, LOW);
      digitalWrite(LEFT_B, HIGH);
      
      digitalWrite(RIGHT_F, LOW);
      digitalWrite(RIGHT_B, LOW);
      analogWrite(ENB, 255);
      analogWrite(ENA, 0);
  
}
