#include <Arduino.h>
/*************** + Definición de constantes y variables + ***************/
const int16_t LED_GREEN = 32;
const int16_t LED_YELLOW = 33;
const int16_t LED_RED = 25;
const int16_t LED_BLUE = 26;
const int16_t SW1 = 27;
const int16_t SW2 = 14; 
const int16_t SW3 = 12; 
const int16_t SW4 = 13;
int16_t leds[] ={LED_GREEN, LED_YELLOW, LED_RED, LED_BLUE};
int16_t len_leds = sizeof(leds)/sizeof(int16_t);
int16_t buttons[] = {SW1, SW2, SW3, SW4};
int16_t len_button = sizeof(buttons)/sizeof(int16_t);
int16_t sem[] ={LED_GREEN, LED_YELLOW, LED_RED};
int16_t len_sem = sizeof(sem)/sizeof(int16_t);
int16_t tim[] ={1000, 500, 200};
double f_time = 1.0;
typedef enum{
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_FALLING,
  BUTTON_RISING,
  BUTTON_UP_2,
  BUTTON_DOWN_2,
  BUTTON_FALLING_2,
  BUTTON_RISING_2
} fsmButtonState_t;
fsmButtonState_t fsmButtonState;                    // States of the FSM SW1
fsmButtonState_t fsmButtonState2;                   // States of the FSM SW2
enum sem{NORMAL, DISCONECTED, ALARM};
enum fac{one, two, half};
typedef struct{
  const int16_t* sem_light;                         // puntero
  const int16_t* sem_time;                          // puntero
  const int16_t  length;                            // longitud del semáforo (3)
  enum sem       modes;                             // variable enumerada de modos
  enum fac       times;                             // variable enumerada de tiempos
} sequenceControl;
/************************************************************************/
int16_t NonBlockingDelay(int16_t t_delay);
int16_t NonBlockingDelay2(int16_t t_delay);
void FsmButtonInit(void);
void FsmButtonUpdate(int16_t buttons[], sequenceControl controler);
void ButtonPressed (void);
void ButtonReleased (sequenceControl controler);
void FsmButtonError (void);
void FsmButtonError2 (void);
void ButtonPressed_2 (void);
void ButtonReleased_2 (sequenceControl controler);
void TurnOffLeds(sequenceControl controler);
void Sequence(sequenceControl controler);
void SelectTime(sequenceControl controler);
void SelectMode(sequenceControl controler);
void setup() {
  for (int16_t i=0; i<len_leds;i++) pinMode(leds[i],OUTPUT);
  for (int16_t i=0; i<len_button;i++) pinMode(buttons[i],INPUT);
  Serial.begin(115200);
}
/**************************** + Main loop + *****************************/
void loop() {
  enum sem modes = NORMAL;
  enum fac times = one;
  sequenceControl controlModes ={sem, tim, len_sem, modes, times};             // objeto (con estructura)
  FsmButtonInit();
  while(1){
    FsmButtonUpdate(buttons, controlModes);
    Sequence(controlModes);
  }
}
/************************* + End Main loop + **************************/
void FsmButtonInit(void){
  fsmButtonState = BUTTON_UP;
  fsmButtonState2 = BUTTON_UP_2;
}
void FsmButtonUpdate(int16_t buttons[], sequenceControl controler){
  switch (fsmButtonState)
  {
  case  BUTTON_UP:
    if (digitalRead(buttons[0])){
      fsmButtonState = BUTTON_FALLING;
    }
    break;
  case  BUTTON_DOWN:
    if (!digitalRead(buttons[0])){
      fsmButtonState = BUTTON_RISING;
    }
    break;
  case  BUTTON_FALLING:
    if (NonBlockingDelay(40)){
      if (digitalRead(buttons[0])){
        fsmButtonState = BUTTON_DOWN;
        ButtonPressed();
      } else {
        fsmButtonState = BUTTON_UP;
      }
    }
    break;
  case  BUTTON_RISING:
    if (NonBlockingDelay(40)){
      if (!digitalRead(buttons[0])){
        fsmButtonState = BUTTON_UP;
        ButtonReleased(controler);
      } else {
        fsmButtonState = BUTTON_DOWN;
      }
    }
    break;
  default:
    FsmButtonError();
    break;
  }
  // Segundo Botón--------------------------
  switch (fsmButtonState2)
  {
  case  BUTTON_UP_2:
    if (digitalRead(buttons[1])){
      fsmButtonState2 = BUTTON_FALLING_2;
    }
    break;
  case  BUTTON_DOWN_2:
    if (!digitalRead(buttons[1])){
      fsmButtonState2 = BUTTON_RISING_2;
    }
    break;
  case  BUTTON_FALLING_2:
    if (NonBlockingDelay(40)){
      if (digitalRead(buttons[1])){
        fsmButtonState2 = BUTTON_DOWN_2;
        ButtonPressed_2();
      } else {
        fsmButtonState2 = BUTTON_UP_2;
      }
    }
    break;
  case  BUTTON_RISING_2:
    if (NonBlockingDelay(40)){
      if (!digitalRead(buttons[1])){
        fsmButtonState2 = BUTTON_UP_2;
        ButtonReleased_2(controler);
      } else {
        fsmButtonState2 = BUTTON_DOWN_2;
      }
    }
    break;
  default:
    FsmButtonError2();
    break;
  }
}
void ButtonPressed (void){
  digitalWrite(LED_BLUE, HIGH);
}
void ButtonReleased (sequenceControl controler){
  digitalWrite(LED_BLUE, LOW);
  SelectMode(controler);
}
void FsmButtonError (void){
  fsmButtonState = BUTTON_UP;
}
void ButtonPressed_2 (void){
  digitalWrite(LED_BLUE, HIGH);
}
void ButtonReleased_2 (sequenceControl controler){
  digitalWrite(LED_BLUE, LOW);
  SelectTime(controler);
}
void FsmButtonError2 (void){
  fsmButtonState2 = BUTTON_UP_2;
}

