/*
  Front Motor (Steering) => Channel A
 Back Motor => Channel B

 Since the motor shield hijacks 6 pins for the motors'
 control, they are declared in the MotorShieldR3 library.
 */

 #include <Adafruit_NeoPixel.h>

#define AiA 3 // PWM
#define AiB 2
#define BiA 5 // PWM
#define BiB 4
#define pinfrontLights    7    //Pin that activates the Front lights.
#define pinbackLights     4    //Pin that activates the Back lights.
char command = 'S';
int velocity = 255;
int speed = 0;
bool drvFlag = false;

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            7

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      6

#define BRIGHTNESS 50



int gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

uint16_t j;
int red;
int dir;
unsigned long oldtime;
int mode;
bool turboFlag = false;

#define DEFAULT 0
#define BABE 7

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(9600);  //Set the baud rate to that of your Bluetooth module.
  pinMode(pinfrontLights , OUTPUT);
  pinMode(pinbackLights , OUTPUT);
  pinMode(AiA, OUTPUT);
  pinMode(AiB, OUTPUT);
  pinMode(BiA, OUTPUT);
  pinMode(BiB, OUTPUT);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  j=0;
  red = 0;
  dir = 10;
  oldtime = millis();
  mode = DEFAULT;

}

void SA(){ // stop A
  digitalWrite(AiA,  LOW);
  digitalWrite(AiB,  LOW);
}
// void FA(){ // forward A
//   digitalWrite(AiA,  HIGH);
//   digitalWrite(AiB,  LOW);
// }
void FA(){ // forward A PWM
  analogWrite(AiA,  speed);
  digitalWrite(AiB,  LOW);
}
void RA(){ // reverse A
  analogWrite(AiA,  255-speed);
  digitalWrite(AiB,  HIGH);
}
void SB(){ // stop B
  digitalWrite(BiA,  LOW);
  digitalWrite(BiB,  LOW);
}
// void FB(){ // forward B
//   digitalWrite(BiB,  HIGH);
//   digitalWrite(BiA,  LOW);
// }
void FB(){ // forward B PWM
  digitalWrite(BiB,  HIGH);
  analogWrite(BiA,  255-speed);
}
void RB(){ // reverse B
  digitalWrite(BiB,  LOW);
  analogWrite(BiA,  speed);
}

void loop(){
  if(Serial.available() > 0){
    command = Serial.read();
    //Change pin mode only if new command is different from previous.
    //Serial.println(command);
    switch(command){
    case 'F':
      FA(); FB(); drvFlag = true;
      break;
    case 'B':
      RA(); RB(); drvFlag = true;
      break;
    case 'L':
      RB(); FA(); drvFlag = true;
      break;
    case 'R':
      FB(); RA(); drvFlag = true;
      break;
    case 'S':
      SB(); SA(); drvFlag = false;
      break;
    case 'I':  //FR
      SA(); FB(); drvFlag = true;
      break;
    case 'J':  //BR
      SA(); RB(); drvFlag = true;
      break;
    case 'G':  //FL
      FA(); SB(); drvFlag = true;
      break;
    case 'H':  //BL
      RA(); SB(); drvFlag = true;
      break;
    case 'W':  //Font ON

      break;
    case 'w':  //Font OFF

      break;
    case 'U':  //Back ON

      break;
    case 'u':  //Back OFF

      break;
    case 'V':  //Horn ON
    turboFlag = true;
      break;
    case 'v':  //Horn OFF
    turboFlag = false;
      break;
    case 'X':  //Extra ON
    mode = BABE;
      break;
    case 'x':  //Extra OFF
    mode = DEFAULT;
      break;
    case 'D':  //Everything OFF
      SB(); SA(); drvFlag = false;
      break;

    default:  //Get velocity
      if(command=='q'){
        velocity = 255;  //Full velocity

      }
      else{
        //Chars '0' - '9' have an integer equivalence of 48 - 57, accordingly.
        if((command >= 48) && (command <= 57)){
          //Subtracting 48 changes the range from 48-57 to 0-9.
          //Multiplying by 25 changes the range from 0-9 to 0-225.
          velocity = (command - 48)*25;

        }
      }
    }
  }

// timer loop
  if(millis() - oldtime > 20){
    oldtime = millis();
    switch (mode) {
      case DEFAULT:
      rainbow();
        break;

      case BABE:
      fade_red();
        break;
    }

    // acceleration ramping
    if(drvFlag){
      speed += 7;
      if(speed > velocity){
        speed = velocity;
      }
    }else{
      speed = 35;
    }

    if(turboFlag){
      speed = 255;
    }

}

  //
  // for(int i=0;i<NUMPIXELS;i++){
  //
  //   // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  //   pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
  //
  //   pixels.show(); // This sends the updated pixel color to the hardware.
  //
  //   //delay(delayval); // Delay for a period of time (in milliseconds).
  //
  // }
}

void fade_red(){
  red = red + dir;
  if (red > 255){
    red = 255;
    dir = -10;
  }else if(red < 0){
    red = 0;
    dir = 10;
  }


  for(int i=0; i<strip.numPixels(); i++) { // Update Pixels
    strip.setPixelColor(i, strip.Color(red,0,0));
  }
  strip.show();
}

void rainbow() {
  uint16_t i;

  j++;
  if(j>255){
    j = 0;
  }
  for(i=0; i<strip.numPixels(); i++) { // Update Pixels
    strip.setPixelColor(i, Wheel(((i*10)+j) & 255));
  }
  strip.show();
  //delay(20);

}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
