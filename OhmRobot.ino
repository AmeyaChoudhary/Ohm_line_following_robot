#include <AFMotor.h>

// Create motor objects
AF_DCMotor leftForward(1);  // Motor on M1
AF_DCMotor leftBack(2);     // Motor on M2
AF_DCMotor rightBack(3);    // Motor on M3
AF_DCMotor rightForward(4); // Motor on M4

// Define constants
const int turn90DegreesTime = 1100; // Average time in milliseconds for a 90-degree turn
const int baseSpeed = 100;          // Base speed of the motors
const int turnSpeed = 120;          // Speed for turning

// IR sensor pins
const int irSensorLeftPin = A0;
const int irSensorRightPin = A2;

// Setup function
void setup() {
  // Set up motor speed
  leftForward.setSpeed(baseSpeed);
  leftBack.setSpeed(baseSpeed);
  rightBack.setSpeed(baseSpeed);
  rightForward.setSpeed(baseSpeed);

  // Initialize IR sensor pins
  pinMode(irSensorLeftPin, INPUT);
  pinMode(irSensorRightPin, INPUT);

  // Start serial communication
  Serial.begin(9600);
}

// Loop function
void loop() {
  // Check if both IR sensors detect white
  if (analogRead(irSensorLeftPin) < 700 && analogRead(irSensorRightPin) < 700) {
    // Both sensors detect white, execute movement pattern
    executeMovementPattern();
  } else {
    // If either sensor detects black, stop motors
    stopMotors();
    Serial.println("Black detected, stopping motors.");
  }
}

// Function to execute the movement pattern
void executeMovementPattern() {
  
  // Turn 90 degrees to the right using calibrated time
  turnRight(turn90DegreesTime);

  // Move forward for 6 seconds
  moveForward(3500);

  // Turn 90 degrees to the left using calibrated time
  turnLeft(turn90DegreesTime);

  // Move forward for 2 seconds
  moveForward(1000);

  delay(1000);
  // Turn 90 degrees to the left using calibrated time
  turnLeft(turn90DegreesTime);

  // Move forward for 6 seconds
  moveForward(3500);

  // Turn 90 degrees to the right using calibrated time
  turnRight(turn90DegreesTime);

  moveForward(1000);
}

// Function to move forward for a given duration
void moveForward(int duration) {
  leftForward.run(FORWARD);
  leftBack.run(FORWARD);
  rightBack.run(FORWARD);
  rightForward.run(FORWARD);

  // Check IR sensors while moving
  for (int i = 0; i < duration / 100; i++) {
    if (analogRead(irSensorLeftPin) >= 700 || analogRead(irSensorRightPin) >= 700) {
      stopMotors();
      Serial.println("Black detected during movement, stopping motors.");
      return;
    }
    delay(100);
  }
  stopMotors();
}

// Function to turn right for a given duration
void turnRight(int duration) {
  leftForward.setSpeed(turnSpeed );
  leftBack.setSpeed(turnSpeed );
  rightBack.setSpeed(turnSpeed );
  rightForward.setSpeed(turnSpeed );

  leftForward.run(FORWARD);
  leftBack.run(FORWARD);
  rightBack.run(BACKWARD);
  rightForward.run(BACKWARD);

  // Check IR sensors while turning
  for (int i = 0; i < duration / 100; i++) {
    if (analogRead(irSensorLeftPin) >= 700 || analogRead(irSensorRightPin) >= 700) {
      stopMotors();
      Serial.println("Black detected during turn, stopping motors.");
      return;
    }
    delay(100);
  }
  stopMotors();

  // Reset speed to base speed
  leftForward.setSpeed(baseSpeed);
  leftBack.setSpeed(baseSpeed);
  rightBack.setSpeed(baseSpeed);
  rightForward.setSpeed(baseSpeed);
}

// Function to turn left for a given duration
// Function to turn left for a given duration
void turnLeft(int duration) {
  // Increase the duration by 100 milliseconds
  duration += 80;

  leftForward.setSpeed(turnSpeed);
  leftBack.setSpeed(turnSpeed);
  rightBack.setSpeed(turnSpeed);
  rightForward.setSpeed(turnSpeed);

  leftForward.run(BACKWARD);
  leftBack.run(BACKWARD);
  rightBack.run(FORWARD);
  rightForward.run(FORWARD);

  // Check IR sensors while turning
  for (int i = 0; i < duration / 100; i++) {
    if (analogRead(irSensorLeftPin) >= 700 || analogRead(irSensorRightPin) >= 700) {
      stopMotors();
      Serial.println("Black detected during turn, stopping motors.");
      return;
    }
    delay(100);
  }
  stopMotors();

  // Reset speed to base speed
  leftForward.setSpeed(baseSpeed);
  leftBack.setSpeed(baseSpeed);
  rightBack.setSpeed(baseSpeed);
  rightForward.setSpeed(baseSpeed);
}

// Function to stop all motors
void stopMotors() {
  leftForward.run(RELEASE);
  leftBack.run(RELEASE);
  rightBack.run(RELEASE);
  rightForward.run(RELEASE);
  delay(100); // Short delay to ensure motors stop completely
}
