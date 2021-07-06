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

#define DigitoIngresado   flag1.bit.b0
#define ABIERTO           flag1.bit.b1
#define SecCorrecto       flag1.bit.b2
#define SecError          flag1.bit.b3
#define builtin           flag1.bit.b4

void leerBotones();
void secAbierto();
void SecCerrado();
void Error();
void Correcto();
                        
uint8_t ESTADO, estado, estadoAnterior, ledPrendido, i, CantDigitos;
uint32_t CLAVEINGRESADA, CLAVEGUARDADA;
unsigned long Tiempo, UTDebounce, UTPrendido, UTsec, UTSec;

_flag flag1;

void leerBotones(){
  if (digitalRead(SW1) || digitalRead(SW2) || digitalRead(SW3) || digitalRead(SW4)){
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
      if( (Tiempo - UTDebounce) >= 30){
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
         UTPrendido = millis();
         DigitoIngresado = 1;
         UTDebounce = 0;
       }
       else{
        UTDebounce = millis();
       }
     }
  }
  else{
    estadoAnterior = 0x00;
    UTDebounce = millis();
  }
}

void secAbierto(){
  Tiempo = millis();
  if((Tiempo - UTsec) > 250){
    builtin ^= 0x01;
    UTsec = millis();
  }
}

void SecCerrado(){
  Tiempo = millis();
  if(builtin && ((Tiempo - UTsec) > 100)){
    builtin = 0;
    UTsec = millis();
  }
  if(!builtin && ((Tiempo - UTsec) > 1000)){
    builtin = 1;
    UTsec = millis();
  }
}

void Correcto(){
  if (i == 0)
    ESTADO = 0;
  Tiempo = millis();
  if((Tiempo - UTSec) >= 500){
    ESTADO ^= 0b00001111;
    i++;
    if(i == 7){
      i = 0;
      SecCorrecto = 0;
    }
    UTSec = millis();
  }
}

void Error(){
  if( i == 0)
   ESTADO = 0x00;
  Tiempo = millis();
  if((Tiempo - UTSec) >= 250){
    ESTADO ^= 0b00001111;
    i++;
    if(i == 13){
      i = 0;
      SecError = 0;
    }
    UTSec = millis();
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

  ABIERTO = 1;
  ledPrendido = 0x01;
}

void loop(){
  leerBotones();
  if(DigitoIngresado){
    CLAVEINGRESADA <<= 4;
    CLAVEINGRESADA |= ESTADO;
    DigitoIngresado = 0;
    CantDigitos++;
  }
  if( CantDigitos == 6 ){
    CantDigitos = 0;
    if(ABIERTO){
      CLAVEGUARDADA = CLAVEINGRESADA;
      ABIERTO = 0;
    }
    else{
      if(CLAVEINGRESADA == CLAVEGUARDADA){
        CLAVEGUARDADA = 0;
        SecCorrecto = 1;
        ABIERTO = 1;
      }
      else
        SecError = 1;
    }
    CLAVEINGRESADA = 0;
  }
  if(ABIERTO)
    secAbierto();
  if(!ABIERTO)
    SecCerrado();
  if (SecCorrecto)
    Correcto();
  if (SecError)
    Error();
  if(!SecCorrecto && !SecError){
    Tiempo = millis();
    if ((Tiempo - UTPrendido) > 250)
      ESTADO = 0x00;
  }
  digitalWrite(LED1,ESTADO & 0x01); 
  digitalWrite(LED2,ESTADO & 0x02);
  digitalWrite(LED3,ESTADO & 0x04);
  digitalWrite(LED4,ESTADO & 0x08);
  digitalWrite(LEDBUILTIN, builtin);
}
