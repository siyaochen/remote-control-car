#include <IRremote.h>
#include <Servo.h>

#define E1 A0 // Enable pin for motor 1
#define E2 A1 // Enable pin for motor 2
#define I1 2 // Control pin 1 for motor 1
#define I2 3 // Control pin 2 for motor 1
#define I3 4 // Control pin 1 for motor 2
#define I4 5 // Control pin 2 for motor 2

// Servo objects
Servo Servo_0;

int IR = A5; // Pin for IR sensor
IRrecv receiver(IR);
decode_results output;
unsigned long key_value = 0;
int clawFrequency = 0;


void setup() {
  Serial.begin(9600);

  // Initialize IR receiver
  receiver.enableIRIn();

  // Initialize motor
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(I1, OUTPUT);
  pinMode(I2, OUTPUT);
  pinMode(I3, OUTPUT);
  pinMode(I4, OUTPUT);

  // Attach the servos on pins to the servo object
  Servo_0.attach(6);
}


void loop() {
  // Process signal if received
  if (receiver.decode(&output)) {
    if (output.bits > 0) {
      if (output.value == 0XFFFFFFFF) {
        // Nothing
        output.value = key_value;
      }
      
      switch (output.value) {
        case 0xFFE01F:
          // Close claw
          Serial.println("-");
          for (int i = 0; i <= 20; i++) {
            if (clawFrequency >= 0) {
              clawFrequency -= 10;
            }
            delay(20);
            Servo_0.write(clawFrequency);
          }
          break ;
        case 0xFFA857:
          // Open claw
          Serial.println("+");
          for (int i = 0; i <= 20; i++) {
            if (clawFrequency <= 255) {
              clawFrequency += 10;
            }
            delay(20);
            Servo_0.write(clawFrequency);
          }
          break ;
        case 0xFF18E7:
          Serial.println("2");
          moveForward();
          break ;
        case 0xFF7A85:
          Serial.println("3");
          break ;
        case 0xFF10EF:
          Serial.println("4");
          turnLeft();
          break ;
        case 0xFF38C7:
          Serial.println("5");
          stopCar();
          break ;
        case 0xFF5AA5:
          Serial.println("6");
          turnRight();
          break ;
        case 0xFF42BD:
          Serial.println("7");
          break ;
        case 0xFF4AB5:
          Serial.println("8");
          break ;
        case 0xFF52AD:
          Serial.println("9");
          break ;
      }

      // Update key value
      key_value = output.value;
    }
    
    Serial.println(output.value, HEX);
    
    // Receive the next value
    receiver.resume();
  }
}


void moveForward() {
  // Change power of motors (calibrated)
  analogWrite(E1, 1000);
  analogWrite(E2, 1000);

  // Set direction of wheels to move forward
  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);

  delay(1000);

  stopCar();
}


void turnRight() {
  // Change power of motors (calibrated)
  analogWrite(E1, 200);
  analogWrite(E2, 200);

  // Set wheel directions to turn right
  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  digitalWrite(I3, HIGH);
  digitalWrite(I4, LOW);

  delay(100);

  stopCar();
}


void turnLeft() {
  // Change power of motors (calibrated)
  analogWrite(E1, 100);
  analogWrite(E2, 100);

  // Set wheel directions to turn left
  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);

  delay(100);

  stopCar();
}


void stopCar() {
  // Turn wheels off
  analogWrite(E1, 0);
  analogWrite(E2, 0);
}