/*********************** + Non-blocking delay + ***********************/
int16_t NonBlockingDelay(int16_t t_delay){
  int16_t t_base = 1;                   // Time base for delay
  static int16_t count = 0;             // Counting variable
  int16_t n_time = t_delay / t_base;    // # of samples per delay
  if (count >= n_time) {
    count = 0;
    return 1;                           // time accomplished
  } else{
    delay(t_base);
    count ++;
    return 0;
  }
}
int16_t NonBlockingDelay2(int16_t t_delay){
  int16_t t_base = 1;                   // Time base for delay
  static int16_t count = 0;             // Counting variable
  int16_t n_time = t_delay / t_base;    // # of samples per delay
  if (count >= n_time) {
    count = 0;
    return 1;                           // time accomplished
  } else{
    delay(t_base);
    count ++;
    return 0;
  }
}
void TurnOffLeds(sequenceControl controler){
  for (int16_t i = 0; i < controler.length; i++){
    digitalWrite(controler.sem_light[i], LOW);
  }
}
void Sequence(sequenceControl controler) {
  static int16_t pos = 0;                   // static: Entre llamadas a la función conserva su valor
  static boolean flag = true;
  TurnOffLeds(controler);
  if (flag){
    digitalWrite(controler.sem_light[pos], HIGH);
  }
  Serial.println(f_time);
  Serial.println(controler.times);
  switch (controler.modes)
  {
    case NORMAL:
      flag =1;
      if (NonBlockingDelay2(controler.sem_time[pos] * f_time)){
        if (pos < controler.length - 1) {
          ++pos;
        } else {
          pos = 0;
        }
      }
      break;
    case DISCONECTED:
      if (NonBlockingDelay2(controler.sem_time[pos])){
        pos = 1;
        flag = !flag;
      }
      break;
    case ALARM:
      if (NonBlockingDelay2(controler.sem_time[pos]/2)){
        pos = 2;
        flag = !flag;
      }
      break;
  }
}
void SelectTime(sequenceControl controler){
  Serial.println("FTime--------------------------------_______----------");
  // static int16_t pos = 0;
  switch (controler.times){
    case one:
      f_time = 2.0;
      controler.times = two;
      break;
    case two:
      f_time = 0.5;
      controler.times = half;
      break;
    case half:
      f_time = 1.0;
      controler.times = one;
      break;
  }
  
  // if (f_time == 1){
  //   f_time = 2;
  // }
  // if (f_time == 2){
  //   f_time = 0.5;
  // }
  // if (f_time == 0.5){
  //   f_time = 1;
  // }
}

void SelectMode(sequenceControl controler){
  switch (controler.modes){
    case NORMAL:
      controler.modes = DISCONECTED;
      break;
    case DISCONECTED:
      controler.modes = ALARM;
      break;
    case ALARM:
      controler.modes = NORMAL;
      break;
  }
}