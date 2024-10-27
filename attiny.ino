#include "PinChangeInterrupt.h"

const unsigned int pinVrSensor = PB2;
const unsigned int pinSparkInput = PB1;
const unsigned int pinOutput =  PB0;
const unsigned int pinCoil14 =  PB3;
const unsigned int pinCoil23 =  PB4;
const unsigned int toothTotal = 62;
const unsigned int toothMissing = 2;

volatile unsigned long toothDetectedTime;
volatile unsigned long toothDetectedPrevTime;
volatile unsigned long pulseDuration;

volatile bool toothDetected = false;
volatile unsigned int toothCount;

volatile bool crankshaftPositionDetected = false;
volatile bool crankshaftPositionBelow180 = true;

void setup() {
  pinMode(pinVrSensor, INPUT_PULLUP);
  pinMode(pinSparkInput, INPUT_PULLUP);

  pinMode(pinCoil14, OUTPUT);
  pinMode(pinCoil23, OUTPUT);
  digitalWrite(pinCoil14, 0);
  digitalWrite(pinCoil23, 0);

  attachInterrupt(digitalPinToInterrupt(pinVrSensor), toothDetection, RISING);
  attachPCINT(digitalPinToPCINT(pinSparkInput), spark, CHANGE);
}

void loop() {
  if(toothDetected) {
    volatile unsigned long pulseDurationNew;
    pulseDurationNew = toothDetectedTime - toothDetectedPrevTime;

    //Detect index tooths
    if (pulseDurationNew > pulseDuration*toothMissing && pulseDurationNew < pulseDuration*(toothMissing+2)) {
      toothCount = 0;
      crankshaftPositionDetected = true;
    }

    pulseDuration = pulseDurationNew;

    if(crankshaftPositionDetected) {
      toothCount++;
      if(toothCount < (toothTotal/2)) {
        crankshaftPositionBelow180 = true;
        digitalWrite(pinOutput, 1);
      }
      else {
        crankshaftPositionBelow180 = false;
        digitalWrite(pinOutput, 0);
      }
    }
    toothDetected = false;
  }
}

void toothDetection() {
  toothDetectedPrevTime = toothDetectedTime;
  toothDetectedTime = micros();
  toothDetected = true;
}

void spark() {
  if(digitalRead(pinSparkInput)) {
    if(crankshaftPositionBelow180) {
      digitalWrite(pinCoil14, 1);
    }
    else {
      digitalWrite(pinCoil23, 1);
    }
  }
  else {
    digitalWrite(pinCoil14, 0);
    digitalWrite(pinCoil23, 0);
  }
}