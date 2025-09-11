#include <Arduino.h>

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
const int RelayPin = 10;

// Current and previous coordinates
int xi = 0, yi = 0, zi = 0;
int x, y, z;
int x0, y0, z0;

bool directionX = HIGH; // Initial direction of motor X
bool directionY = LOW;  // Initial direction of motor Y
bool directionA = LOW;  // Initial direction of motor A
bool directionZ = HIGH; // Initial direction of motor Z

void setup()
{
  Serial.begin(9600);
  Serial.println("STATUS: Stepper Control Initialized"); // Added STATUS prefix
  // Serial.println("Enter coordinates (x, y, z):"); // Commented out less relevant message

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

  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);
}

void loop()
{
  if (Serial.available() > 0)
  {
    // Read X coordinate
    // Serial.println("Waiting for X..."); // Commented out
    String xStr = Serial.readStringUntil('\n');
    x = xStr.toInt();
    // Serial.print("Received X string: '"); // Commented out
    // Serial.print(xStr); // Commented out
    // Serial.println("'"); // Commented out
    // Serial.print("Parsed X: "); // Commented out
    // Serial.println(x); // Commented out

    // Read Y coordinate
    // Serial.println("Waiting for Y..."); // Commented out
    String yStr = Serial.readStringUntil('\n');
    y = yStr.toInt();
    // Serial.print("Received Y string: '"); // Commented out
    // Serial.print(yStr); // Commented out
    // Serial.println("'"); // Commented out
    // Serial.print("Parsed Y: "); // Commented out
    // Serial.println(y); // Commented out

    // Read Z coordinate
    // Serial.println("Waiting for Z..."); // Commented out
    String zStr = Serial.readStringUntil('\n');
    z = zStr.toInt();
    // Serial.print("Received Z string: '"); // Commented out
    // Serial.print(zStr); // Commented out
    // Serial.println("'"); // Commented out
    // Serial.print("Parsed Z: "); // Commented out
    // Serial.println(z); // Commented out

    // Feedback for user
    Serial.print("STATUS: Received Target Coordinates: "); // Added STATUS prefix
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.println(z);

    // Serial.print("Current Position (Before Move): "); // Commented out internal state
    // Serial.print(xi); // Commented out
    // Serial.print(", "); // Commented out
    // Serial.print(yi); // Commented out
    // Serial.print(", "); // Commented out
    // Serial.println(zi); // Commented out

    // Calculate relative movement
    x0 = x;
    y0 = y;
    z0 = z; // Store target coordinates
    x -= xi;
    y -= yi;
    z -= zi; // Calculate difference (steps to move)
    xi = x0;
    yi = y0;
    zi = z0; // Update current position to the target

    // Serial.print("Calculated Relative Movement: "); // Commented out internal calculation
    // Serial.print(x); // Commented out
    // Serial.print(", "); // Commented out
    // Serial.print(y); // Commented out
    // Serial.print(", "); // Commented out
    // Serial.println(z); // Commented out

    // Update directions based on relative movement
    directionX = (x < 0) ? HIGH : LOW;
    directionA = (x < 0) ? LOW : HIGH; // Assuming A mirrors X
    directionY = (y < 0) ? HIGH : LOW;
    directionZ = (z < 0) ? LOW : HIGH; // Check if Z direction needs inversion based on your setup

    // Serial.print("Direction Flags (X, Y, Z, A): "); // Commented out internal state
    // Serial.print(directionX == HIGH ? "HIGH" : "LOW"); // Commented out
    // Serial.print(", "); // Commented out
    // Serial.print(directionY == HIGH ? "HIGH" : "LOW"); // Commented out
    // Serial.print(", "); // Commented out
    // Serial.print(directionZ == HIGH ? "HIGH" : "LOW"); // Commented out
    // Serial.print(", "); // Commented out
    // Serial.println(directionA == HIGH ? "HIGH" : "LOW"); // Commented out

    // Get absolute number of steps
    x = abs(x);
    y = abs(y);
    z = abs(z);

    // Serial.print("Absolute Steps (X, Y, Z): "); // Commented out internal calculation
    // Serial.print(x); // Commented out
    // Serial.print(", "); // Commented out
    // Serial.print(y); // Commented out
    // Serial.print(", "); // Commented out
    // Serial.println(z); // Commented out

    digitalWrite(DirX, directionX);
    digitalWrite(DirY, directionY);
    digitalWrite(DirA, directionA);
    digitalWrite(DirZ, directionZ);

    // Move motors for X and A (assuming they move together)
    Serial.println("STATUS: Moving X/A axes..."); // Added STATUS prefix
    for (int i = 0; i < x; i++)
    {
      // Consider if StepA needs different logic or timing
      digitalWrite(StepX, HIGH);
      digitalWrite(StepA, LOW); // Check if A should step differently
      delayMicroseconds(1500);
      digitalWrite(StepX, LOW);
      digitalWrite(StepA, HIGH); // Check if A should step differently
      delayMicroseconds(1500);
    }
    Serial.println("STATUS: X/A movement done."); // Added STATUS prefix

    Serial.println("STATUS: Moving Y axis..."); // Added STATUS prefix
    for (int i = 0; i < y; i++)
    {
      digitalWrite(StepY, HIGH);
      delayMicroseconds(1000);
      digitalWrite(StepY, LOW);
      delayMicroseconds(1000);
    }
    Serial.println("STATUS: Y movement done."); // Added STATUS prefix

    // Move Z after X and Y movement is complete
    Serial.println("STATUS: Moving Z axis..."); // Added STATUS prefix
    for (int i = 0; i < z; i++)
    {
      digitalWrite(StepZ, HIGH);
      delayMicroseconds(550);
      digitalWrite(StepZ, LOW);
      delayMicroseconds(550);
    }
    Serial.println("STATUS: Z movement done."); // Added STATUS prefix

    delay(2000);

    // Only activate relay if the target coordinates are not the home position (0,0,0)
    // Note: x0, y0, z0 hold the target coordinates received via Serial
    if (x0 != 0 || y0 != 0 || z0 != 0)
    {
      Serial.println("STATUS: Activating relay...");      // Added STATUS prefix
      digitalWrite(RelayPin, HIGH);                       // Relay ON
      delay(5000);                                        // Water for 5 seconds
      digitalWrite(RelayPin, LOW);                        // Relay OFF
      Serial.println("STATUS: Relay sequence finished."); // Added STATUS prefix
      delay(1000);                                        // Short delay after relay off
    }
    else
    {
      Serial.println("STATUS: Target is home, skipping relay."); // Added STATUS prefix
    }

    // Notify completion
    Serial.println("STATUS: Movement sequence complete.");     // Added STATUS prefix
    Serial.println("STATUS: Waiting for next coordinates..."); // Added STATUS prefix
  }
}
