/**
 * Be Creative
**/

// Includes
#include <Servo.h>

// Pin assignments
#define ELEVATOR_ENCODER_PIN 2
#define ELEVATOR_SPEED_PIN 4
#define ELEVATOR_DIR_1_PIN 28
#define ELEVATOR_DIR_2_PIN 29
#define BOTTOM_QUEUE_SERVO_PIN 6
#define TOP_QUEUE_SERVO_PIN 7
#define SLOT_1_SERVO_PIN 8
#define SLOT_2_SERVO_PIN 9
#define SLOT_3_SERVO_PIN 10
#define SLOT_4_SERVO_PIN 11
#define SLOT_5_SERVO_PIN 12
#define SLOT_0_LED_PIN 22
#define SLOT_1_LED_PIN 23
#define SLOT_2_LED_PIN 24
#define SLOT_3_LED_PIN 25
#define SLOT_4_LED_PIN 26
#define SLOT_5_LED_PIN 27
#define BUTTON_PIN 28

// Servo positions
#define SLOT_1_OPEN 0
#define SLOT_1_CLOSE 90
#define SLOT_2_OPEN 0
#define SLOT_2_CLOSE 90
#define SLOT_3_OPEN 0
#define SLOT_3_CLOSE 90
#define SLOT_4_OPEN 0
#define SLOT_4_CLOSE 90
#define SLOT_5_OPEN 0
#define SLOT_5_CLOSE 90
#define TOP_OPEN 0
#define TOP_CLOSE 90
#define BOTTOM_OPEN 0
#define BOTTOM_CLOSE 90

// Create servos
Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;
Servo servo_top;
Servo servo_bottom;

int slot;

void setup() {
  
  Serial.begin(115200);
  
  Serial.println("Beginning setup");
  // Attach servos
  servo_1.attach(SLOT_1_SERVO_PIN);
  servo_2.attach(SLOT_2_SERVO_PIN);
  servo_3.attach(SLOT_3_SERVO_PIN);
  servo_4.attach(SLOT_4_SERVO_PIN);
  servo_5.attach(SLOT_5_SERVO_PIN);
  servo_top.attach(TOP_QUEUE_SERVO_PIN);
  servo_bottom.attach(BOTTOM_QUEUE_SERVO_PIN);
  
  // Set up LEDs
  pinMode(SLOT_0_LED_PIN, OUTPUT);
  pinMode(SLOT_1_LED_PIN, OUTPUT);
  pinMode(SLOT_2_LED_PIN, OUTPUT);
  pinMode(SLOT_3_LED_PIN, OUTPUT);
  pinMode(SLOT_4_LED_PIN, OUTPUT);
  pinMode(SLOT_5_LED_PIN, OUTPUT);
  
  // Set up button
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  
  // Set up motor
  pinMode(ELEVATOR_ENCODER_PIN, INPUT);
  digitalWrite(ELEVATOR_ENCODER_PIN, HIGH);
  pinMode(ELEVATOR_SPEED_PIN, OUTPUT);
  pinMode(ELEVATOR_DIR_1_PIN, OUTPUT);
  pinMode(ELEVATOR_DIR_2_PIN, OUTPUT);
  attachInterrupt(ELEVATOR_ENCODER_PIN, stop_elevator, FALLING);
  
  Serial.println("End of setup");
}

void loop() {
  int pin;
  int chosen_slot = flash_slots();
  
  switch(chosen_slot){
    case 0:
      break;
    case 1:
      servo_1.write(SLOT_1_OPEN);
      break;
    case 2:
      servo_2.write(SLOT_2_OPEN);
      break;
    case 3:
      servo_3.write(SLOT_3_OPEN);
      break;
    case 4:
      servo_4.write(SLOT_4_OPEN);
      break;
    case 5:
      servo_5.write(SLOT_5_OPEN);
      break;
    default:
      pin = -1;
  }
  
  Serial.println("Releasing disk");
  servo_top.write(TOP_OPEN);
  delay(5000);
  Serial.println("Disk released");
  
  Serial.println("Closing slot");
  switch(chosen_slot){
    case 0:
      break;
    case 1:
      servo_1.write(SLOT_1_CLOSE);
      break;
    case 2:
      servo_2.write(SLOT_2_CLOSE);
      break;
    case 3:
      servo_3.write(SLOT_3_CLOSE);
      break;
    case 4:
      servo_4.write(SLOT_4_CLOSE);
      break;
    case 5:
      servo_5.write(SLOT_5_CLOSE);
      break;
    default:
      pin = -1;
  }
  servo_top.write(TOP_CLOSE);
  Serial.println("Slot closed");
  
  Serial.println("Activating elevator");
  digitalWrite(ELEVATOR_DIR_1_PIN, HIGH);
  analogWrite(ELEVATOR_SPEED_PIN, 128);
}

void stop_elevator(){
  analogWrite(ELEVATOR_SPEED_PIN, 0);
  digitalWrite(ELEVATOR_DIR_1_PIN, LOW);
  digitalWrite(ELEVATOR_DIR_2_PIN, LOW);
  Serial.print("Stopped elevator");
}

int flash_slots(){
  
  int pin;
  unsigned long last_time;
  
  digitalWrite(SLOT_0_LED_PIN, LOW);
  digitalWrite(SLOT_1_LED_PIN, LOW);
  digitalWrite(SLOT_2_LED_PIN, LOW);
  digitalWrite(SLOT_3_LED_PIN, LOW);
  digitalWrite(SLOT_4_LED_PIN, LOW);
  digitalWrite(SLOT_5_LED_PIN, LOW);
  
  while(true){
    slot = (slot + 1) % 6;
    
    switch(slot){
      case 0:
        pin = SLOT_0_LED_PIN;
        break;
      case 1:
        pin = SLOT_1_LED_PIN;
        break;
      case 2:
        pin = SLOT_2_LED_PIN;
        break;
      case 3:
        pin = SLOT_3_LED_PIN;
        break;
      case 4:
        pin = SLOT_4_LED_PIN;
        break;
      case 5:
        pin = SLOT_5_LED_PIN;
        break;
      default:
        pin = -1;
    }
    
    digitalWrite(pin, HIGH);
    
    last_time = millis();
    while(millis() - last_time < 1000){
      if(digitalRead(BUTTON_PIN) == LOW){
        Serial.print("Chosen slot ");
        Serial.println(slot);
        return slot;
      }
    }
  }
}
