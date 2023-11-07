/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
/*RS - 8
D4 - 4
D5 - 5
D6 - 6
D7 - 7
*/
//                RS E  D4 D5 D6 D7
LiquidCrystal lcd(8, 9, 4, 5, 6, 7 );
int button;
const int BUTTON_NONE   = 0;
const int BUTTON_RIGHT  = 1;
const int BUTTON_UP     = 2;
const int BUTTON_DOWN   = 3;
const int BUTTON_LEFT   = 4;
const int BUTTON_SELECT = 5;

int getPressedButton()
{
  int buttonValue = analogRead(0); // считываем значения с аналогового входа(A0) 
  if (buttonValue < 100) {
    return BUTTON_RIGHT;  
  }
  else if (buttonValue < 200) {
    return BUTTON_UP;
  }
  else if (buttonValue < 400){
    return BUTTON_DOWN;
  }
  else if (buttonValue < 600){
    return BUTTON_LEFT;
  }
  else if (buttonValue < 800){
    return BUTTON_SELECT;
  }
  return BUTTON_NONE;
}

void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello");
  Serial.begin(9600);
  Serial.println("Setup");
  lcd.setCursor(0,0);
  lcd.print("LCD Key Shield");
  lcd.setCursor(0,1);
  lcd.print("Press Key:");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
button = getPressedButton();
  switch (button)
  {
    case BUTTON_RIGHT: // при нажатии кнопки выводим следующий текст
       lcd.setCursor(0, 0); 
       lcd.print("                 "); 
       lcd.setCursor(0, 0);
       lcd.print("BUTTON: RIGHT");
         Serial.println("BUTTON: RIGHT");
       break;
    case BUTTON_LEFT:
       lcd.setCursor(0, 0); 
       lcd.print("                   "); 
       lcd.setCursor(0, 0);
       lcd.print("BUTTON: LEFT");
        Serial.println("BUTTON: LEFT");
       break;
    case BUTTON_UP:
       lcd.setCursor(0, 0); 
       lcd.print("                  "); 
       lcd.setCursor(0, 0);
       lcd.print("BUTTON: UP");
        Serial.println("BUTTON: UP");
       break;
    case BUTTON_DOWN:
       lcd.setCursor(0, 0); 
       lcd.print("                "); 
       lcd.setCursor(0, 0);
       lcd.print("BUTTON: DOWN");
        Serial.println("BUTTON: DOWN");
       break;
    case BUTTON_SELECT:
       lcd.setCursor(0, 0); 
       lcd.print("                 "); 
       lcd.setCursor(0, 0);
       lcd.print("BUTTON: SELECT");
        Serial.println("BUTTON: SELECT");
       break;
  }
  
}

