uint8_t LED1 = 0x02, LED2 = 0x03, LED3 = 0x04, LED4 = 0x05;  /*Les asigno nombres solo para que sea mas entendible el codigo*/
uint8_t SW1 = 0x06, SW2 = 0x07, SW3 = 0x08, SW4 = 0x09;
uint8_t Aux = 0x01;
long CODIGOALEATORIO, CODIGOINTRODUCIDO;
int i, MODO, contador, T, t, j = 1;
int Led = 0x01;

void setup(){                                               /*Asignacion de las entradas y salidas*/
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
}

boolean antirebote(int pin){                                /*Funcion para evitar el ruido al accionar los botones, recibe como parametro de entrada el pin que se esta accionando*/
  contador = 0;                                           /*Comienza con un contador en cero*/
  boolean estado, estadoAnterior;                             /*Define las dos variables booleanas(0 o 1) a utilizar*/
  while( contador < 10){                                      /*Se ejecutara mientras el contador sea menor a 10*/
    estado = digitalRead(pin);                                /*guarda en la variable estado el estado actual del boton, puede ser cero o uno depende si esta apagado o no*/
    if( estado != estadoAnterior){                            /*Si es estado es distinto al estado anterior (Detecta un flanco)*/
      contador = 0;                                             /*El contado vuelve a cero*/
      estadoAnterior = estado;                                  /*En la variable estadoAnterior se guarda el estado actual del pulsador*/
    }else{                                                    /*Si el estado actual es igual al estado anterior*/
      contador++;                                               /*El contador se aumenta en 1*/
    }
  }
  return estado;                                            /*Cuando el contador llegue a 10 devuelve la variable estado que es el estado real del pulsador*/
}

void INICIO (){
                          /*Se calcula el tiempo que tienen que estar prendidos los leds*/
    if( Led > 8 ){                                /*Hay cuatro leds los cuales corresponderan a los bits 1 2 4 y 8 en la variable Led por lo cual si esta supera ese valor vuelve a 1*/
      Led = 0x01;
    }
    digitalWrite(LED1,Led & 0x01);                /*Se aplica una mascara para cada led con & entonces solo se encendera el led que su bit correspondiente en Led sea 1*/
    digitalWrite(LED2,Led & 0x02);
    digitalWrite(LED3,Led & 0x04);
    digitalWrite(LED4,Led & 0x08);
    T = millis();
    if ( T - t > 250){                 /*Se mantendra esta configuracion hasta que se supere el tiempo asignado en Tdelay*/
      Led <<= 1;                                  /*Cuando se supere este tiempo el bit a la izquierda se pone en 1 y el resto en cero*/
      t = millis();                              /*Se registra el tiempo en que esto sucedio en Ta*/
    }
}

void GANADORTOTAL(){
    contador = 0;
  Aux = 0x01;
  while(contador < 10){
    T = millis();
    if ( (T - t) > 250 ){
    digitalWrite(LED1, Aux);
    digitalWrite(LED2, Aux);
    digitalWrite(LED3, Aux);
    digitalWrite(LED4, Aux);
    digitalWrite(37, Aux);
    digitalWrite(39, Aux);
    digitalWrite(41, Aux);
    digitalWrite(43, Aux);
    digitalWrite(45, Aux);
    digitalWrite(47, Aux);
    digitalWrite(49, Aux);
    digitalWrite(51, Aux);
    digitalWrite(53, Aux);
    digitalWrite(52, Aux);
    Aux ^= 0x01;
    contador++;
    t = millis();
    }
  }
    j++;
}

void GANA(){
  contador = 0;
  Aux = 0x01;
  while(contador < 10){
    T = millis();
    if ( (T - t) > 250 ){
    digitalWrite(LED1, Aux);
    Aux ^= 0x01;
    contador++;
    t = millis();
    }
  }
    j++;
    MODO = 1;
    CODIGOINTRODUCIDO = 0;
    if( j > 10){
      GANADORTOTAL();
      MODO = 0;
    }
}

void PIERDE(){
  contador = 0;
  Aux = 0x01;
  while(contador < 10){
    T = millis();
    if ( (T - t) > 250 ){
    digitalWrite(LED4, Aux);
    Aux ^= 0x01;
    contador++;
    t = millis();
    }
  }
  j = 1;
  MODO = 0;
  CODIGOINTRODUCIDO = 0;
  Led = 0x01;
}

