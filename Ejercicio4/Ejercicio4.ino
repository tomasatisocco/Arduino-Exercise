/* This program count with 4 buttons and 4 leds.
If button 1 is pressed the leds turn on in secuence, ie. first LED1 turno on followe for LED2 etc.
each LED is on for 500ms and if the button 2 is pressed the secuence start in the oposite way
With the button 3 all LEDS turn off
With the button 4 the time each led is turned on multiplies by 2 till 2sec after that the time change to 125ms*/

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

#define FOWARD    flag1.bit.b0
#define BACKWARD    flag1.bit.b1

void ReadBtns();

uint8_t btnON = 0b00000000;
uint8_t actualBtn, lastBtn, multiplier, ledON;
unsigned long time, lastTimeDebounce, delay, lastTimedelay;

_flag flag1;

void setup(){
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);

  FOWARD = 1;
  multiplier = 0x02;
  ledON = 0x01;
}

void ReadBtns(){
  if ( digitalRead(SW1) || digitalRead(SW2) || digitalRead(SW3) || digitalRead(SW4)){
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
      delay = 30;
      if( (time - lastTimeDebounce) > delay){
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
       }
       else{
        lastTimeDebounce = 0;
       }
     }
  }else{
    lastBtn = 0x00;
    lastTimeDebounce = millis();
  }
}

void loop() {
  ReadBtns();
  if (btnON & 0x01){
    FOWARD = 1;
    BACKWARD = 0;
  }
  if (btnON & 0x02){
    FOWARD = 0;
    BACKWARD = 1;
  }
  if (btnON & 0x04){
    FOWARD = 0;
    BACKWARD = 0;
  }
  if (btnON & 0x08){
    multiplier <<= 1;
    if (multiplier > 16)
      multiplier = 0x01;
  }
  delay = 125 * multiplier ;
  time = millis();
  if ((time - lastTimedelay) > delay){
    if(FOWARD){
      ledON <<= 1;
      lastTimedelay = millis();
    }
    if (BACKWARD){
      ledON >>= 1;
      lastTimedelay = millis();
    }
    if(ledON > 8)
      ledON = 0x01;
    if (ledON < 1)
      ledON = 0x08;
    if (!FOWARD && !BACKWARD)
      ledON = 0x00;
  }
  digitalWrite(LED1,ledON & 0x01);
  digitalWrite(LED2,ledON & 0x02);
  digitalWrite(LED3,ledON & 0x04);
  digitalWrite(LED4,ledON & 0x08);
  btnON = 0x00;
}
