// Score counter with LCD screen
// Версия 1.0 от 12.06.2016

#define PLAYERCOUNT 2             // Players count
#define GAMEMODES 2               // Game modes count
#define PINBT1 8                  // Connection pin for 1 player button
#define PINBT2 9                  // Connection pin for 2 player button
#define PINBT3 10                  // Connection pin for Reset/Mode button
#define PINBUZZ 12                 // Connection pin for buzzer
#define PINLED 13
int ClickTime=100;
int LongClickTime=500;
int MaxScore[GAMEMODES]={10, 21}; // Maximum score for different modes
int BuzzPointFreq[PLAYERCOUNT]={4000,6000};          // Sound signal frequency adding point, Hz
int BuzzPointTime=300;            // Duration of the sound signal adding a point, ms
int BuzzWinFreq=2000;             // Frequency of sound signal when winning, Hz
int BuzzWinTime=800;              // Duration of the sound signal when winning, ms



#include <LiquidCrystal.h>    // Library for working with LCD screen
#include <OneButton.h>        // Library for processing button clicks
LiquidCrystal lcd(2, 3, 4, 5,  6,  7);  // Initializing the LCD screen connected to  pins 2,3,4,5,6,7
byte GameMode=0;              // Current game mode
OneButton Buttons[PLAYERCOUNT+1]={OneButton(PINBT1, true),OneButton(PINBT2, true),OneButton(PINBT3, true)};  
int Score[PLAYERCOUNT]={0,0}; //Current score
bool ScoreMode=true;          // Show scores
bool PlayerWinMode=false;     // Hode win message
int Winner=0;                 // Number of winner
void setup() 
{
  Serial.begin(9600);
  Serial.println("Score Board v1.0");
  pinMode(PINLED, OUTPUT);   
  digitalWrite(PINLED,LOW);
  lcd.begin(16, 2);         // Display initialization - 16 characters, 2 rows
  lcd.clear();              
  lcd.setCursor(0, 0);      // Welcome message
  lcd.print("   Score board");
  lcd.setCursor(0, 1);      
  lcd.print("      v 1.0");
  delay(2000);
  for (byte i=0; i< (PLAYERCOUNT+1); i++)     // For each player
  {
    Buttons[i].setClickTicks(ClickTime);
    Buttons[i].setPressTicks(LongClickTime);
  }
  Buttons[0].attachClick(Button1Click);     // Assigning a button click event
  Buttons[1].attachClick(Button2Click);
  Buttons[2].attachClick(ButtonResetClick);
  Buttons[2].attachLongPressStart(ButtonResetLongPress);    // Assigning the event of a long press of the reset button (switching game modes)
  GameMode=0;               // Game mode by default
  NewGame();                // New game
  LcdUpdate();              
}

void loop() 
{
  for (byte i=0; i< (PLAYERCOUNT+1); i++)
  {
    Buttons[i].tick();      // poll the status of all buttons
  }
  // put your main code here, to run repeatedly:
}
void Button1Click()
{
    AddPoint(0);    // Add a point to the first player
}
void Button2Click()
{
    AddPoint(1);    // Add a point to the second player
}
void ButtonResetClick()
{
    NewGame();      // New game
    LcdUpdate();
}
void ButtonResetLongPress()
{
  if (GameMode<GAMEMODES-1) GameMode++;          // Cycling game modes
  else GameMode=0;
  LcdUpdate();
}
void AddPoint(int Player)     // Adding score point
{
  if (ScoreMode)  
  {
    Score[Player]++;    // In counter mode - add point
    tone(PINBUZZ, BuzzPointFreq[Player], BuzzPointTime);    
    WinnerCheck();        // Check if you have won
  }
  else if (PlayerWinMode) NewGame();             //In winner display mode - start a new game
  LcdUpdate();                                
}
void NewGame()
{
  Serial.println("New game"); 
  lcd.clear();             
  lcd.setCursor(0, 0);      // Place the cursor at the beginning
  lcd.print("      NEW    ");
  lcd.setCursor(0, 1);      // Place the cursor at the beginning
  lcd.print("      GAME    ");
  digitalWrite(PINLED,HIGH);
  delay(2000);
  digitalWrite(PINLED,LOW);
  Winner=0;               // reset the winner
  for (byte i=0; i< (PLAYERCOUNT); i++)     // For each player
  {
    Score[i]=0;        // Reset scores
  }
  ScoreMode=true;        // Show scores
  PlayerWinMode=false;     // Hide player win message
}
void WinnerCheck()
{
  for (byte i=0; i< (PLAYERCOUNT); i++)     // For each player
  {
    if (Score[i]>=MaxScore[GameMode])    // The player has scored more than the maximum for current mode
    {
      tone(PINBUZZ, BuzzWinFreq, BuzzWinTime);
      
      Winner=i;               // Winner number
      ScoreMode=false;        // Hide scores
      PlayerWinMode=true;     // Show player win message
    }
  }
}
void LcdUpdate()      
{
  if (ScoreMode)    // Score display mode
  {
  for (byte i=0; i< (PLAYERCOUNT); i++)
  {
      lcd.setCursor(0, i);             // Cursor - first character of a line
      lcd.print("Player  :        ");
      lcd.setCursor(7, i);             // Cursor - to display the player number
      lcd.print(i+1);                  // Player number
      lcd.setCursor(10, i);            // Cursor - to display the player number
      lcd.print(Score[i]);          // Player score
      Serial.print("Player ");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(Score[i]); 
      Serial.print(", "); 
  }
  lcd.setCursor(13, 0);    // Displaying the maximum score
  lcd.print("max");
  lcd.setCursor(14, 1);    // Maximum score for current mode
  lcd.print(MaxScore[GameMode]);
  Serial.print("max ");
  Serial.print(MaxScore[GameMode]);
  Serial.println(";");
  }
  if (PlayerWinMode)    // Player win mode
  {
      digitalWrite(PINLED,HIGH);
      lcd.setCursor(0, 0);    // Show "Player # WIN!
      lcd.print("     Player      ");
      lcd.setCursor(12, 0);   
      lcd.print(Winner+1);      // Winner number
      lcd.setCursor(0, 1);    
      lcd.print("       WIN!      ");
      Serial.print("Player ");
      Serial.print(Winner);
      Serial.print(" WIN!");
      Serial.println(";");
  }
}

