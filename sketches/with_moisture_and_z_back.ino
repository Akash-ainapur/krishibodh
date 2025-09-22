#include <Arduino.h>

// Stepper Motor Pin Definitions (CNC Shield)
const int StepX = 2;
const int DirX = 5;
const int StepY = 3;
const int DirY = 6;
const int StepZ = 4;
const int DirZ = 7;
const int StepA = 12;
const int DirA = 13;
const int RelayPin = 10;
const int SoilPin = A1; // Soil moisture sensor analog pin

// Moisture threshold (adjust based on sensor readings)
const int moistureThreshold = 500; // Below this = dry

// Coordinates
int xi = 0, yi = 0, zi = 0;
int x, y, z;
int x0, y0, z0;

// Motor directions
bool directionX = HIGH;
bool directionY = LOW;
bool directionA = LOW;
bool directionZ = HIGH;

void setup()
{
    Serial.begin(9600);
    Serial.println("STATUS: Stepper Control Initialized");

    pinMode(StepX, OUTPUT);
    pinMode(DirX, OUTPUT);
    pinMode(StepY, OUTPUT);
    pinMode(DirY, OUTPUT);
    pinMode(StepZ, OUTPUT);
    pinMode(DirZ, OUTPUT);
    pinMode(StepA, OUTPUT);
    pinMode(DirA, OUTPUT);
    pinMode(RelayPin, OUTPUT);
    pinMode(SoilPin, INPUT);

    digitalWrite(DirX, directionX);
    digitalWrite(DirY, directionY);
    digitalWrite(DirA, directionA);
    digitalWrite(DirZ, directionZ);
    digitalWrite(RelayPin, LOW); // Relay OFF initially
}

void loop()
{
    if (Serial.available() > 0)
    {
        String xStr = Serial.readStringUntil('\n');
        x = xStr.toInt();
        String yStr = Serial.readStringUntil('\n');
        y = yStr.toInt();
        String zStr = Serial.readStringUntil('\n');
        z = zStr.toInt();

        Serial.print("STATUS: Received Target Coordinates: ");
        Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.println(z);

        x0 = x; y0 = y; z0 = z;
        x -= xi; y -= yi; z -= zi;
        xi = x0; yi = y0; zi = z0;

        directionX = (x < 0) ? HIGH : LOW;
        directionA = (x < 0) ? LOW : HIGH;
        directionY = (y < 0) ? HIGH : LOW;
        directionZ = (z < 0) ? LOW : HIGH;

        x = abs(x); y = abs(y); z = abs(z);

        digitalWrite(DirX, directionX);
        digitalWrite(DirY, directionY);
        digitalWrite(DirA, directionA);
        digitalWrite(DirZ, directionZ);

        Serial.println("STATUS: Moving X/A axes...");
        for (int i = 0; i < x; i++)
        {
            digitalWrite(StepX, HIGH);
            digitalWrite(StepA, LOW);
            delayMicroseconds(1500);
            digitalWrite(StepX, LOW);
            digitalWrite(StepA, HIGH);
            delayMicroseconds(1500);
        }
        Serial.println("STATUS: X/A movement done.");

        Serial.println("STATUS: Moving Y axis...");
        for (int i = 0; i < y; i++)
        {
            digitalWrite(StepY, HIGH);
            delayMicroseconds(1000);
            digitalWrite(StepY, LOW);
            delayMicroseconds(1000);
        }
        Serial.println("STATUS: Y movement done.");

        Serial.println("STATUS: Moving Z axis...");
        for (int i = 0; i < z; i++)
        {
            digitalWrite(StepZ, HIGH);
            delayMicroseconds(500);
            digitalWrite(StepZ, LOW);
            delayMicroseconds(500);
        }
        Serial.println("STATUS: Z movement done.");

        delay(2000);

        if (x0 != 0 || y0 != 0 || z0 != 0)
        {
            int moistureValue = analogRead(SoilPin);
            Serial.print("STATUS: Soil Moisture Value: ");
            Serial.println(moistureValue);

            if (moistureValue > moistureThreshold)
            {
                Serial.println("STATUS: Soil is DRY → Activating relay...");
                digitalWrite(RelayPin, HIGH);
                delay(5000);
                digitalWrite(RelayPin, LOW);
                Serial.println("STATUS: Relay sequence finished.");
            }
            else
            {
                Serial.println("STATUS: Soil is WET → Skipping relay.");
            }

            delay(1000);

            // Move Z-axis back to 0
            if (zi != 0)
            {
                Serial.println("STATUS: Returning Z axis to 0...");
                digitalWrite(DirZ, LOW); // LOW = toward 0
                for (int i = 0; i < zi; i++)
                {
                    digitalWrite(StepZ, HIGH);
                    delayMicroseconds(500);
                    digitalWrite(StepZ, LOW);
                    delayMicroseconds(500);
                }
                zi = 0; // Update current Z position
                Serial.println("STATUS: Z axis returned to 0.");
            }
        }
        else
        {
            Serial.println("STATUS: Target is home, skipping relay.");
        }

        Serial.println("STATUS: Movement sequence complete.");
        Serial.println("STATUS: Waiting for next coordinates...");
    }
}
