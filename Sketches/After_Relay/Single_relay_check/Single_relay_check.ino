const int RelayPin = 10; // D8 == SpnEn

void setup() {
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW); // LOW = OFF on some relays
}

void loop() {
  digitalWrite(RelayPin, LOW); // Relay ON
  delay(3000);
  digitalWrite(RelayPin, HIGH);  // Relay OFF
  delay(3000);
}
