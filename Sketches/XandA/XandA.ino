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

const int stepsPerRevolution = 200; // Adjust based on your motor's specification
const int microsteps = 5;         // Adjust based on your driver setting (1, 2, 4, 8, 16, etc.)
const int totalSteps = stepsPerRevolution * microsteps;

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

  // Set direction for motor (e.g., clockwise)
  digitalWrite(DirX, HIGH);
  digitalWrite(DirA, LOW);
}

void loop() {
  // Perform one full rotation for motor X
  for (int step = 0; step < totalSteps; step++) {
    digitalWrite(StepX, HIGH);
    digitalWrite(StepA, HIGH);  // Step signal HIGH
    delayMicroseconds(500);     // Adjust speed (500µs = ~1ms for one full step)
    digitalWrite(StepX, LOW);
    digitalWrite(StepX, LOW);   // Step signal LOW
    delayMicroseconds(500);
  }

  // Stop execution after one rotation
  while (true) {
    // Infinite loop to halt further execution
  }
}
