#include <LiquidCrystal.h>

typedef struct tickerItem {
  String symbol;
  float price;
  // -1 for negative change in recent price, 0 for no change, and 1 for positive
  short change; 
};

const int maxItems = 2;
int itemsAdded = 0;

tickerItem tickerItems[maxItems];

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void getNextItemFromSerial() {
  //mocked
  static int lastSymbol = -1;
  static float lastPrice = 100;

  lastSymbol++;

  if(lastSymbol >= maxItems) lastSymbol = 0;

  lastPrice += 11.35;

  String symbol;

  if(lastSymbol == 0) symbol = "BTC/USD";
  else symbol = "LTC/USD";

  updateItemsList(symbol, lastPrice, 0);
}

void updateScreen() {
  lcd.clear();
  lcd.home();

  if(itemsAdded == 0) {
    lcd.print("No symbols found");
  } else {
    for(short i = 0; i < itemsAdded; i++) {
      lcd.print(tickerItems[i].symbol + " ");
      lcd.print(tickerItems[i].price);

      if(i == 0) {
        lcd.setCursor(0,1);
      }
    }
  }
}

void updateItemsList(String symbol, float price, short change) {
  short index = -1;

  for(short i = 0; i < maxItems; i++) {
    if(tickerItems[i].symbol == symbol) {
      index = i;
      break;
    }
  }

  if(index >= 0) {
    tickerItems[index].price = price;
    tickerItems[index].change = change;
  } else {
    tickerItems[itemsAdded].symbol = symbol;
    tickerItems[itemsAdded].price = price;
    tickerItems[itemsAdded].change = change;

    itemsAdded++;
  }
}

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
  updateScreen();
  delay(1000);
  getNextItemFromSerial();
}
