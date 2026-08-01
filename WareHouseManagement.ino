#include<Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

const int relay1=25;
const int buzzer=2;


// Function to write into lcd Screen with no return value
void writetolcd(String line1, String line2){
  lcd.clear();
// Write new information
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}


void setup() {
  
  Serial.begin(115200);
  
  // SDA = Serial Data -> 21 SCL = Serial Clock ->22
  //Initialize the lcd
  Wire.begin(21,22);  // LCD Screen 1    
  lcd.init();
  lcd.backlight();

  // Relay
  pinMode(relay1, OUTPUT);
  // digitalWrite(relay1,LOW);

  // Buzzer
  pinMode(buzzer,OUTPUT);

}

void loop() {
  // writetolcd("Chemmical Hazard","Move Out");

  // digitalWrite(relay1, HIGH);

  for (int i=0;i<4;i++){
    tone(buzzer,1800);
    delay(200);
    tone(buzzer,1000);
    delay(200);

  }




  
  Serial.print("End of line");
  delay(3000);


}
