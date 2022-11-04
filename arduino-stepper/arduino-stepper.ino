// Using the S8BYJ-48 Stepper motor, and UNL-2003 driver board
#define A 2
#define B 3
#define C 4
#define D 5
#define NUMBER_OF_STEPS_PER_REV 512
#define DRIVER_DELAY 2 // Found to be the minimum time a coarse step will work

#define REVERSE_PIN 8
#define FORWARD_PIN 9
#define STATUS_LED 10

void setup() {
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);

  pinMode(REVERSE_PIN, INPUT);
  pinMode(FORWARD_PIN, INPUT);
  pinMode(STATUS_LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(FORWARD_PIN) > 0){
    coarseStep(512);
    digitalWrite(STATUS_LED, LOW);
  }
  else if (digitalRead(REVERSE_PIN) > 0){
    digitalWrite(STATUS_LED, HIGH);
  }

}

void writeStepper(int a,int b,int c,int d){
  digitalWrite(A,a);
  digitalWrite(B,b);
  digitalWrite(C,c);
  digitalWrite(D,d);
}

void fineStep(int steps){
  while (steps > 0){
    writeStepper(1,0,0,0);
    delay(DRIVER_DELAY);
    writeStepper(1,1,0,0);
    delay(DRIVER_DELAY);
    writeStepper(0,1,0,0);
    delay(DRIVER_DELAY);
    writeStepper(0,1,1,0);
    delay(DRIVER_DELAY);
    writeStepper(0,0,1,0);
    delay(DRIVER_DELAY);
    writeStepper(0,0,1,1);
    delay(DRIVER_DELAY);
    writeStepper(0,0,0,1);
    delay(DRIVER_DELAY);
    writeStepper(1,0,0,1);
    delay(DRIVER_DELAY);
    steps--;
  }
}

void coarseStep(int steps){
  while(steps > 0){
    writeStepper(1,1,0,0);
    delay(DRIVER_DELAY);
    writeStepper(0,1,1,0);
    delay(DRIVER_DELAY);
    writeStepper(0,0,1,1);
    delay(DRIVER_DELAY);
    writeStepper(1,0,0,1);
    delay(DRIVER_DELAY);
    steps--;
  }
}