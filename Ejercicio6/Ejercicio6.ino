/* Simulation of a security box with a 6 digit code, every digit can go from 1 to 4
introduced by 4 Buttons.
When the box is open de LED_BUILTIN blink each 250ms
If the 6 digits are introduced the box is closed and the led builtin blink 100ms each 1 sec
If the code to open the box is correct all LEDs blink 3 times
If it is incorrect all the LEDs blink each 250ms for 3 sec
Every time you press a button the correspondet led turn on for 250ms*/

#include <Arduino.h>

typedef union{
  struct{
    uint8_t b0: 1;
    uint8_t b1: 1;
    uint8_t b2: 1;
    uint8_t b3: 1;
    uint8_t b4: 1;
    uint8_t b5: 1;
    uint8_t b6: 1;
    uint8_t b7: 1;
  }bit;
  uint8_t byte;
}_flag;

#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LEDBUILTIN 13

#define SW1 6
#define SW2 7
#define SW3 8
#define SW4 9

#define INPUTDIGIT   flag1.bit.b0
#define BOXOPEN           flag1.bit.b1
#define CORRECTSEC       flag1.bit.b2
#define ERRORSEC          flag1.bit.b3
#define BUILTIN           flag1.bit.b4

void ReadBtns();
void OpenSec();
void ClosedSec();
void ErrorCode();
void CorrectCode();

uint8_t btnON, actualBtn, lastBtn, i, numOfDigits;
uint32_t actualCode, savedCode;
unsigned long time, lastTimeDebounce, lastTimeOn, lastTimeSec2, lastTimeSec1;

_flag flag1;

/*Check if a button is pressed and save it in btnON*/

void ReadBtns(){
  if (digitalRead(SW1) || digitalRead(SW2) || digitalRead(SW3) || digitalRead(SW4)){
    actualBtn = 0x00;
    if( digitalRead(SW1))
      actualBtn |= 0x01;
    if( digitalRead(SW2))
      actualBtn |= 0x02;
    if( digitalRead(SW3))
      actualBtn |= 0x04;
    if( digitalRead(SW4))
      actualBtn |= 0x08;
      time = millis();
      if( (time - lastTimeDebounce) >= 30){
       if (actualBtn ^ lastBtn){
         lastBtn = actualBtn;
         if (lastBtn & 0x01 )
           btnON = 0x01;
         if (lastBtn & 0x02 )
           btnON = 0x02;
         if (lastBtn & 0x04 )
           btnON = 0x04;
         if (lastBtn & 0x08 )
           btnON = 0x08;
         lastTimeOn = millis();
         INPUTDIGIT = 1;
         lastTimeDebounce = 0;
       }
       else{
        lastTimeDebounce = millis();
       }
     }
  }
  else{
    lastBtn = 0x00;
    lastTimeDebounce = millis();
  }
}

/* Secuence of Open and Close box */

void OpenSec(){
  time = millis();
  if((time - lastTimeSec2) > 250){
    BUILTIN ^= 0x01;
    lastTimeSec2 = millis();
  }
}

void ClosedSec(){
  time = millis();
  if(BUILTIN && ((time - lastTimeSec2) > 100)){
    BUILTIN = 0;
    lastTimeSec2 = millis();
  }
  if(!BUILTIN && ((time - lastTimeSec2) > 1000)){
    BUILTIN = 1;
    lastTimeSec2 = millis();
  }
}

/*Secuence of correct and incorrect Code when the box is closed*/

void CorrectCode(){
  if (i == 0)
    btnON = 0;
  time = millis();
  if((time - lastTimeSec1) >= 500){
    btnON ^= 0b00001111;
    i++;
    if(i == 7){
      i = 0;
      CORRECTSEC = 0;
    }
    lastTimeSec1 = millis();
  }
}

void ErrorCode(){
  if( i == 0)
   btnON = 0x00;
  time = millis();
  if((time - lastTimeSec1) >= 250){
    btnON ^= 0b00001111;
    i++;
    if(i == 13){
      i = 0;
      ERRORSEC = 0;
    }
    lastTimeSec1 = millis();
  }
}

void setup(){
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LEDBUILTIN, OUTPUT);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);

  BOXOPEN = 0x01;
}

void loop(){
  ReadBtns();
  if(INPUTDIGIT){
    actualCode <<= 4;
    actualCode |= btnON;
    INPUTDIGIT = 0;
    numOfDigits++;
  }
  if( numOfDigits == 6 ){
    numOfDigits = 0;
    if(BOXOPEN){
      savedCode = actualCode;
      BOXOPEN = 0x00;
    }
    else{
      if(actualCode == savedCode){
        savedCode = 0;
        CORRECTSEC = 1;
        BOXOPEN = 0x01;
      }
      else
        ERRORSEC = 1;
    }
    actualCode = 0;
  }
  if(BOXOPEN)
    OpenSec();
  if(!BOXOPEN)
    ClosedSec();
  if (CORRECTSEC)
    CorrectCode();
  if (ERRORSEC)
    ErrorCode();
  if(!CORRECTSEC && !ERRORSEC){
    time = millis();
    if ((time - lastTimeOn) > 250)
      btnON = 0x00;
  }
  digitalWrite(LED1,btnON & 0x01);
  digitalWrite(LED2,btnON & 0x02);
  digitalWrite(LED3,btnON & 0x04);
  digitalWrite(LED4,btnON & 0x08);
  digitalWrite(LEDBUILTIN, BUILTIN);
}
