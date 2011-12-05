

//RESET function is at the bottom

void setupLedFlash(){
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  for(int i=0; i<3; i++){
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    delay(150);
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    delay(150); 
  }
 digitalWrite(12, LOW); 
}

void breatheGreenLED(){
  for(int i=0; i<200; i++){
    analogWrite(13, i);
    delay(8);
  }
  for(int i=199; i>0; i--){
    analogWrite(13, i);
    delay(8);
  }
  analogWrite(13, 0);
}


void flashRedLED(){
  pinMode(12, OUTPUT);
  for(int i=0; i<12; i++){
    digitalWrite(12, HIGH);
    delay(250);
    digitalWrite(12, LOW);
    delay(250); 
  }
}

void flashGreenLED(){
  pinMode(13, OUTPUT);
  for(int i=0; i<3; i++){
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
    delay(250); 
  }
}

void softwareReset(){
  //this function will pull the RESET PIN to LOW, causing the entire Arduino
  //and Ethernet Shield to reset themselves. 
  Serial.println("Resetting board");
  lcdResetNow();
  digitalWrite(softwareResetPin, LOW);
}

