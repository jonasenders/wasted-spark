
  const unsigned int pinVr = PB1;
  const unsigned int pinSpark = PB2;

  const unsigned int toothTotal = 62;
  const unsigned int toothMissing = 2;

  int unsigned beat = 0;

  const unsigned int rpm = 10;
  
void setup() {


  pinMode(pinVr, OUTPUT);
  pinMode(pinSpark, OUTPUT);
  digitalWrite(pinVr, 1);
  digitalWrite(pinSpark, 0);
}

void loop() {
  if ( (beat % 2) == 0) {
    digitalWrite(pinVr, 1);
  }
  else {
    if(beat < ((toothTotal-toothMissing)*2)) {
      digitalWrite(pinVr, 0);
    }
  }
  if(beat == 18) {
    digitalWrite(pinSpark, 1);
  }
  if(beat == 26) {
    digitalWrite(pinSpark, 0);
  }
  if(beat == 78) {
    digitalWrite(pinSpark, 1);
  }
  if(beat == 86) {
    digitalWrite(pinSpark, 0);
  }
  delay(((60*1000UL)/(rpm*toothTotal*2)));
  beat++;
  if(beat == toothTotal*2) {
    beat = 0;
  }
}