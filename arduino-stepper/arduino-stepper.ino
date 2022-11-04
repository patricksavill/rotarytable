// Using the S8BYJ-48 Stepper motor, and UNL-2003 driver board
#define A 2
#define B 3
#define C 4
#define D 5
#define NUMBER_OF_STEPS_PER_REV 512
#define DRIVER_DELAY 2 // Found to be the minimum time a coarse step will work
const int MOTOR_COILS[]={8,12,4,6,2,3,1,9};

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
  int j = 0;
  while (j < 512){
    binaryFineStep(false);
    j ++;
  }
  while (j>0){
    binaryFineStep(true);
    j--;
  }
    while (j < 512){
    binaryCoarseStep(false);
    j ++;
  }
  while (j>0){
    binaryCoarseStep(true);
    j--;
  }

  // TODO hook up control buttons
  if (digitalRead(FORWARD_PIN) > 0){;
    digitalWrite(STATUS_LED, LOW);
  }
  else if (digitalRead(REVERSE_PIN) > 0){
    digitalWrite(STATUS_LED, HIGH);
  }

}

void writeBinaryStepper(int b){
  digitalWrite(A, (b >> 3) & 1);
  digitalWrite(B, (b >> 2) & 1);
  digitalWrite(C, (b >> 1) & 1);
  digitalWrite(D, (b >> 0) & 1);
}

void binaryFineStep(bool reverse){
  /* We can make the motor move with binary bitshifts
  * Uses up to two coils at a time.
  * 1000
  * 1100
  * 0100
  * 0110
  * etc etc
  * as defined in MOTOR_COILS
  */
  if (reverse){
    int i = 7;
    while (i >= 0){
      writeBinaryStepper(MOTOR_COILS[i]);
      delay(DRIVER_DELAY);
      i--;
    }
  }
  
  else{
    int i = 0;
    while (i < 8){
      writeBinaryStepper(MOTOR_COILS[i]);
      delay(DRIVER_DELAY);
      i++;
    }
  }
}

void binaryCoarseStep(bool reverse){
  // Coarser steps, use one coil at a time
  if(reverse){
    int i = 7;
    while (i >= 0){
      writeBinaryStepper(MOTOR_COILS[i]);
      delay(DRIVER_DELAY);
      i-=2;
    }
  }
  else{
    int i = 0;
    while (i < 8){
      writeBinaryStepper(MOTOR_COILS[i]);
      delay(DRIVER_DELAY);
      i+=2;
    }
  }
}
