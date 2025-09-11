#include <Arduino.h>

// CNC Shield Stepper Control for 4 Motors with Z-axis
const int StepX = 2;
const int DirX = 5;
const int StepY = 3;
const int DirY = 6;
const int StepZ = 4;
const int DirZ = 7;
const int StepA = 12;
const int DirA = 13;
const int RelayPin = 10;

int xi = 0, yi = 0, zi = 0;
int x, y, z;
int x0, y0, z0;
bool directionX = HIGH;
bool directionY = LOW;
bool directionA = LOW;
bool directionZ = HIGH;

void setup() {
  Serial.begin(9600);
  Serial.println("STATUS: Stepper Control Initialized");
  pinMode(StepX, OUTPUT); pinMode(DirX, OUTPUT);
  pinMode(StepY, OUTPUT); pinMode(DirY, OUTPUT);
  pinMode(StepZ, OUTPUT); pinMode(DirZ, OUTPUT);
  pinMode(StepA, OUTPUT); pinMode(DirA, OUTPUT);
  digitalWrite(DirX, directionX);
  digitalWrite(DirY, directionY);
  digitalWrite(DirA, directionA);
  digitalWrite(DirZ, directionZ);
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    String xStr = Serial.readStringUntil('\n');
    x = xStr.toInt();
    String yStr = Serial.readStringUntil('\n');
    y = yStr.toInt();
    String zStr = Serial.readStringUntil('\n');
    z = zStr.toInt();
    Serial.print("STATUS: Received Target Coordinates: ");
    Serial.print(x); Serial.print(", ");
    Serial.print(y); Serial.print(", ");
    Serial.println(z);

    x0 = x; y0 = y; z0 = z;
    x -= xi; y -= yi; z -= zi;
    xi = x0; yi = y0; zi = z0;

    directionX = (x < 0) ? HIGH : LOW;
    directionA = (x < 0) ? LOW : HIGH;
    directionY = (y < 0) ? HIGH : LOW;
    directionZ = (z < 0) ? LOW : HIGH;

    long dx = abs(x);
    long dy = abs(y);
    long dz = abs(z);
    digitalWrite(DirX, directionX);
    digitalWrite(DirA, directionA);
    digitalWrite(DirY, directionY);
    digitalWrite(DirZ, directionZ);

    long steps = dx;
    if (dy > steps) steps = dy;
    if (dz > steps) steps = dz;
    if (steps > 0) {
      Serial.println("STATUS: Moving X/Y/Z axes in parallel...");
      long errX = 0, errY = 0, errZ = 0;
      for (long i = 0; i < steps; i++) {
        errX += dx;
        errY += dy;
        errZ += dz;
        if (errX >= steps) {
          digitalWrite(StepX, HIGH);
          digitalWrite(StepA, HIGH);
          delayMicroseconds(1500);
          digitalWrite(StepX, LOW);
          digitalWrite(StepA, LOW);
          errX -= steps;
        }
        if (errY >= steps) {
          digitalWrite(StepY, HIGH);
          delayMicroseconds(1000);
          digitalWrite(StepY, LOW);
          errY -= steps;
        }
        if (errZ >= steps) {
          digitalWrite(StepZ, HIGH);
          delayMicroseconds(550);
          digitalWrite(StepZ, LOW);
          errZ -= steps;
        }
        // adjust this delay if needed for overall feedrate
        // here minimal pause to allow next iteration; can be reduced or replaced by micros() logic
      }
      Serial.println("STATUS: Parallel movement done.");
    } else {
      Serial.println("STATUS: No movement needed.");
    }

    delay(2000);
    if (x0 != 0 || y0 != 0 || z0 != 0) {
      Serial.println("STATUS: Activating relay...");
      digitalWrite(RelayPin, HIGH);
      delay(3000);
      digitalWrite(RelayPin, LOW);
      Serial.println("STATUS: Relay sequence finished.");
      delay(1000);
    } else {
      Serial.println("STATUS: Target is home, skipping relay.");
    }
    Serial.println("STATUS: Movement sequence complete.");
    Serial.println("STATUS: Waiting for next coordinates...");
  }
}
