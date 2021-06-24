//#include "LowPower.h"
#include <Adafruit_NeoPixel.h>

#define N_LEDS 20  //adjust this number to reflect the number of LEDs in your LED strip
#define PIN      10
const int trigPin = 12;
const int echoPin = 11;

const int potPin = 0;    // select the input pin for the potentiometer
int val = 0;       // variable to store the value coming from the sensor

int entering = 300; //turn lights to magenta
int approaching = 200; //turnlights to green
int safedistance = 145; //turn lights to yellow
int normalstop = 45; //turn lights to red
int emergencystop = 10; //flashes warning lights

int wait = 10*1000;  //#of seconds to wait before turning off 
                     //(multiplied by 1000 to convert to milliseconds)
float gap = 2;  //noise threshold for detecting movement


Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t white = strip.Color(255, 255, 255); //white
uint32_t off = strip.Color(0, 0, 0);    //off
uint32_t bG = strip.Color(15, 255, 200); //blue-green
uint32_t purple = strip.Color(200, 5, 150); //purple 255, 15, 200
uint32_t red = strip.Color(255, 0, 0);  //red
uint32_t green = strip.Color(0, 255, 0);  //green
uint32_t blue = strip.Color(0, 0, 255);  //blue
uint32_t yellow = strip.Color(170, 200, 0);


long duration;
float distancecm;
float oldDistance = 0;

unsigned long changeTime;

boolean noChange = false;
boolean sleepTime = false;
boolean even = true;
int middle = 0;
int range;
int adj = 1;
int adj2 = 0;

void setup() {
  pinMode (trigPin, OUTPUT);
  pinMode (echoPin, INPUT);
  middle = N_LEDS / 2;
  if (N_LEDS % 2 == 1) {
    even = false;
  }
  if (even == true) {
    adj = 1;
    adj2 = 0;
  } else {
    adj = 0;
    adj2 = 1;
  }
  Serial.begin(9600);
  strip.begin();



}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distancecm = duration * .0342 / 2;
  val = analogRead(potPin);    // read the value from the sensor
  Serial.println(distancecm);
  //Serial.println(val);
  delay(100);
  sleep();
 
}

void sleep(){
 
  if (distancecm >= oldDistance) {
    if (distancecm - oldDistance <= gap) {
      noChange = true;
    } else {
      noChange = false;
    }
  } else {
    if (oldDistance - distancecm <= gap) {
      noChange = true;
    } else {
      noChange = false;
    }
  }
  if (noChange == true) {
    if (millis() - changeTime > wait) {
      sleepTime = true;
    }
  } else {
    sleepTime = false;
  }

  if (distancecm > entering || sleepTime == true) {
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, off);
    }
    strip.show();
    //LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
  } else {
    colorChange2();
    if (noChange == false) {
      changeTime = millis();
      oldDistance = distancecm;
    }

  }

}

void colorChange2() {

  if (distancecm <= (entering+(val/10)) && distancecm > (approaching+(val/10))) {
    range = map(distancecm, (entering+(val/10)), (approaching+(val/10)),  0, middle+adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, off);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, purple);
      strip.setPixelColor(middle - adj -i, purple);
    }

  } else if (distancecm <= (approaching+(val/10)) && distancecm > (safedistance+(val/10))) {
    range = map(distancecm, (approaching+(val/10)), (safedistance+(val/10)), 0, middle+adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, purple);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, green);
      strip.setPixelColor(middle - adj -i, green);
    }
  } else if (distancecm <= (safedistance+(val/10)) && distancecm > (normalstop+(val/10))) {
    range = map(distancecm, (safedistance+(val/10)), (normalstop+(val/10)), 0, middle+adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, green);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, yellow);
      strip.setPixelColor(middle - adj -i, yellow);
    }
  } else if (distancecm <= (normalstop+(val/10)) && distancecm > (emergencystop+(val/10))) {
    range = map(distancecm, (normalstop+(val/10)), (emergencystop+(val/10)), 0, middle+adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, yellow);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, red);
      strip.setPixelColor(middle - adj -i, red);
    }
  } else if(distancecm < (emergencystop-5+(val/10))){
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, white);
    }
    strip.show();
    delay(300);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, red);
    }
    strip.show();
    delay(300);
  }
  strip.show();
}


uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
