#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<SPI.h>
#include<MFRC522.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// RFID pins
#define ssPin 5
#define rstPin 4
MFRC522 rfid(ssPin,rstPin);

// Relay Pins
const int relay1=25;
const int buzzer=2;


String cardsAccepted[] = {"01 02 03 04","11 22 33 44","55 66 77 88"};
String employeeName[] = {"Employee 1","Employee 2","Employee 3"};


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
  if (value){
    digitalWrite(relay1, HIGH);
   }
  else{
    digitalWrite(relay1, LOW);
  }
}

void RFIDAccepted(String accessedby){
  writetoLCD("Accessed by: ",accessedby);
  relayControl_Access(true);
  buzzerAccept();
  
}

void RFIDDenied(){
  writetoLCD("Access Denied","Try Again");
  relayControl_Access(false);
  buzzerAlert(5);

}

void loop() {
  Serial.println("New Porcess");
  writetoLCD("Chemmical Hazard","Move Out");
  delay(2000);

  // relayControl(true);
  // delay(1000);
  // relayControl(false);

  // digitalWrite(relay1, HIGH);

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

  for (int i=0;i<sizeof(cardsAccepted);i++){

    if (readUID == cardsAccepted[i]){
      RFIDAccepted(employeeName[i]);
      allowed = true;
      break;
    }
  }

  if (!allowed){
    RFIDDenied();
  }


  // bool allowed = false;
  // for (int i = 0; i < 4; i++) {
  //   if (readUID == masterCards[i]) {
  //     accessGranted(memberNames[i]);
  //     allowed = true;
  //     break;
  //   }
  // }

  // if (!allowed) {
  //   accessDenied();
  // }

  // Serial.print("Card UID:");
  // for (byte i = 0; i < rfid.uid.size; i++) {
  //   Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
  //   Serial.print(rfid.uid.uidByte[i], HEX);
  // }
  Serial.println();


   
  Serial.println("End of line");
  rfid.PICC_HaltA();
  delay(3000);

  // rfid.PICC_HaltA();
}
