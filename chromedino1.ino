#include <LiquidCrystal.h>
//Dino positions
#define run_1 0//run position 1
#define run_2 1//run position 2
#define jump_high 2//jump high
#define jump_half_head 3//jump half upper
#define jump_half_body 4//jump half lower

#define upperObject1 5
#define lowerObject1 6
#define empty 7

int dino_up_pos=0;
int dino_low_pos=0;


//initialize lib with interface pin #
LiquidCrystal lcd (11,9,6,5,4,3);
static bool buttonPushed = false;
static byte currentPos;
static byte currentPosUp;
static byte currentPosDown;
static int score = 0;
static bool playing = false;
static bool scoring =false;
const int buttonPin = 2;
int buttonState = 0;
static int upperBG[17] = {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
static int lowerBG[17] = {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};

void graphics(){
byte dino[64] = {
  //0-Run 1
 0b11111,
 0b10111,
 0b11000,
 0b11101,
 0b11110,
 0b11100,
 0b10100,
 0b10010,
 //1-Run 2
 0b11111,
 0b10111,
 0b11000,
 0b11100,
 0b11110,
 0b11100,
 0b01100,
 0b01110,
 //2-Jump High 
 0b11111,
 0b10111,
 0b11000,
 0b11101,
 0b11110,
 0b11100,
 0b11011,
 0b00000,
 //3-Jump Half (Head)
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b11111,
 0b10111,
 //4-Jump Half (Body)
 0b11000,
 0b11101,
 0b11110,
 0b11100,
 0b11011,
 0b00000,
 0b00000,
 0b00000,
 //5-Upper1
 0b11111,
 0b11111,
 0b11111,
 0b11110,
 0b11110,
 0b11110,
 0b00000,
 0b00000,
 //6-Lower1
 0b00000,
 0b00000,
 0b01110,
 0b01110,
 0b01110,
 0b01110,
 0b01110,
 0b01110,
 //7-Empty 
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
};
int g;
//draw dino positions
  for (g = 0; g < 8; g++) {
    lcd.createChar(g, &dino[g*8]);
  }
}
void drawDino0(){
  //Starting screen
  lcd.setCursor (0,0);
  lcd.print("Press to Start!");
  lcd.setCursor (0,1);
  currentPos = run_1;
  lcd.write(byte(currentPos));
}
void drawDinoWalk(){
  dino_low_pos=1;
  lcd.setCursor(0,1);
  currentPos = run_2;
  lcd.write(byte(currentPos));
  delay(200);
  lcd.setCursor(0,1);
  currentPos = run_1;
  lcd.write(byte(currentPos));
  delay(200);
  detect();
}
void drawDinoJump(){
  //half up + down
  dino_low_pos=1;
  dino_up_pos=1;
  lcd.setCursor (0,0);
  currentPosUp = jump_half_head;
  lcd.write(byte(currentPosUp));
  lcd.setCursor (0,1);
  currentPosDown = jump_half_body;
  lcd.write(byte(currentPosDown));
  detect();
  
  delay(200);
  drawObject();
  //up
  dino_low_pos=0;
  dino_up_pos=1;
  lcd.setCursor (0,0);
  currentPosUp = jump_high;
  lcd.write(byte(currentPosUp));
  lcd.setCursor (0,1);
  lcd.print(" ");
  detect();
  
  
  delay(200);
  drawObject();
  detect();
  delay(200);
  drawObject();
  detect();
  
  //down
  dino_low_pos=1;
  dino_up_pos=1;
  lcd.setCursor (0,0);
  currentPosUp = jump_half_head;
  lcd.write(byte(currentPosUp));
  lcd.setCursor (0,1);
  currentPosDown = jump_half_body;
  lcd.write(byte(currentPosDown));
  delay(200);
  lcd.setCursor(0,0);
  lcd.print(" ");
  dino_up_pos=0;
  dino_low_pos=1;
  detect();
}
void buttonPush() {
  buttonPushed = true;
}


void objectRandom (int upperBG[], int lowerBG[]){

  int ObjectPos = random(5,10);
  
  //Object Array
  switch (ObjectPos){
  case upperObject1:
    lowerBG[16] = empty;
    //upper and lower too close
    if (lowerBG[15] == lowerObject1) upperBG[16] = empty;
    else upperBG[16] = upperObject1;    
   break;
  case lowerObject1:
    //Max two lower objects in a row or upper and lower too close
    if ((lowerBG[15] == lowerObject1)||(lowerBG[14] == lowerObject1)|| (lowerBG[13] == lowerObject1)||(upperBG[15] == upperObject1)){
      lowerBG[16] = empty;
    }
    else lowerBG[16] = lowerObject1;
   break;
  case empty:
  case 8: //empty2
  case 9: //empty3
  case 10: //empty4
    upperBG[16] = empty;
    lowerBG[16] = empty;
   break;
  }
}


void detect(){
  if((dino_up_pos==1&&upperBG[0]!=7)||(dino_low_pos==1&&lowerBG[0]!=7)){
    lcd.clear();
    lcd.setCursor(1,0);
    playing = false;
    scoring = false;
  }
}

void drawObject (){
  objectRandom(upperBG,lowerBG);
  
  for (int i = 0; i < 16; i++){
    upperBG[i]=upperBG[i+1];
    lowerBG[i]=lowerBG[i+1];
  }
  if(upperBG[0]!=7){
      lcd.setCursor(0,0);
      lcd.write(byte(upperBG[0]));
  }
  else if(dino_up_pos==0){
    lcd.setCursor(0,0);
      lcd.print(" ");
  }


  if(lowerBG[0]!=7){
      lcd.setCursor(0,1);
      lcd.write(byte(lowerBG[0]));
  }
  else if(dino_low_pos==0){
    lcd.setCursor(0,1);
      lcd.print(" ");
  }

  for (int i = 1; i < 16; i++){
      lcd.setCursor(i,0);
      lcd.write(byte(upperBG[i]));
    
      lcd.setCursor(i,1);
      lcd.write(byte(lowerBG[i]));
  }

}

void over(){
   lcd.clear();
   lcd.setCursor (0,0);
   lcd.print ("Game Over!");
   lcd.setCursor (0,1);
   lcd.print ("Score:");
   lcd.setCursor (8,1);
   lcd.print (score);
   delay(100000000000000000);
}
void setup() {
  //Readwrite
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  //Contrast
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  //Button
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  //Autoplay
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  randomSeed(analogRead(A0));
  
  attachInterrupt(0/*PIN_BUTTON*/, buttonPush, FALLING);
  
  graphics();
  //set up LCD (columns,rows)
  lcd.begin(16, 2);
}
void loop() {
  //detect();
  if (playing == true){
  drawObject();
  }
  if (!playing) {
    if(score==0){
      drawDino0();
    }
    else{
      over();
    }
  }
  else {
    drawDinoWalk();
    if(buttonPushed){
      drawDinoJump();
      buttonPushed = false;
    }
  }
  
  if (buttonPushed) {
    graphics();
    playing = true;
    scoring = true;
    buttonPushed = false;
    lcd.clear();
  }
  if (scoring == true){
    score++;
  }
  //Generate object
  
    return;
}