// Счетичк очков с отображением на LCD экране
// Версия 1.0 от 12.06.2016

#define PLAYERCOUNT 2             // Количество игроков
#define GAMEMODES 2               // Количество режимов игры
#define PINBT1 8                  // Пин подключения 1 кнопки (увеличение 1 счетчика)
#define PINBT2 9                  // Пин подключения 2 кнопки (увеличение 2 счетчика)
#define PINBT3 10                  // Пин подключения 3 кнопки (сброс)
#define PINBUZZ 12                 // Пин подключения зуммера
#define PINLED 13
int ClickTime=100;
int LongClickTime=500;
int MaxScore[GAMEMODES]={10, 21}; // Максимальный счет для разных режимов
int BuzzPointFreq[PLAYERCOUNT]={4000,6000};          // Частота звуковго сигнала добавление очка, Гц
int BuzzPointTime=300;            // Длительность звукового сигнала добавление очка, мс
int BuzzWinFreq=2000;             // Частота звуковго сигнала при выиграше, Гц
int BuzzWinTime=800;              // Длительность звукового сигнала при выиграше, мс



#include <LiquidCrystal.h>    // Библиотека для работы с LCD экраном
#include <OneButton.h>        // Библиотека для обработки нажатий кнопок 
LiquidCrystal lcd(2, 3, 4, 5,  6,  7);  // Инициализируем LCD экран, подключенный к стандартным пинам
byte GameMode=0;              // Текущий игровой режим
OneButton Buttons[PLAYERCOUNT+1]={OneButton(PINBT1, true),OneButton(PINBT2, true),OneButton(PINBT3, true)};  
int Score[PLAYERCOUNT]={0,0}; //Текущий счет
bool ScoreMode=true;          // Режим отображения счета
bool PlayerWinMode=false;     // Режим отображения выиграша одного из игроков
int Winner=0;                 // Номер выигравшего игрока
void setup() 
{
  Serial.begin(9600);
  Serial.println("Score Board v1.0");
  pinMode(PINLED, OUTPUT);   
  digitalWrite(PINLED,LOW);
  lcd.begin(16, 2);         // Инициализируем дисплей
  lcd.clear();              // Очищаем дисплей
  lcd.setCursor(0, 0);      // Устанавливаем курсор в начало
  lcd.print("   Score board");
  lcd.setCursor(0, 1);      // Устанавливаем курсор в начало
  lcd.print("      v 1.0");
  delay(2000);
  for (byte i=0; i< (PLAYERCOUNT+1); i++)     // Для каждого игрока
  {
    Buttons[i].setClickTicks(ClickTime);
    Buttons[i].setPressTicks(LongClickTime);
  }
  Buttons[0].attachClick(Button1Click);     // Назначаем событие нажатия кнопок
  Buttons[1].attachClick(Button2Click);
  Buttons[2].attachClick(ButtonResetClick);
  Buttons[2].attachLongPressStart(ButtonResetLongPress);    // Назначаем событие длительного нажатия кнопки сброса (переключение игровых режимов)
  GameMode=0;               // Режим по умолчанию
  NewGame();                // Новая игра
  LcdUpdate();              // Обновляем дисплей
}

void loop() 
{
  for (byte i=0; i< (PLAYERCOUNT+1); i++)
  {
    Buttons[i].tick();      // Опрашиваем состояние всех кнопок
  }
  // put your main code here, to run repeatedly:
}
void Button1Click()
{
    AddPoint(0);    // Добавляем очко первому игроку
}
void Button2Click()
{
    AddPoint(1);    // Добавляем очко второму игроку
}
void ButtonResetClick()
{
    NewGame();      // Новая игра
    LcdUpdate();
}
void ButtonResetLongPress()
{
  if (GameMode<GAMEMODES-1) GameMode++;          // Циклическое переключение режимов игры
  else GameMode=0;
  LcdUpdate();
}
void AddPoint(int Player)     // Добавление очка к счетчику
{
  if (ScoreMode)  
  {
    Score[Player]++;    // В режиме счетчика - добавить счет
    tone(PINBUZZ, BuzzPointFreq[Player], BuzzPointTime);    
    WinnerCheck();        // Проверить, не выиграл ли
  }
  else if (PlayerWinMode) NewGame();             // В режиме отображения победителя - начать новую игру
  LcdUpdate();                                  // Обновляем изображение на экране
}
void NewGame()            // Новая игра
{
  Serial.println("New game"); 
  lcd.clear();              // Очищаем дисплей
  lcd.setCursor(0, 0);      // Устанавливаем курсор в начало
  lcd.print("      NEW    ");
  lcd.setCursor(0, 1);      // Устанавливаем курсор в начало
  lcd.print("      GAME    ");
  digitalWrite(PINLED,HIGH);
  delay(2000);
  digitalWrite(PINLED,LOW);
  Winner=0;               // обнулить победителя
  for (byte i=0; i< (PLAYERCOUNT); i++)     // Для каждого игрока
  {
    Score[i]=0;        // Обнулить счетчики
  }
  ScoreMode=true;        // Отображать счет
  PlayerWinMode=false;     // не отображать сообщение о выиграше
}
void WinnerCheck()
{
  for (byte i=0; i< (PLAYERCOUNT); i++)     // Для каждого игрока
  {
    if (Score[i]>=MaxScore[GameMode])    // Игрок набрал больше максимума для данного режима
    {
      tone(PINBUZZ, BuzzWinFreq, BuzzWinTime);
      
      Winner=i;               // номер победителя
      ScoreMode=false;        // не отображать счет
      PlayerWinMode=true;     // отображать сообщение о выиграше
    }
  }
}
void LcdUpdate()      // Обновляем изображение на дисплее
{
  if (ScoreMode)    // Режим отображения счета
  {
  for (byte i=0; i< (PLAYERCOUNT); i++)
  {
      lcd.setCursor(0, i);             // Курсор - первый символ строки
      lcd.print("Player  :        ");
      lcd.setCursor(7, i);             // Курсор - к отображению номера игрока
      lcd.print(i+1);                  // Номер игрока
      lcd.setCursor(10, i);            // Курсор - к отображению счета игрока
      lcd.print(Score[i]);          // счет игрока
      Serial.print("Player ");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(Score[i]); 
      Serial.print(", "); 
  }
  lcd.setCursor(13, 0);    // Отображение максимального счета
  lcd.print("max");
  lcd.setCursor(14, 1);    // Максимальный счет для текущего режима
  lcd.print(MaxScore[GameMode]);
  Serial.print("max ");
  Serial.print(MaxScore[GameMode]);
  Serial.println(";");
  }
  if (PlayerWinMode)    // Режим отображения сообщения о выиграше
  {
      digitalWrite(PINLED,HIGH);
      lcd.setCursor(0, 0);    // Отображение сообщения о выиграше
      lcd.print("     Player      ");
      lcd.setCursor(12, 0);   
      lcd.print(Winner+1);      // Номер победителя
      lcd.setCursor(0, 1);    
      lcd.print("       WIN!      ");
      Serial.print("Player ");
      Serial.print(Winner);
      Serial.print(" WIN!");
      Serial.println(";");
  }
}

