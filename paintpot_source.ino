// ----------------------------- PaintPot ----------------------------
// |                 Copyright (C) 2018 - Yuriy Murenko              |
// |                   free for non-commercial use                   |
// |                          www.murenko.com                        |
// |-----------------------------------------------------------------|
// | An Arduino Leonardo powered USB peripheral for digital painting |
// | productivity. Allows the size of the brush, the brush opacity,  |
// | and flow rate (or stroke smoothing) to be controlled by turning |
// | knobs (potentiometers and a rotary encoder).                    |
// |-----------------------------------------------------------------|

#include<Keyboard.h>;
//digital pin assignments
const int pinA = 2;  // Connected to CLK on KY-040
const int pinB = 4;  // Connected to DT on KY-040
const int pinC = 8;
// ----------------

//digital read values from rotary encoder
int aVal;
int bVal;
// ----------------

//last read values from rotary encoder
int pinALast;
int pinBLast;
// ----------------

//constant values for detecting forward rotation of encoder
//don't read from pins because rapid motion 
const int pinAf = LOW;
const int pinBf = HIGH;
// ----------------

//variables for detecting backward rotation of encoder
int pinAr;
int pinBr;
// ----------------

//analog read values from potentiometers
//compare with potLast values to calculate rotation
int potA;
int potB;
// ----------------

//last analog read from the potentiometers
//used for detecting rotation 
int potLast;
int potLastB;
// ----------------

//0x82 - alt key (smoothing in Photoshop)
//0x81 - shift key (flow rate in Photoshop)


void setup() {
  //setup for digital pin signal
  pinMode (pinA, INPUT_PULLUP);
  pinMode (pinB, INPUT_PULLUP);
  pinMode (pinC, INPUT_PULLUP);
  // ----------------
  
  //initialize the last pin A value for rotation detection
  pinALast = digitalRead(pinA);
  // ----------------
  
  //begin keyboard simulation
  //this device behaves as a keyboard
  Keyboard.begin();
  // ----------------
  
  //initialize the "last" values for potentiometer rotation detection
  potLast = analogRead(0);
  potLastB = analogRead(3);
  // ----------------
}

void loop() {
  //read the values for the encoder pins
  aVal = digitalRead(pinA);
  bVal = digitalRead(pinB);
  // ----------------
  
  //read the values for the potentiometer pins
  potA = analogRead(0);
  potB = analogRead(3);
  // ----------------
  
  //detection of potentiomter motion
  // ----------------
  //Note: there is considerable jitter in the resistance reading
  //from the potentiometers as the contacts tarnish
  //the +/- 9 values should account for the jitter
  // ----------------
  //potA controls the opacity of the brush
  if (potA > potLast + 9 || potA < potLast - 9) {
    if (potA / 10 < 10||potA>1010) {
      potLast = potA;
    }
    else if (potA / 10 > 100) {
      delay(10);
      Keyboard.print("100");
      potLast = potA;
    }
    else {
      delay(10);
      Keyboard.print(potA / 10);
      potLast = potA;
    }
  }
  //potB controls the flow of the brush
  //normally this is done by holding shift and entering a number
  if (potB > potLastB + 9 || potB < potLastB - 9) {
    if (potB>1010) {
      potLastB = potB; 
    }
    else if (potB / 10 > 100) {
      //a delay is necessary after the key press otherwise windows
      //does not properly register the following keyboard output
      Keyboard.press(0x81);
      delay(10);
      Keyboard.print("100");
      potLastB = potB;
    }
    /* -- Use the following block only for smoothing option -- 
      else if(potB<100){ 
      Keyboard.press(0x81);
      delay(10);
      Keyboard.print("00");
      potLastB = potB;
      delay(100);
    }--------------------------------------------------------*/
    else{
      Keyboard.press(0x81);
      delay(10);
      Keyboard.print(potB/10);
      potLastB = potB;
    }
    delay(10);
    //keys must be released, as the keyboard.press function does not trigger a release
    Keyboard.releaseAll();
  }
  //Rotary encoder button is pressed -> change brush hardness
  if (digitalRead(pinC)==LOW){
    //Rotation is occuring
    if (aVal != pinALast) {
        //Forward rotation
        if (digitalRead(pinB)!=aVal) {
          if (digitalRead(pinA)!=pinBf && digitalRead(pinB)!=pinAf){
            Keyboard.print("}");
          }
        }
        //Backward rotation
        else if(digitalRead(pinA)==pinAr && digitalRead(pinB)==pinBr){
          Keyboard.print("{");
          pinAr = digitalRead(pinA);
          pinBr = digitalRead(pinB);
        }
      }
     
  pinALast = aVal;
  }
  //Rotary encoder button is NOT pressed -> change brush size
  if (digitalRead(pinC)==HIGH){
    //Rotation is occuring
    if (aVal != pinALast) {
        //Forward rotation
        if (digitalRead(pinB)!=aVal) {
          if (digitalRead(pinA)!=pinBf && digitalRead(pinB)!=pinAf){
            Keyboard.print("]");
          }
        }
        //Backward rotation
        else if(digitalRead(pinA)==pinAr && digitalRead(pinB)==pinBr){
          Keyboard.print("[");
          pinAr = digitalRead(pinA);
          pinBr = digitalRead(pinB);
        }
      }
  pinALast = aVal;
  }
}


