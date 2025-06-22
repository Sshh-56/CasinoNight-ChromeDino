//   O N E - A R M E D  B A N D I T 
//   ==============================
// This software simulates slot machine by
// arduino board. Simulation environment contains 
// three slots, which can held four separate 
// values (heart, spade, club or diamond). The
// value is random based. The bet is one euro, and
// if all the slots have same value, the profit is 8 euros.
// If two of the slots have same value, the profit
// is 2 euros.
//
#include <LiquidCrystal.h>

const int RS = 12;
const int EN = 11;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
//-----------------------------------------------------------
const int sw1    = 6;
const int sw2    = 7;
const int led1   = 10;
const int buzzer = 9;

unsigned long interval1       = 1500;
unsigned long interval2       = 3000;
unsigned long interval3       = 4500;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

boolean aOn     = false; // Status of slot a
boolean bOn     = false; // Status of slot b
boolean cOn     = false; // Status of slot c
boolean running = false;

int money    = 0;  // Status of the Cash in the beginning

byte heart[8] =
{
  	B00000,
    B01010,
    B11111,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000
};

byte diamond[8] = 
{
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000 
};

byte spade[8] = 
{
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B00100,
  B01110,
  B00000 
};

byte club[8] = 
{
  B00000,
  B01110,
  B01110,
  B11111,
  B11111,
  B00100,
  B01110,
  B00000 
};

byte box[8] =
{
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000
};

#define HEART   0
#define SPADE   1
#define CLUB    2
#define DIAMOND 3
#define BOX     4

int a = BOX;
int b = BOX;
int c = BOX;

double temp;
double tempCelcius;
double tempFahrenheit;
double humidity;

unsigned long currentMillis;

void setup() {
  	Serial.begin(9600);
  	pinMode(led1, OUTPUT);
  	pinMode(sw1, INPUT_PULLUP);
  	pinMode(sw2, INPUT_PULLUP);
  	pinMode(buzzer, OUTPUT);
    lcd.begin(16, 2);
  	lcd.print("*Arduino Casino*");
    lcd.createChar(HEART, heart);
    lcd.createChar(SPADE, spade);
    lcd.createChar(CLUB,  club);
    lcd.createChar(DIAMOND, diamond);
    randomSeed(analogRead(0));
  	delay(1000);
    lcd.clear();
    lcd.print("Cash       Start");
  	printCash();
}
 
void loop()  {
  	
  	if(running && money > 0) {
      currentMillis = millis();
      //printCash();
      gameIsRunning();
      
      if(currentMillis - previousMillis1 > interval1 && aOn == false) {
          lcd.clear();
          Serial.println("Print A slot");
           printPattern(a, BOX, BOX);
          //printCash();
			gameIsRunning();
          aOn = true;
      }

      if(currentMillis - previousMillis2 > interval2 && bOn == false) {
          lcd.clear();
          Serial.println("Print A and B slot");
          printPattern(a, b, BOX);
          //printCash();
          gameIsRunning();
          bOn = true;
      }

      if(currentMillis - previousMillis3 > interval3 && !cOn) {
          lcd.clear();
          Serial.println("Print A and B and C slots");
          printPattern(a, b, c);
         
          cOn = true;
        
          running = false;
          // Let's check the staus of the slots when
          // all the slots have been drawn
          if (a == b || b == c || a == c) {
            if( a == b && a == c ) {
            	Serial.println("Win 8 euros");
              	money = money + 8 - 1; // Minus bet
              	youAreWinner();
              	blinkingLedAndSounding(); blinkingLedAndSounding();
            } else {
             	Serial.println("Win 2 euros");
              	money = money + 2 - 1; // Minus bet
                youAreWinner();
              	blinkingLedAndSounding();
            }
          } else {
            Serial.println("Charging 1 euro");
            money--; 
          }
          printCash();
       }
      
    }
  
  	// The beginning of the cycle, and all variables
    // will be initialized.
  	if(digitalRead(sw1) == LOW && !running && money > 0) {
        currentMillis = millis();
      	previousMillis1 = currentMillis;
        previousMillis2 = currentMillis;
      	previousMillis3 = currentMillis;
        aOn = false;
      	bOn = false;
        cOn = false;
      	lcd.clear();
        a = random(4);
        b = random(4);
      	c = random(4);
      	Serial.println("Restart Game");
      	printPattern(BOX, BOX, BOX);
      	running = true;
     	delay(100); 
    }
  
    if(digitalRead(sw2) == LOW && !running) {
      	Serial.println("Insert 1 Euro to Cash");
        money++;
     	delay(300); 
      	printCash();
    }
  
  if (money <= 0)  {
   	increaseMoney(); 
  }
}

void printPattern( int a, int b, int c)  {
  	lcd.setCursor(0,0);
	lcd.print('[');
    lcd.print(' ');
    lcd.write(byte(a));
    lcd.print(' ');
    lcd.print(']');
    lcd.print('[');
    lcd.print(' ');
    lcd.write(byte(b));
    lcd.print(' ');
    lcd.print(']');
    lcd.print('[');
    lcd.print(' ');
    lcd.write(byte(c));
    lcd.print(' ');
    lcd.print(']');  
}

void printCash() {
 	lcd.setCursor(0,1);
  	lcd.print("Total: ");
  	lcd.print(money);
  	lcd.write(" EUR  ");
}

void blinkingLedAndSounding(){
  	int i;

  	for(i = 0; i < 3 ; i++) {
      tone(buzzer,261);
      digitalWrite(led1, HIGH);
      delay(300);
      noTone(buzzer); 
      tone(buzzer,293);
      digitalWrite(led1, LOW);
      delay(300);    
      noTone(buzzer); 
      tone(buzzer,329);
      digitalWrite(led1, HIGH);
      delay(300);
      noTone(buzzer);     
      tone(buzzer,349);    
      digitalWrite(led1, LOW);
      delay(300);    
      noTone(buzzer); 
      tone(buzzer,392);
      digitalWrite(led1, HIGH);
      delay(300);
      digitalWrite(led1, LOW);
      noTone(buzzer);
    }
}

void youAreWinner(){
 	lcd.setCursor(0,1);
  	lcd.print("You are Winner");
}

void increaseMoney(){
  	lcd.setCursor(0,1);
  	lcd.print("Increase Money");
}

void gameIsRunning(){
  	lcd.setCursor(0,1);
  	lcd.print("Running       ");
}