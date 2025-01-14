const unsigned int pinVrSensor = PB2;
const unsigned int pinSparkInput = PB1;
const unsigned int pinOutput =  PB0;
const unsigned int pinCoil14 =  PB3;
const unsigned int pinCoil23 =  PB4;
const unsigned int teethTotal = 62;
const unsigned int teethMissing = 2;

volatile unsigned long toothDetectedTime;
volatile unsigned long toothDetectedPrevTime;
volatile unsigned long pulseDuration;

volatile bool toothDetected = false;
volatile unsigned int toothCount;

volatile bool crankshaftPositionDetected = false;

void setup() {
  pinMode(pinVrSensor, INPUT);
  pinMode(pinSparkInput, INPUT);

  pinMode(pinOutput, OUTPUT);
  digitalWrite(pinOutput, 0);

  pinMode(pinCoil14, OUTPUT);
  pinMode(pinCoil23, OUTPUT);
  digitalWrite(pinCoil14, 1);
  digitalWrite(pinCoil23, 0);

  attachInterrupt(digitalPinToInterrupt(pinVrSensor), toothDetection, FALLING);
}

void loop() {
  if(toothDetected) {
    toothCount++;
    volatile unsigned long pulseDurationNew;
    pulseDurationNew = toothDetectedTime - toothDetectedPrevTime;

    //Detect index tooths
    if (pulseDurationNew > pulseDuration*teethMissing && pulseDurationNew < pulseDuration*(teethMissing+3)) {
      toothCount = 0;
      crankshaftPositionDetected = true;
      digitalWrite(pinOutput, !digitalRead(pinOutput));
    }

    pulseDuration = pulseDurationNew;

    if(crankshaftPositionDetected && toothCount < (teethTotal/2)) {
      digitalWrite(pinCoil14, 1);
      digitalWrite(pinCoil23, 0);
    }
    if(toothCount >= (teethTotal/2)) {
      digitalWrite(pinCoil14, 0);
      digitalWrite(pinCoil23, 1);
    }
    toothDetected = false;
  }
}

void toothDetection() {
  toothDetectedPrevTime = toothDetectedTime;
  toothDetectedTime = micros();
  toothDetected = true;
}