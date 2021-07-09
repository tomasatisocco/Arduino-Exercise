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

#define SW1 6
#define SW2 7
#define SW3 8
#define SW4 9

#define STAND 1
#define PLAYING 2
#define READING 3

#define STARTSEC   flag1.bit.b0

uint8_t btnON, actualBtn, lastBtn, currentState;
uint32_t actualCode, savedCode;
unsigned long time, lastTimeDebounce, lastTimeOn, lastTimeSec, lastTimeSec1;

_flag flag1;

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

void StartSec(){
    if( Led > 0x08 ){
      Led = 0x01;
    }
    digitalWrite(LED1,Led & 0x01);
    digitalWrite(LED2,Led & 0x02);
    digitalWrite(LED3,Led & 0x04);
    digitalWrite(LED4,Led & 0x08);
    time = millis();
    if ( time - lastTimeSec > 250){
      Led <<= 1;
      lastTimeSec = millis();
    }
}

void ChangeState(uint8_t input){
  switch (currentState) {
    case STAND:
      STARTSEC = 0x00;
      currentState = PLAYING;
    break;
    case PLAYING:

    break;
    case READING:

    break;
  }
}

void setup(){

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(53, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(37, OUTPUT);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);

  STARTSEC = 0x01;
}

void loop(){
  ReadBtns();
  if (STARTSEC){
    StartSec();
  }
}
