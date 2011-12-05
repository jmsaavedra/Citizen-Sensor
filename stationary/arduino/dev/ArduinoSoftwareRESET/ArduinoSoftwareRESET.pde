
//digitalPin 7 is connected to the RESET pin on Arduino
//NOTE: you CANNOT program the board while they are connected
//by default digitalPin 13 will blink upon reset, so stick an LED in there

int interval = 5000;
long int time = 0;

void setup(){
  digitalWrite(7, HIGH); //We need to set it HIGH immediately on boot
  pinMode(7,OUTPUT);     //We can declare it an output ONLY AFTER it's HIGH
                         // (( HACKHACKHACKHACK ))
  Serial.begin(9600);    //So you can watch the time printed
}


void loop(){

  time = millis();
  
  Serial.println(time);
  
  if(time > interval){
    Serial.println("RESET!");
    digitalWrite(7, LOW); //Pulling the RESET pin LOW triggers the reset.
  }
}

