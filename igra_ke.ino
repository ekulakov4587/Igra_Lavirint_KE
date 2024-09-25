#include <LiquidCrystal.h>
int Xmax=16;
int Ymax=2;

const int rs = 12, en = 11, d4 = 6, d5 = 4, d6 = 13, d7 = 2;
LiquidCrystal  lcd(rs, en, d4, d5, d6, d7);

bool thisFrameControlUpdated = false;
unsigned long lastGameUpdateTick = 0;
unsigned long gameUpdateInterval = 200;

// Joystick
int x_pin = A0;
int y_pin = A1;
int sw_pin = 8;

bool endGame;

int goal_x, goal_y , start_x, start_y, wall1_x, wall1_y, wall2_x, wall2_y, p_x, p_y, newPos_x, newPos_y;

enum Direction {LEFT, RIGHT, UP, DOWN, NONE};

Direction getJoystick()
{
  int x_data = analogRead(A0);
  int y_data = analogRead(A1);

  if(x_data<150)
    return LEFT;
  if(x_data>900)
    return RIGHT;
  if(y_data<150)
    return UP;
  if(y_data>900)
    return DOWN;
  return NONE;
}

void game_init()
{
  goal_x=15;
  goal_y=0;

  start_x=0;
  start_y=1;

  wall1_x=3;
  wall1_y=1;
  wall2_x=10;
  wall2_y=0;

  lcd.setCursor(wall1_x, wall1_y);
  lcd.print("|");

  lcd.setCursor(wall2_x, wall2_y);
  lcd.print("|");

  lcd.setCursor(start_x, start_y);
  lcd.print("*");

  lcd.setCursor(goal_x, goal_y);
  lcd.print("O");

  p_x=start_x;
  p_y=start_y;
}

void move(Direction d)
{
  newPos_x = p_x;
  newPos_y = p_y;

  if (d == LEFT){
    newPos_x = p_x-1;
  }
  else if (d == RIGHT) {
    newPos_x = p_x+1;
  } else if (d == UP) {
      newPos_y = p_y-1;
  }
  else if (d == DOWN) {
    newPos_y = p_y+1;
  }

  if (newPos_x<Xmax && newPos_y<Ymax)
  if (newPos_x>=0 && newPos_y>=0)
  if  (!(newPos_x==wall1_x && newPos_y==wall1_y))
  if (!(newPos_x==wall2_x && newPos_y==wall2_y))
  {
    lcd.setCursor(p_x, p_y);
    lcd.print(" ");
    p_x=newPos_x;
    p_y=newPos_y;
    lcd.setCursor(p_x, p_y);
    lcd.print("*");
  }
}

void setup() {
  lcd.begin(Xmax, Ymax);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(8, INPUT);
  digitalWrite(8, HIGH);
  
  game_init();

  //led diodi
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  endGame = false;
}

void loop() { 
  Direction dir;
  dir = getJoystick();
  if(!thisFrameControlUpdated){
  if (dir !=NONE)
    if(!endGame)
    {
      move(dir);
    }
    if(goal_x==p_x && goal_y==p_y)
    {
      endGame=true;
      lcd.setCursor(0, 0);
      lcd.print("Congratulations!");
      lcd.setCursor(0,1);
      lcd.print("****************");
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      delay(500);
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      delay(500);
    }
    thisFrameControlUpdated = true;
  }

  if(millis()-lastGameUpdateTick>gameUpdateInterval)
  {
    lastGameUpdateTick = millis();
    thisFrameControlUpdated = false;
  }
 
}
