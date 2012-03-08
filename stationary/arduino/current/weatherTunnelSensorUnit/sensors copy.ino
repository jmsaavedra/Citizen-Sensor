
//the number of readings to be taken for averaging
//analog sensors, each time they are called
#define NUMREADINGS 25
//debounce time
#define DEBOUNCETIME 10

//---------- temperature ----------
int getTemperature(){
  byte tmp102comm = Wire.requestFrom(72,2); 
  if (tmp102comm == 2) {
    byte msb = Wire.receive(); /* Whole degrees */
    byte lsb = Wire.receive(); /* Fractional degrees */
    int val = ((msb) << 4);   /* MSB */
    val |= (lsb >> 4);    /* LSB */
    float celsius = (val*0.0625)-5;
    float farenheit = (celsius*1.8) + 32;
  /*Serial.print("C: ");
    Serial.println(celsius);
    Serial.print("F: ");
    Serial.println(farenheit);
    Serial.println();
    delay(1000);*/ 
    return int(celsius);
  }
}

//---------- light ----------
int getLight(){
  int lightVal = 0;
  lightVal = analogRead(2);
  delay(DEBOUNCETIME);
  return lightVal;    
}

//---------- humidity ----------
int getHumidity(){
  int humidityVal = 0;
  for(int i=0; i<NUMREADINGS; i++){
    int tempRead = analogRead(5);
    humidityVal = humidityVal + tempRead;
    delay(DEBOUNCETIME);
  }
  humidityVal = humidityVal/NUMREADINGS;    
  return humidityVal;   
}

//---------- ambient noise ----------
int getNoise(){
  int noiseVal = 0;
  int ctr = 0;
  int nAverage = 0;
  for(int i = 0; i< NUMREADINGS-10; i++){
    int tempRead = analogRead(A0);
    if(tempRead > 0){
      tempRead = abs(tempRead - 300);
      noiseVal = noiseVal + tempRead;
      ctr++;
      delay(DEBOUNCETIME);
    }
  }
  noiseVal = noiseVal/ctr;
  return noiseVal;
}

//---------- carbon dioxide ----------
int getCO2(){
  int CO2Val = 0;
  for(int i=0; i<NUMREADINGS; i++){
    int tempRead = analogRead(14);
    CO2Val = CO2Val + tempRead;
    delay(DEBOUNCETIME);
  }
  CO2Val = CO2Val/NUMREADINGS;
  CO2Val = 1000 - CO2Val; // sensor returns lower value the more CO2 is detected
  return CO2Val;    
}

//---------- air quality ----------
int getAirQual(){
  int AirQualVal = 0;
  for(int i=0; i<NUMREADINGS; i++){
    int tempRead = analogRead(10);
    AirQualVal = AirQualVal + tempRead;
    delay(DEBOUNCETIME);
  }
  AirQualVal = AirQualVal/NUMREADINGS;  
  return AirQualVal;    
}

//---------- carbon monoxide ----------
int getCO(){
  pinMode(14, OUTPUT);

  MQ7.CoPwrCycler();  

  if(MQ7.CurrentState() == LOW){   //we are at 1.4v, read sensor data!
    int CoData = 0;
    for(int i=0; i<NUMREADINGS; i++){
      int tempRead = analogRead(8);
      CoData = CoData + tempRead;
      delay(DEBOUNCETIME);
    }
    CoData = CoData/NUMREADINGS; 
    CoPrev = CoData;
    return CoData;
    //Serial.println(CoData);
  }
  else{                            //sensor is at 5v, heating time
    //during heating time we cannot read new data, return previous reading:
    return CoPrev;
  }      
}

