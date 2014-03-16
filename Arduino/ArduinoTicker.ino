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

//buffer for reading from serial bus
const int serialBufferSize = 32;
char serialBuffer[serialBufferSize];

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//mock method, replaces reading from serial bus
//for testing
void getNextItemFromSerial() {
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

//parse message from serial bus and update the list of tickers
//message format:
// SYMBOL;price;change;
//e.g. GOOG;1034.21;0;
void parseSerialInput(char buffer[32]) {   
   String command = String(buffer);
   int sepIndex = command.indexOf(";");
   String symbol = command.substring(0,sepIndex);
   command = command.substring(sepIndex+1);
   
   sepIndex = command.indexOf(";");
   char conversionBuffer[32];
   String priceSubstring = command.substring(0, sepIndex);
   priceSubstring.toCharArray(conversionBuffer,32);
   float price = atof(conversionBuffer);
   
   command = command.substring(sepIndex+1);
   sepIndex = command.indexOf(";");
   short change = (short) command.substring(0, sepIndex).toInt(); 
   
   updateItemsList(symbol, price, change);
}

//update the values displayed on the screen, reads from tickerItems
//shows only max 2 values now
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

//update items list
//either update record for given symbol or append with a new one
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
  //set up serial connection
  Serial.begin(9600);
  Serial.println("READY");
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("ArduinoTicker");
  lcd.setCursor(0,1);
  lcd.print("v0.1");
  lcd.display();
}

void loop() {
  byte received = Serial.readBytes(serialBuffer, serialBufferSize);
  if(received > 0) {
    parseSerialInput(serialBuffer);
  }
  
  updateScreen();
  delay(100);
}
