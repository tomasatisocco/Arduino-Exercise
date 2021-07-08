/*This program turn on a Led while the correspondent button is pressed*/

#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5

#define SW1 6
#define SW2 7
#define SW3 8
#define SW4 9

uint8_t btnON = 0b00000000;

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

void loop() {
  btnON = 0b00000000;
  if(digitalRead(SW1)){
    btnON |= 0x01;
  }
   if(digitalRead(SW2)){
    btnON |= 0x02;
  }
   if(digitalRead(SW3)){
    btnON |= 0x04;
  }
   if(digitalRead(SW4)){
    btnON |= 0x08;
  }
  digitalWrite(LED1,btnON & 0x01);
  digitalWrite(LED2,btnON & 0x02);
  digitalWrite(LED3,btnON & 0x04);
  digitalWrite(LED4,btnON & 0x08);
}
