/*
   ***_-_-_ weather tunnel sensor box v08 _-_-_***
 
 Circuit:
 * Ethernet shield attached to digital pins 10, 50, 51, 52, 53
 * LCD Screen on pins 49, 45, 29, 27, 25, 23 
 * LEDs: 12 (red), 13 (blue)
 * RESET pin connected to: pin 7
 
 Sensors:
 * ADMP401 ambient sound mic on    pin A0  - +3v - GND
 * TEMT6000 ambient light on       pin A2  - +5v - GND
 * humidity on                     pin A5  - +5v - GND
 * MQ-7 Carbon Monoxide sensor on  pin A8  - +5v - GND - D14
 * MG-811 Carbon Dioxide sensor on pin A14 - +9v - GND
 * MQ-135 Air Quality sensor on    pin A10 - +5v - GND
 * TMP102 Temperature sensor on    pin D20 (SDA-BLUE) - D21 (SCL-WHITE) - +3v - GND
 
 joseph saavedra
 http://jos.ph
 january 2011
 */

// institution
String institutionName = "Tester!";

// institution specific Pachube feed address
String feedAddr = "23656";  //Test: 23656

//unique ethernet shield MAC addr:  90-A2-DA-00-2D-A3
//this number can be found printed on the bottom of your Arduino
// fill in your ethernet shield's MAC address here:
byte mac[] = {  
  0x90, 0xA2, 0xDA, 0x00, 0x2D, 0x16   };


//TEST IP:        192.168.2.2
//Subnet Mask:    255.255.255.0
//Router:         192.168.2.1

//IP in Room 1100: 149.31.36.204
//Subnet Mask:        255.255.255.0
//Router:             149.31.36.1

// IP address for the controller:
byte ip[] = {  
  149, 31, 36, 204 };

// subnet mask -- if unneeded {255,255,255,0} works
byte subnet[] = {  
  255, 255, 255, 0  };

// router address -- if unneeded {192,168,1,1} works
byte gateway[] = {  
  149, 31, 36, 1  };	

//  Pachube server address:
byte server[] = {
//  209,40,205,190}; //THIS IS THE OLD ADDRESS
  173,203,98,29}; //THIS IS THE NEW PACHUBE ADDRESS!

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <CS_MQ7.h>

LiquidCrystal lcd(49, 45, 29, 27, 25, 23);

CS_MQ7 MQ7(14, 6); //Citizen Sensor MQ-7 Carbon Monoxide Breakout instance
int CoPrev = 0; //for storing CO Data

Client client(server, 80);

long lastConnectionTime = 0;        // last time you connected to the server, in milliseconds
boolean lastConnected = false;      // state of the connection last time through the main loop
const int postingInterval = 30000;  //delay between updates to Pachube.com, in milliseconds
const int softwareResetPin = 7; //connect this pin to RESET pin
const long int resetInterval = 120000; //if no successful connection has been made in 2 min, RESET board

void setup() {
  digitalWrite(softwareResetPin, HIGH); //softwareReset hack
  pinMode(softwareResetPin, OUTPUT);  //softwareReset hack
  Serial.begin(9600);
  Serial.println("start setup");
  lcdSetup();
  Ethernet.begin(mac, ip);
  lcdEthernet(ip, subnet, gateway, feedAddr);
  Wire.begin();
  lastConnectionTime = millis();
  Serial.println("end setup");
  delay(1000);
  setupLedFlash();
  lcd.clear();
}

void loop() {
  Serial.println("Reading Sensors...");
  lcd.clear();
  String dataString = " "; //will hold all sensor data

  // read the sensors:
  Serial.println("sensor 0 - temperature..."); //temperature
  int sensor0_reading = 0;
  sensor0_reading = getTemperature();
  dataString = String(sensor0_reading);
  lcdUpdater(0, sensor0_reading);
  Serial.println(dataString);

  Serial.println("sensor 1 - light..."); //light
  int sensor1_reading = 0;
  sensor1_reading = getLight();
  dataString += ",";
  dataString = dataString + String(sensor1_reading);
  lcdUpdater(1, sensor1_reading);
  Serial.println(dataString);

  Serial.println("sensor 2 - humidity..."); //humidity
  int sensor2_reading = getHumidity();
  dataString += ",";
  dataString = dataString + String(sensor2_reading);
  lcdUpdater(2, sensor2_reading);
  Serial.println(dataString);

  Serial.println("sensor 3 - noise..."); //noise
  int sensor3_reading = getNoise();
  dataString += ",";
  dataString = dataString + String(sensor3_reading);
  lcdUpdater(3, sensor3_reading);
  Serial.println(dataString);

  Serial.println("sensor 4 - CO2..."); //CO2
  //int sensor4_reading = analogRead(8);
  int sensor4_reading = getCO2();
  dataString += ",";
  dataString = dataString + String(sensor4_reading);
  lcdUpdater(4, sensor4_reading);
  Serial.println(dataString);

  Serial.println("sensor 5 - CO..."); //CO
  //int sensor4_reading = analogRead(10);
  int sensor5_reading = getCO();
  dataString += ",";
  dataString = dataString + String(sensor5_reading);
  lcdUpdater(5, sensor5_reading);
  Serial.println(dataString);

  Serial.println("sensor 6 - Air Qual..."); //Air Quality
  //int sensor4_reading = analogRead(10);
  int sensor6_reading = getAirQual();
  dataString += ",";
  dataString = dataString + String(sensor6_reading);
  lcdUpdater(6, sensor6_reading);
  Serial.println(sensor6_reading);

  //if we have passed the timer interval set for posting (3000ms), 
  //then send data!
  if(millis() - lastConnectionTime > postingInterval){
    Serial.println("sendData!");
    if(millis() - lastConnectionTime > resetInterval){
      softwareReset(); //RESET ENTIRE BOARD
    }
    else{
      sendData(dataString);
    }
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
  Serial.println("... end of loop ...");
}


// make an HTTP connection to the server:
void sendData(String thisData) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Attempting ");
  lcd.setCursor(0,1);
  lcd.print("Connect and Send");
  // if there's a successful connection:
  if (client.connect()) {
    Serial.println("connecting...");
    lcd.setCursor(12,0);
    lcdConnect();
    lcdStatusBar();

    // send the HTTP PUT request. 
    lcdPUT();
    client.print("PUT /api/"+feedAddr+".csv HTTP/1.1\n");
    lcdStatusBar();

    lcdPachubeConnect();   
    client.print("Host: www.pachube.com\n");
    // weather tunnel API key: mFgy2cgsyv4GKhw4G4EzNcpgpfhQJkg4JvCrNpOskwo
    client.print("X-PachubeApiKey: mFgy2cgsyv4GKhw4G4EzNcpgpfhQJkg4JvCrNpOskwo\n");
    client.print("Content-Length: ");
    client.println(thisData.length(), DEC);
    lcdStatusBar();

    // last pieces of the HTTP PUT request:
    client.print("Content-Type: text/csv\n");
    client.println("Connection: close\n");

    // here's the actual content of the PUT request:
    lcdDataPUT();
    client.println(thisData);
    lcdStatusBar();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("   Data Sent!   ");
    flashGreenLED();
    // note the time that the connection was made:
    lastConnectionTime = millis();

    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  } 
  else {
    // if you couldn't make a connection:
    lcdConnectionFail();
  }
}

