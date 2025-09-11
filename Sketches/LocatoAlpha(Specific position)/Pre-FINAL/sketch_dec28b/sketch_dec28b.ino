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

// Current and previous coordinates
int xi = 0;
int yi = 0;
int x, y;
int x0, y0;

bool directionX = LOW;    // Initial direction of motor X
bool directionY = LOW;    // Initial direction of motor Y
bool directionA = HIGH;   // Initial direction of motor A

void setup() {
  Serial.begin(9600);
  Serial.println("Stepper Control Initialized");
  Serial.println("Enter coordinates (x, y):");

  pinMode(StepX, OUTPUT);
  pinMode(DirX, OUTPUT);
  pinMode(StepY, OUTPUT);
  pinMode(DirY, OUTPUT);
  pinMode(StepZ, OUTPUT);
  pinMode(DirZ, OUTPUT);
  pinMode(StepA, OUTPUT);
  pinMode(DirA, OUTPUT);

  digitalWrite(DirX, directionX);
  digitalWrite(DirY, directionY);
  digitalWrite(DirA, directionA);
}

void loop() {
  if (Serial.available() > 0) {
    // Read X coordinate
    Serial.print("Enter X coordinate: ");
    while (Serial.available() == 0) {} // Wait for input
    x = Serial.parseInt();

    // Read Y coordinate
    Serial.print("Enter Y coordinate: ");
    while (Serial.available() == 0) {} // Wait for input
    y = Serial.parseInt();

    // Feedback for user
    Serial.print("Moving to: ");
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);

    // Calculate relative movement
    x0 = x;
    y0 = y;
    x -= xi;
    y -= yi;
    xi = x0;
    yi = y0;

    // Update directions
    if (x < 0) {
      directionX = HIGH;
      directionA = LOW;
      x = -x;
    }
    else{
      directionX = LOW;
      directionA = HIGH;

    }
    if (y < 0) {
      directionY = HIGH;
      y = -y;
    }
    else{
      directionY = LOW;


    }

    digitalWrite(DirX, directionX);
    digitalWrite(DirY, directionY);
    digitalWrite(DirA, directionA);

    // Move motors for X and Y
    for (int i = 0; i < x; i++) {
      digitalWrite(StepX, HIGH);
      digitalWrite(StepA, LOW);
      delayMicroseconds(1000);
      digitalWrite(StepX, LOW);
      digitalWrite(StepA, HIGH);
      delayMicroseconds(1000);
    }

    for (int i = 0; i < y; i++) {
      digitalWrite(StepY, HIGH);
      delayMicroseconds(800);
      digitalWrite(StepY, LOW);
      delayMicroseconds(800);
    }

    // Notify completion
    Serial.println("Movement complete.");
    Serial.println("Enter new coordinates (x, y):");
  }
}
