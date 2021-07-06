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

#define ADELANTE    flag1.bit.b0
#define ATRAS    flag1.bit.b1

void leerBotones();

uint8_t ESTADO = 0b00000000;                          
uint8_t estado, estadoAnterior, multiplicador, ledPrendido;
unsigned long Tiempo, UTDebounce, Delay, UTDelay;

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
  
  ADELANTE = 1;
  multiplicador = 0x02;
  ledPrendido = 0x01;
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
      Tiempo = millis();
      Delay = 30;
      if( (Tiempo - UTDebounce) > Delay){
       if (estado ^ estadoAnterior){
         estadoAnterior = estado;
         if (estadoAnterior & 0x01 )
           ESTADO = 0x01;
         if (estadoAnterior & 0x02 )
           ESTADO = 0x02;
         if (estadoAnterior & 0x04 )
           ESTADO = 0x04;
         if (estadoAnterior & 0x08 )
           ESTADO = 0x08;
       }
       else{
        UTDebounce = 0;
       }
     }
  }else{
    estadoAnterior = 0x00;
    UTDebounce = millis();
  }
}

void loop() {
  leerBotones();
  if (ESTADO & 0x01){
    ADELANTE = 1;
    ATRAS = 0;
  }
  if (ESTADO & 0x02){
    ADELANTE = 0;
    ATRAS = 1;
  }
  if (ESTADO & 0x04){
    ADELANTE = 0;
    ATRAS = 0;
  }
  if (ESTADO & 0x08){
    multiplicador <<= 1;
    if (multiplicador > 16)
      multiplicador = 0x01;
  }
  Delay = 125 * multiplicador ;
  Tiempo = millis();
  if ((Tiempo - UTDelay) > Delay){
    if(ADELANTE){
      ledPrendido <<= 1;
      UTDelay = millis();
    }
    if (ATRAS){
      ledPrendido >>= 1;
      UTDelay = millis();
    }
    if(ledPrendido > 8)
      ledPrendido = 0x01;
    if (ledPrendido < 1)
      ledPrendido = 0x08;
    if (!ADELANTE && !ATRAS)
      ledPrendido = 0x00;
  }
  digitalWrite(LED1,ledPrendido & 0x01); 
  digitalWrite(LED2,ledPrendido & 0x02);
  digitalWrite(LED3,ledPrendido & 0x04);
  digitalWrite(LED4,ledPrendido & 0x08);
  ESTADO = 0x00;
}
