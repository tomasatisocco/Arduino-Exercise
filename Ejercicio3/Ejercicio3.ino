#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5

#define SW1 6
#define SW2 7
#define SW3 8
#define SW4 9

void leerBotones();

uint8_t ESTADO = 0b00000000;                          
uint8_t estado, estadoAnterior = 0x00;
unsigned long TDebounce, UTDebounce, Delay = 30;

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

void leerBotones(){
  if ( digitalRead(SW1) || digitalRead(SW2) || digitalRead(SW3) || digitalRead(SW4)){
    estado = 0x00;
    if( digitalRead(SW1))
      estado |= 0x01;
    if( digitalRead(SW2))
      estado |= 0x02;
    if( digitalRead(SW3))
      estado |= 0x04;
    if( digitalRead(SW4))
      estado |= 0x08;
      TDebounce = millis();
      if( (TDebounce - UTDebounce) > Delay){
       if (estado ^ estadoAnterior){
         estadoAnterior = estado;
         if (estadoAnterior & 0x01 )
           ESTADO ^= 0x01;
         if (estadoAnterior & 0x02 )
           ESTADO ^= 0x02;
         if (estadoAnterior & 0x04 )
           ESTADO ^= 0x04;
         if (estadoAnterior & 0x08 )
           ESTADO ^= 0x08;
       }
       else{
        UTDebounce = millis();
       }
     }
  }else{
    estadoAnterior = 0x00;
    UTDebounce = millis();
  }
}

void loop() {
  leerBotones();
  digitalWrite(LED1,ESTADO & 0x01); 
  digitalWrite(LED2,ESTADO & 0x02);
  digitalWrite(LED3,ESTADO & 0x04);
  digitalWrite(LED4,ESTADO & 0x08);
}
