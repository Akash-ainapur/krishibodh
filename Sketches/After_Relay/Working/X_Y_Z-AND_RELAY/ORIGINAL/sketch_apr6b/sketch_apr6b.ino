// CNC Shield Stepper Control for 4 Motors with Z-axis
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
int xi = 0, yi = 0, zi = 0; 
int x, y, z;
int x0, y0, z0;

bool directionX = HIGH;  // Initial direction of motor X
bool directionY = LOW;   // Initial direction of motor Y
bool directionA = LOW;   // Initial direction of motor A
bool directionZ = HIGH;  // Initial direction of motor Z

void setup() {
  Serial.begin(9600);
  Serial.println("Stepper Control Initialized");
  Serial.println("Enter coordinates (x, y, z):");

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
  digitalWrite(DirZ, directionZ);


  
}

void loop() {
  if (Serial.available() > 0) {
    // Read X coordinate
    Serial.print("Enter X coordinate: ");
    while (Serial.available() == 0) {}
    x = Serial.parseInt();

    // Read Y coordinate
    Serial.print("Enter Y coordinate: ");
    while (Serial.available() == 0) {}
    y = Serial.parseInt();
    
    // Read Z coordinate
    Serial.print("Enter Z coordinate: ");
    while (Serial.available() == 0) {}
    z = Serial.parseInt();

    // Feedback for user
    Serial.print("Moving to: ");
    Serial.print(x); Serial.print(", ");
    Serial.print(y); Serial.print(", ");
    Serial.println(z);

    // Calculate relative movement
    x0 = x; y0 = y; z0 = z;
    x -= xi; y -= yi; z -= zi;
    xi = x0; yi = y0; zi = z0;

    // Update directions
    directionX = (x < 0) ? HIGH : LOW;
    directionA = (x < 0) ? LOW : HIGH;
    directionY = (y < 0) ? HIGH : LOW;
    directionZ = (z < 0) ? LOW : HIGH;

    x = abs(x);
    y = abs(y);
    z = abs(z);

    digitalWrite(DirX, directionX);
    digitalWrite(DirY, directionY);
    digitalWrite(DirA, directionA);
    digitalWrite(DirZ, directionZ);

    // Move motors for X and Y
    for (int i = 0; i < x; i++) {
      digitalWrite(StepX, HIGH);
      digitalWrite(StepA, LOW);
      delayMicroseconds(1500);
      digitalWrite(StepX, LOW);
      digitalWrite(StepA, HIGH);
      delayMicroseconds(1500);
    }

    for (int i = 0; i < y; i++) {
      digitalWrite(StepY, HIGH);
      delayMicroseconds(1000);
      digitalWrite(StepY, LOW);
      delayMicroseconds(1000);
    }

    // Move Z after X and Y movement is complete
    for (int i = 0; i < z; i++) {
      digitalWrite(StepZ, HIGH);
      delayMicroseconds(550);
      digitalWrite(StepZ, LOW);
      delayMicroseconds(550);
    }


    




    // Notify completion
    Serial.println("Movement complete.");
    Serial.println("Enter new coordinates (x, y, z):");
  }


    
}
