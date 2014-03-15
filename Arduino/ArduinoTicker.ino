#include <LiquidCrystal.h>


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.home();
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("ArduinoTicker");
  lcd.setCursor(0,1);
  lcd.print("v0.1");
  lcd.display();
}

void loop() {

}
