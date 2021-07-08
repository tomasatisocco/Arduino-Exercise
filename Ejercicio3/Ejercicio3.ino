/* This program turn on and turn of each LED every time the correspondent button is pressed*/

#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5

#define SW1 6
#define SW2 7
#define SW3 8
#define SW4 9

void ReadBtns();

uint8_t btnON = 0b00000000;                          
uint8_t actualBtn, lastBtns = 0x00;
unsigned long debounceTime, lastTimeDebounce, delay = 30;

void setup(){
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
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
      debounceTime = millis();
      if( (debounceTime - lastTimeDebounce) > delay){
       if (actualBtn ^ lastBtns){
         lastBtns = actualBtn;
         if (lastBtns & 0x01 )
           btnON ^= 0x01;
         if (lastBtns & 0x02 )
           btnON ^= 0x02;
         if (lastBtns & 0x04 )
           btnON ^= 0x04;
         if (lastBtns & 0x08 )
           btnON ^= 0x08;
       }
       else{
        lastTimeDebounce = millis();
       }
     }
  }else{
    lastBtns = 0x00;
    lastTimeDebounce = millis();
  }
}

void loop() {
  ReadBtns();
  digitalWrite(LED1,btnON & 0x01);
  digitalWrite(LED2,btnON & 0x02);
  digitalWrite(LED3,btnON & 0x04);
  digitalWrite(LED4,btnON & 0x08);
}
