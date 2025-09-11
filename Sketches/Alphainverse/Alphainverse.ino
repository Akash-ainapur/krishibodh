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

int runCount = 0;  // Count how many times the motor has run

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
}

void loop() {
  if(runCount < 1) {  // Run 500 times
    // Set direction for motor A
    digitalWrite(DirA, HIGH);
    digitalWrite(DirY, HIGH);
    digitalWrite(DirX, LOW);  // HIGH for clockwise, LOW for anticlockwise

    // Generate 1000 step pulses for motor A
    for(int x = 0; x < 400; x++) {
      digitalWrite(StepA, HIGH);
      digitalWrite(StepY, HIGH);
      digitalWrite(StepX, HIGH);  // Step signal HIGH
      delayMicroseconds(1500);     // Wait 600 microseconds
      digitalWrite(StepA, LOW);
      digitalWrite(StepY, LOW);
      digitalWrite(StepX, LOW);   // Step signal LOW
      delayMicroseconds(1500);     // Wait 600 microseconds
    }
    
      // 1-second pause after 1000 steps
    runCount++;   // Increment the run count
  // } else if(runCount ==2 ) {
  //   for(int x=0; x< 600; x++)
    
  //   delayMicroseconds(1500)
  //   digitalWrite(StepY, LOW);

    // Stop the motor after 500 runs by halting the program
      // Infinite loop to halt further execution
  }
  else{
    while(1);
  }
}
