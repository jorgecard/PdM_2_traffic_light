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
enum dir{ascending, descending};                   //Enum var for sequence
typedef struct{
  const int16_t* leds;                    // puntero
  const int16_t  length;
  enum dir       orientation;
} sequenceControl;
/************************************************************************/
void TurnOffLeds(sequenceControl controler);
void Sequence(sequenceControl controler);
bool ReadKey(int16_t key);

void setup() {
    for (int16_t i=0; i<len_leds;i++) pinMode(leds[i],OUTPUT);
    for (int16_t i=0; i<len_button;i++) pinMode(buttons[i],INPUT);
    Serial.begin(115200);
}
/**************************** + Main loop + *****************************/
void loop() {
  enum dir orientation = ascending;
  int16_t selector = 0;
  sequenceControl controlLeds ={leds, len_leds, orientation};                // objeto (con estructura)
  while(1){
    if (ReadKey(SW1)){
      selector = !selector;
      if (selector) {
        controlLeds.orientation = ascending;
        Serial.println("selector ascending");
      } else {
        controlLeds.orientation = descending;
        Serial.println("selector descending");
      }
    }
    Sequence(controlLeds);
    delay(500);
  }
}
/************************* + End Main loop + **************************/
/*************************** + Functions + ****************************/
void TurnOffLeds(sequenceControl controler){
  for (int16_t i = 0; i < controler.length; i++){
    digitalWrite(controler.leds[i], LOW);
  }
}
void Sequence(sequenceControl controler) {
  static int16_t pos = 0;                   // static: Entre llamadas a la función conserva su valor
  TurnOffLeds(controler);
  digitalWrite(controler.leds[pos], HIGH);
  if (controler.orientation == ascending) {
    // si es menor que len: aumentamos pos
    if (pos < controler.length - 1) {
      ++pos;
    } else {
      pos = 0;
    }
  } else {
    // si es mayor que cero vamos descendiendo
    if (pos > 0) {
      --pos;
    } else {
      pos = controler.length - 1;
    }
  }
}
bool ReadKey(int16_t key){
  if (digitalRead(key)){
    delay(50);          // antirebote
    // indicamos si se presionó la tecla:
    return digitalRead(key);          // devuelve 1 si se presiona
  } else{
    return 0;                         // devuelve 0 si no se presiona
  }
}
