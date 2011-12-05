

void lcdSetup(){
  int scrollDelay = 400; //400 looks good
  lcd.begin(16,2);
  String IntroMsg = " Welcome to the Weather Tunnel Project!     ";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hello "+institutionName +"!");
  delay(2000);
  for(int IntroMsgIndex = 0; IntroMsgIndex < institutionName.length(); IntroMsgIndex++){
    lcd.scrollDisplayLeft();
    delay(scrollDelay);
  }
  delay(100);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(IntroMsg);
  delay(1000);
  for(int IntroMsgIndex = 0; IntroMsgIndex < 25; IntroMsgIndex++){
    //if(IntroMsgIndex<38) lcd.print(IntroMsg.charAt(IntroMsgIndex));
    lcd.scrollDisplayLeft();
    delay(scrollDelay);
  }
  lcd.noAutoscroll();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("initializing");
  lcdStatusBar();
  lcdStatusBar();
  lcdStatusBar();
}

void lcdUpdater(int sensor, int reading){

  if(sensor == 0){
    lcd.setCursor(0,1);
    lcd.print("temp:          ");
  }  
  else if(sensor == 1){
    lcd.setCursor(0,1);
    lcd.print("light:          ");
  }
  else if(sensor == 2){
    lcd.setCursor(0,1);
    lcd.print("humidity:       ");
  }
  else if(sensor == 3){
    lcd.setCursor(0,1);
    lcd.print("noise:          ");
  }
  else if(sensor == 4){
    lcd.setCursor(0,1);
    lcd.print("CO2:            ");
  }
  else if(sensor == 5){
    lcd.setCursor(0,1);
    lcd.print("CO:             ");
  }            
  else if(sensor == 6){
    lcd.setCursor(0,1);
    lcd.print("Air Qual:       ");
  }   
  
  lcd.setCursor(0,0);
  if(sensor==0 || sensor==3){
    lcd.print("SENSING>  ");
  } 
  else if(sensor==1 || sensor==4){ 
    lcd.print("SENSING>> ");
  } 
  else if(sensor==2 || sensor==5){ 
    lcd.print("SENSING>>>");
  }
  else if(sensor==6){ 
    lcd.print("SENSING >>");
  }
  
  lcd.setCursor(11,1);
  lcd.print(reading);
  lcd.setCursor(11,0);
  int timer = (millis() - lastConnectionTime)/1000; //time since last successful data upload
  lcd.print(timer);
  lcd.print("sec");
  breatheGreenLED();
}

void lcdEthernet(byte ip[], byte subnet[], byte gateway[], String feedAddr){

  int infoDelay = 2000;
  //show IP address
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Ethernet IP addr");
  lcd.setCursor(0,1);
  for(int i=0; i<=3; i++){
    lcd.print(int(ip[i]));
    if(i<3) lcd.print(".");
  }
  delay(infoDelay);

  //show subnet mask
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Ethernet subnet");
  lcd.setCursor(0,1);
  for(int i=0; i<=3; i++){
    lcd.print(int(subnet[i]));
    if(i<3) lcd.print(".");
  }
  delay(infoDelay);  

  //show gateway
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Ethernet gateway");
  lcd.setCursor(0,1);
  for(int i=0; i<=3; i++){
    lcd.print(int(gateway[i]));
    if(i<3) lcd.print(".");
  }
  delay(infoDelay); 

  //show gateway
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Pachube Feed ID:");
  lcd.setCursor(0,1);
  lcd.print(feedAddr);
  delay(infoDelay);   
}

// ---------------------------------
//These are debug message functions!
void lcdConnect(){
  lcd.setCursor(0,1);
  lcd.print("Connection opened ");
  lcd.setCursor(12,0);
}

void lcdPUT(){
  lcd.setCursor(0,1);
  lcd.print("PUT req. started "); 
  lcd.setCursor(13,0);
}
void lcdPachubeConnect(){
  lcd.setCursor(0,1);
  lcd.print("API credentials "); 
  lcd.setCursor(14,0);
}

void lcdDataPUT(){
  lcd.setCursor(0,1);
  lcd.print("PUT Data        ");
  lcd.setCursor(15,0);
}

void lcdStatusBar(){ 
  lcd.print(".");
  delay(1000);
}

void lcdConnectionFail(){
  Serial.println("*** connection failed ***");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connection Fail");
  lcd.setCursor(0,1);
  lcd.print("chck IP/subnt/gt");
  flashRedLED();
}

void lcdResetNow(){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("RESETTING BOARD NOW"); 
  flashRedLED();
}

