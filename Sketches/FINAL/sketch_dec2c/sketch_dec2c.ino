// CNC Shield Stepper Control for 4 Motors
// Pin definitions
const int StepX = 2;
const int DirX = 5;
const int StepY = 3;
const int DirY = 6;
const int StepZ = 4;
const int DirZ = 7;
const int StepA = 12;
const int DirA = 13;


int runCount = 0;          // Count how many steps have been completed
bool directionX = LOW;    // Current direction of motor X = LOW
bool directionA = HIGH;     // Current direction of motor A = HIGH
bool directionY = LOW;    // Current direction of motor i.e Y(LOW) ::towards unbox pillar

void setup() {
  // Set all motor pins as output
  pinMode(StepX, OUTPUT);
  pinMode(DirX, OUTPUT);
  pinMode(StepY, OUTPUT);
  pinMode(DirY, OUTPUT);
  pinMode(StepZ, OUTPUT);
  pinMode(DirZ, OUTPUT);
  pinMode(StepA, OUTPUT);
  pinMode(DirA, OUTPUT);

  // Initialize directions
  digitalWrite(DirX, directionX);
  digitalWrite(DirA, directionA);
  digitalWrite(DirY, directionY);
}

void loop() {
  // Perform 4000 steps (X and A rotate for 20 rotations assuming 200 steps per revolution)
  for (int i = 0; i < 3100; i++) {
    // Generate step pulses for X and A
    digitalWrite(StepX, HIGH);
    digitalWrite(StepA, LOW);
    delayMicroseconds(800); // Wait 1.5 ms
    digitalWrite(StepX, LOW);
    digitalWrite(StepA, HIGH                                                                                                                      );
    delayMicroseconds(800); // Wait 1.5 ms
  }

  // After 4000 steps, toggle directions for X and A
  directionX = !directionX;
  directionA = !directionA;

  // Update directions for X and A
  digitalWrite(DirX, directionX);
  digitalWrite(DirA, directionA);

  // Now rotate motor Y for 1000 steps (5 rotations)
  for (int i = 0; i < 1750; i++) {
    digitalWrite(StepY, HIGH);
    delayMicroseconds(1500); // Wait 1.5 ms
    digitalWrite(StepY, LOW);
    delayMicroseconds(1500); // Wait 1.5 ms
  }

  // Toggle direction for motor Y after its rotation
  directionY = !directionY;
  digitalWrite(DirY, directionY);

  // Increment the step cycle counter
  runCount++;

  // Optional: Stop the loop after a certain number of cycles (e.g., 500)
  if (runCount >= 100) {
    while (1); // Infinite loop to halt further execution
  }
}
