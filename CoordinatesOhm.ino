#include <AFMotor.h>

#define R_S A2  // IR sensor right
#define L_S A0  // IR sensor left

AF_DCMotor m1(1);  // Define motor 1 as LEFT FORWARD MOTOR
AF_DCMotor m2(2);  // Define motor 2 as LEFT BACK MOTOR
AF_DCMotor m3(3);  // Define motor 3 as RIGHT BACK MOTOR
AF_DCMotor m4(4);  // Define motor 4 as RIGHT FORWARD MOTOR

// Define speed variables
int forwardSpeed = 70; // Adjust the value to set the forward speed
int turnSpeed = 250;   // Adjust the value to set the turn speed
int threshold = 700;   // IR sensor threshold

// Define coordinate and orientation variables
int x = 0, y = 0;      // Initial coordinates
char orientation = 'N'; // Initial orientation: 'N' = North, 'E' = East, 'S' = South, 'W' = West

void setup() {
  // Set initial speed of the motor & stop
  m1.setSpeed(forwardSpeed);
  m1.run(RELEASE);
  m2.setSpeed(forwardSpeed);
  m2.run(RELEASE);
  m3.setSpeed(forwardSpeed);
  m3.run(RELEASE);
  m4.setSpeed(forwardSpeed);
  m4.run(RELEASE);
  
  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);

  Serial.begin(9600);  // Initialize serial communication

  delay(1000);
}

void loop() {
  if ((analogRead(R_S) < threshold) && (analogRead(L_S) < threshold)) {
    forward();
  }  // If both sensors detect below the threshold, move forward
  
  if ((analogRead(R_S) >= threshold) && (analogRead(L_S) < threshold)) {
    turnRight();
  }  // If right sensor detects above the threshold and left sensor detects below, turn right
  
  if ((analogRead(R_S) < threshold) && (analogRead(L_S) >= threshold)) {
    turnLeft();
  }  // If right sensor detects below the threshold and left sensor detects above, turn left
  
  if ((analogRead(R_S) >= threshold) && (analogRead(L_S) >= threshold)) {
    stopMotors();
  }  // If both sensors detect above the threshold, stop
}

void forward() {
  updateCoordinates();  // Update coordinates when moving forward
  m1.run(FORWARD);
  m2.run(FORWARD);
  m3.run(FORWARD);
  m4.run(FORWARD);
}

void turnRight() {
  m1.setSpeed(turnSpeed);
  m2.setSpeed(turnSpeed);
  m3.setSpeed(turnSpeed - 100); // Reduce speed for right wheels
  m4.setSpeed(turnSpeed - 100); // Reduce speed for right wheels
  
  m1.run(FORWARD);
  m2.run(FORWARD);
  m3.run(BACKWARD);
  m4.run(BACKWARD);

  updateOrientation('R');  // Update orientation when turning right

  m1.setSpeed(forwardSpeed);
  m2.setSpeed(forwardSpeed);
  m3.setSpeed(forwardSpeed);
  m4.setSpeed(forwardSpeed);
}

void turnLeft() {
  m1.setSpeed(turnSpeed - 100); // Reduce speed for left wheels
  m2.setSpeed(turnSpeed - 100); // Reduce speed for left wheels
  m3.setSpeed(turnSpeed); 
  m4.setSpeed(turnSpeed);
  
  m1.run(BACKWARD);
  m2.run(BACKWARD);
  m3.run(FORWARD);
  m4.run(FORWARD);

  updateOrientation('L');  // Update orientation when turning left

  m1.setSpeed(forwardSpeed);
  m2.setSpeed(forwardSpeed);
  m3.setSpeed(forwardSpeed);
  m4.setSpeed(forwardSpeed);
}

void stopMotors() {
  m1.run(RELEASE);
  m2.run(RELEASE);
  m3.run(RELEASE);
  m4.run(RELEASE);

  // Print the current coordinates and orientation when the robot stops
  Serial.print("Coordinates: (");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(") Orientation: ");
  Serial.println(orientation);
}

// Update the coordinates based on the current orientation
void updateCoordinates() {
  switch (orientation) {
    case 'N': y++; break;
    case 'E': x++; break;
    case 'S': y--; break;
    case 'W': x--; break;
  }
}

// Update the orientation based on the turn direction ('R' for right, 'L' for left)
void updateOrientation(char turnDirection) {
  if (turnDirection == 'R') {
    switch (orientation) {
      case 'N': orientation = 'E'; break;
      case 'E': orientation = 'S'; break;
      case 'S': orientation = 'W'; break;
      case 'W': orientation = 'N'; break;
    }
  } else if (turnDirection == 'L') {
    switch (orientation) {
      case 'N': orientation = 'W'; break;
      case 'E': orientation = 'N'; break;
      case 'S': orientation = 'E'; break;
      case 'W': orientation = 'S'; break;
    }
  }
}
