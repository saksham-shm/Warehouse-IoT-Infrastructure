#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<SPI.h>
#include<MFRC522.h>
#include<ESP32Servo.h>
#include<DHT.h>


LiquidCrystal_I2C lcd(0x27,16,2);

// RFID pins
#define ssPin 5
#define rstPin 4
#define dhtOne 16
#define dhtTwo 17
#define dhtType DHT22

MFRC522 rfid(ssPin,rstPin);
DHT dht1(dhtOne,dhtType);
DHT dht2(dhtTwo,dhtType);

// Relay Pins
const int relay1=25;
const int buzzer=2;
const int servopin=13;

Servo servo1;

String cardsAccepted[] = {"01 02 03 04","11 22 33 44","55 66 77 88"};
String employeeName[] = {"Employee 1","Employee 2","Employee 3"};

unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 2000;



void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
    // SDA = Serial Data -> 23 SCL = Serial Clock ->22
  //Initialize the lcd
  Wire.begin(21,22);  // LCD Screen 1    
  lcd.init();
  lcd.backlight();
  // Relay
  pinMode(relay1, OUTPUT);
  // digitalWrite(relay1,LOW);
  // Buzzer
  pinMode(buzzer,OUTPUT);
  // RFID

  servo1.attach(servopin);
  servo1.write(0);

  dht1.begin();
  dht2.begin();
  

  Serial.println("MFRC522 Ready");
}

// Function to write into lcd Screen with no return value
void writetoLCD(String line1, String line2){
  lcd.clear();
// Write new information
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}

void buzzerAccept(){
  for (int i=0;i<2;i++){
    tone(buzzer, 1800);
    delay(100);
    noTone(buzzer);
    delay(100);
  }
  delay(500);
}

void buzzerAlert (int time){
  for ( int i=0;i<time;i++){
    // plays the buzzer the required times.
    tone(buzzer,900);
    delay(150);
    tone(buzzer,1800);
    delay(150);
    noTone(buzzer);
    delay(150);
  }
}

void relayControl_Access(boolean value){
  if (value == true){
    digitalWrite(relay1, HIGH);
   }
  else{
    digitalWrite(relay1, LOW);
  }
}


void RFIDAccepted(String accessedby){
  writetoLCD("Accessed by: ",accessedby);
  relayControl_Access(true);
  servo1.write(90);
  buzzerAccept();
  delay(5000);
  servo1.write(0);
  
}

void RFIDDenied(){
  writetoLCD("Access Denied","Try Again");
  relayControl_Access(false);
  servo1.write(0);
  buzzerAlert(5);

}

void readDHT(float dhtData[]){

  float humidOne = dht1.readHumidity();
  float humidTwo = dht2.readHumidity();

  float tempOne = dht1.readTemperature();
  float tempTwo = dht2.readTemperature();

  float ftempOne = dht1.readTemperature(true);
  float ftempTwo = dht2.readTemperature(true);

  dhtData[0] = humidOne;
  dhtData[1] = humidTwo;
  dhtData[2] = tempOne;
  dhtData[3] = tempTwo;
  dhtData[4] = ftempOne;
  dhtData[5] = ftempTwo;

  if (isnan(humidOne) || isnan(humidTwo) || isnan(tempOne) || isnan(tempTwo) || isnan(ftempOne) || isnan(ftempTwo)) {
    Serial.println("DHT read failed");
    return;
}
  // dhtData[] = {humidOne,humidTwo,tempOne,tempTwo,ftempOne,ftempTwo};

}

void loop() {
  Serial.println("New Porcess");
  // writetoLCD("Chemmical Hazard","Move Out");
  // delay(2000);
  float reading[6];
  if (millis() - lastSensorRead >= sensorInterval ){

    lastSensorRead = millis();
    readDHT(reading);
  String output1 = "H1:" + String(reading[0],1) + "%";
  String output2 = "H2:" + String(reading[1],1) + "%";
    writetoLCD(output1, output2);
  }

  

  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  String content = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  String readUID = content.substring(1);

  Serial.print("UID: ");
  Serial.println(readUID); 

  boolean allowed = false;
  const int NumCards = sizeof(cardsAccepted) / sizeof(cardsAccepted[0]);

  for (int i=0;i<NumCards;i++){

    if (readUID == cardsAccepted[i]){
      RFIDAccepted(employeeName[i]);
      allowed = true;
      break;
    }
  }

  if (!allowed){
    RFIDDenied();
  }

 
  Serial.println("End of line");
  rfid.PICC_HaltA();
  // delay(3000);

}