long SIMONDICE(int nivel){
  digitalWrite(LED1,LOW);                /*Se aplica una mascara para cada led con & entonces solo se encendera el led que su bit correspondiente en Led sea 1*/
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);
  CODIGOALEATORIO = 0;
  t = millis();
  T = millis();
  while( (T - t) < 500){
    T = millis(); 
  }
  for( i = 0 ; i < nivel; i++){
    CODIGOALEATORIO <<= 4;
    Aux = random(4);
    if ( Aux == 0){
      CODIGOALEATORIO |= 0x01;
    }
     if ( Aux == 1){
      CODIGOALEATORIO |= 0x02;
    }
     if ( Aux == 2){
      CODIGOALEATORIO |= 0x04;
    }
     if ( Aux == 3){
      CODIGOALEATORIO |= 0x08;
    }
    digitalWrite(LED1,CODIGOALEATORIO & 0x01);               
    digitalWrite(LED2,CODIGOALEATORIO & 0x02);
    digitalWrite(LED3,CODIGOALEATORIO & 0x04);
    digitalWrite(LED4,CODIGOALEATORIO & 0x08);
    t = millis();
    T = millis();
    while( (T - t) < 250){
      T = millis();
    }
    digitalWrite(LED1,LOW);               
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED4,LOW);
    t = millis();
    T = millis();
    while( (T - t) < 250){
      T = millis();
    }
  }
}

void LEER(int n){
  int i = 0;
  while ( i < n ){
    if(antirebote(SW1)){
      CODIGOINTRODUCIDO <<= 4;
      CODIGOINTRODUCIDO |= 0x00001;
      digitalWrite(LED1, HIGH);
      t = millis();
      T = millis();
      while ((T - t) < 250){
        T = millis();
      }
      digitalWrite(LED1, LOW);
      i++;
    }
    if(antirebote(SW2)){
      CODIGOINTRODUCIDO <<= 4;
      CODIGOINTRODUCIDO |= 0x00002;
      digitalWrite(LED2, HIGH);
      t = millis();
      T = millis();
      while ((T - t) < 250){
        T = millis();
      }
      digitalWrite(LED2, LOW);
      i++;
    }
    if(antirebote(SW3)){
      CODIGOINTRODUCIDO <<= 4;
      CODIGOINTRODUCIDO |= 0x00004;
      digitalWrite(LED3, HIGH);
      t = millis();
      T = millis();
      while ((T - t) < 250){
        T = millis();
      }
      digitalWrite(LED3, LOW);
      i++;
    }
    if(antirebote(SW4)){
      CODIGOINTRODUCIDO <<= 4;
      CODIGOINTRODUCIDO |= 0x00008;
      digitalWrite(LED4, HIGH);
      t = millis();
      T = millis();
      while ((T - t) < 250){
        T = millis();
      }
      digitalWrite(LED4, LOW);
      i++;
    }
  }
}

void NIVEL(int nivel){
  uint16_t niv = 0x0001;
  for( i = 0; i < 10; i++){
    if(nivel == i + 1){
    niv <<= i;
    }
  }
  if( nivel == 0){
    niv = 0x0000;
  }
  digitalWrite(37, niv & 0x0001);
  digitalWrite(39, niv & 0x0002);
  digitalWrite(41, niv & 0x0004);
  digitalWrite(43, niv & 0x0008);
  digitalWrite(45, niv & 0x0010);
  digitalWrite(47, niv & 0x0020);
  digitalWrite(49, niv & 0x0040);
  digitalWrite(51, niv & 0x0080);
  digitalWrite(53, niv & 0x0100);
  digitalWrite(52, niv & 0x0200);
}

void loop(){
  while( antirebote(SW1) == LOW && antirebote(SW2) == LOW && antirebote(SW3) == LOW && antirebote(SW4) == LOW && MODO == 0){
    INICIO();
  }
  NIVEL(j);
  SIMONDICE(j);
  LEER(j);
  if ( CODIGOINTRODUCIDO == CODIGOALEATORIO){
    GANA();
    }else{
      PIERDE();
      NIVEL(0);
  }
  while( antirebote(SW1) == HIGH || antirebote(SW2) == HIGH || antirebote(SW3) == HIGH || antirebote(SW4) == HIGH ){

  }
}
