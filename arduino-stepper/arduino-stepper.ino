// Using the S8BYJ-48 Stepper motor, and UNL-2003 driver board
#define A 2
#define B 3
#define C 4
#define D 5
#define NUMBER_OF_STEPS_PER_REV 512
#define STEP_CHUNK_SIZE 8 // Adjust for a greater/smaller turn per button press
#define DRIVER_DELAY 2 // Found to be the minimum time a coarse step will work
const int MOTOR_COILS[]={8,12,4,6,2,3,1,9};

#define FORWARD_PIN 8
#define RESET_PIN 9
#define STATUS_LED 10

int gStepsSoFar = 0;
int gStepsPerPress = NUMBER_OF_STEPS_PER_REV / STEP_CHUNK_SIZE;

void setup() {
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);

  pinMode(FORWARD_PIN, INPUT);
  pinMode(RESET_PIN, INPUT);
  pinMode(STATUS_LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
 
  if (digitalRead(RESET_PIN) > 0){
    // Reset to the position we started in
    digitalWrite(STATUS_LED, HIGH);
    while (gStepsSoFar >= 0){
      binaryCoarseStep(true);
      gStepsSoFar --;
    }
    digitalWrite(STATUS_LED, LOW);
  }
  else if (digitalRead(FORWARD_PIN) > 0){
    digitalWrite(STATUS_LED, HIGH);
    // Step around in grouped increments
    for (int i = 0; i<gStepsPerPress; i++){
      binaryCoarseStep(false);
    }
    gStepsSoFar+=gStepsPerPress;
    if(gStepsSoFar > NUMBER_OF_STEPS_PER_REV){
      // We have performed a full rotation, subtract this
      gStepsSoFar = gStepsSoFar - NUMBER_OF_STEPS_PER_REV;
    }
    digitalWrite(STATUS_LED, LOW);
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
    int i = 1;
    while (i < 8){
      writeBinaryStepper(MOTOR_COILS[i]);
      delay(DRIVER_DELAY);
      i+=2;
    }
  }
}
