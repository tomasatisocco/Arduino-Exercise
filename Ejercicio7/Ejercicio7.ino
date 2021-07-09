/*Simon Says Game with 10 levels, 4 Leds and 4 Buttons
In level 1 one random led will turn on and the player
has to press the correspndent button, in level 2 two leds will turn on, etc.
If player win the level the led 1 is going to blink 5 times, if lose, led 4 is going to blink 5 times
Leds will show each button pressed*/
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5

#define SW1 6
#define SW2 7
#define SW3 8
#define SW4 9

#define STAND 1
#define PLAYSIMON 2
#define READING 3
#define PLAYCORRECT 4
#define PLAYINCORRECT 5

uint8_t actualBtn, lastBtn, currentState, level, i, leds;
uint8_t randomNums[] = {0x01, 0x02, 0x04, 0x08};
uint8_t randomCode[10], inputCode[10];
unsigned long time, lastTimeDebounce, lastTimeOn, lastTimeSec;

void PlayerSays(uint8_t digit);
void ReadBtns();
void StartSec();
void CorrectSec();
void IncorrectSec();
void PlaySimonSays();
void SimonSays();
void ChangeState(uint8_t newState);

/*Read the button pressed avoiding the debounce problem*/

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
         if(currentState == STAND){
           ChangeState(PLAYSIMON);
           SimonSays();
         } else {
           if (lastBtn & 0x01 )
             leds = 0x01;
           if (lastBtn & 0x02 )
             leds = 0x02;
           if (lastBtn & 0x04 )
             leds = 0x04;
           if (lastBtn & 0x08 )
             leds = 0x08;
           PlayerSays(lastBtn);
           lastTimeOn = millis();
         }
         lastTimeDebounce = millis();
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

/*Diferent functions for each secuence of leds that need to be shown*/

void StartSec(){
    if (leds < 0x01 || leds > 0x08){
      leds = 0x01;
    }
    time = millis();
    if ( time - lastTimeSec > 250){
      leds <<= 1;
      lastTimeSec = millis();
    }
}

void CorrectSec(){
  time = millis();
  if (time - lastTimeSec > 250){
    leds ^= 0x01;
    i++;
    lastTimeSec = millis();
    if (i == 11)
      ChangeState(PLAYSIMON);
  }
}

void IncorrectSec(){
  time = millis();
  if (time - lastTimeSec > 250){
    leds ^= 0x08;
    i++;
    lastTimeSec = millis();
    if (i == 11){
      ChangeState(STAND);
    }
  }
}

/*Generate random secuence for each level, first restar randomCode and inputCode to
avoid the problem of old data*/

void SimonSays(){
  for (i = 0; i < 10; i++){
    randomCode[i] = 0;
    inputCode[i] = 0;
  }
  for (i = 0; i < level; i++){
    randomCode[i] = randomNums[random(4)];
  }
  ChangeState(PLAYSIMON);
}

/*Show the secuence of leds generated in SimonSays() changing each 500ms*/

void PlaySimonSays(){
  if (i <= level){
    time = millis();
    if (time - lastTimeSec > 500){
      leds = randomCode[i++];
      lastTimeSec = millis();
    }
  } else {
    ChangeState(READING);
  }
}

/*Save and compare the secuence that player do with the generated randomly*/

void PlayerSays(uint8_t digit){
  inputCode[i++] = digit;
  if (i == level){
    for (i = 0; i <= level; i++){
      if (randomCode[i] != inputCode[i]){
        ChangeState(PLAYINCORRECT);
        break;
      } else {
        if (i == level){
          ChangeState(PLAYCORRECT);
          break;
        }
      }
    }
  }
}

/*Change between teh diferent states making diferent action depending if the actual and new state*/

void ChangeState(uint8_t newState){
  switch (currentState) {
    case STAND:
      currentState = newState;
    break;
    case PLAYSIMON:
      currentState = newState;
    break;
    case READING:
      currentState = newState;
      leds = 0x00;
    break;
    case PLAYCORRECT:
      currentState = newState;
      level++;
      SimonSays();
      lastTimeSec = millis();
      leds = 0x00;
    break;
    case PLAYINCORRECT:
      currentState = newState;
      leds = 0x00;
      level = 1;
    break;
  }
  i = 0x00;
}

void setup(){

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);

/*noice in pin 0 generate a random seed each time*/

  randomSeed(analogRead(0));
  currentState = STAND;
  level = 1;
}

void loop(){
  switch (currentState) {
    case STAND:
      StartSec();
      ReadBtns();
    break;
    case PLAYSIMON:
      PlaySimonSays();
      time = millis();
      if (time - lastTimeSec > 300)
        leds = 0x00;
    break;
    case READING:
      ReadBtns();
      time = millis();
      if (time - lastTimeOn > 300)
        leds = 0x00;
    break;
    case PLAYCORRECT:
      CorrectSec();
    break;
    case PLAYINCORRECT:
      IncorrectSec();
    break;
  }
  digitalWrite(LED1,leds & 0x01);
  digitalWrite(LED2,leds & 0x02);
  digitalWrite(LED3,leds & 0x04);
  digitalWrite(LED4,leds & 0x08);
}
