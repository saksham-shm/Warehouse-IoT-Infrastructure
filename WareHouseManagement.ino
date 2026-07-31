#include<Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);


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
  
  // SDA = Serial Data -> 21 SCL = Serial Clock ->23
  //Initialize the lcd
  Wire.begin(21,23);      

  lcd.init();
  lcd.backlight();

}

void loop() {
  writetolcd("Temp:23.humid:34","Occupancy : 5");
  delay(10000);
}
