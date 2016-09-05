//Miata shift light!
//initialize libraries
#include <Bounce2.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6  //LED Data Pin
#define BUTTON_PIN     11
#define NUMPIXELS      8  //number of leds connected
Bounce debouncer = Bounce();
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int i;
int rpm;
float ighigh,iglow;
unsigned long igfreq, igcal1, igcal2;
int cnt;
int niday=HIGH;


void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(5);
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(115200);        // connect to the serial port

  Serial.println("Frequency Counter"); //this is jsut for debugging so we can see what freq the controller is reading
}

void loop() {
  //loop code, runs every cycle

  // Update the Bounce instance to see if button was pressed:
   debouncer.update();

   // Call code if Bounce fell (transition from HIGH to LOW) :
   if ( debouncer.fell() ) {

     // Toggle LED state if button was pressed :
     niday = !niday;


   }

  //measure period of tach signal
  ighigh = pulseIn(5,HIGH);
  iglow = pulseIn(5,LOW);

  igcal1=1000/((ighigh/1000)+(iglow/1000));

  //do it again
  ighigh = pulseIn(5,HIGH);
  iglow = pulseIn(5,LOW);

  igcal2=1000/((ighigh/1000)+(iglow/1000));
  //to filter out some noise, we only consider our measurement valid if they are similar in value, we accept the average
  if((igcal1-igcal2)<8)igfreq = (igcal1+igcal2)/2;

  //displaying measured freq through serial for debugging 
  Serial.print(cnt++);
  Serial.print("  Freq: ");
  Serial.println(igfreq);
  delay(1);


  //day mode LED setup
  //Progressive lights from 3000-6500, 6 green lights, then yellow, then blue.
  // @6500 all flashing blue
  // @7000 all flashing red 
  if(niday == HIGH){
    if(igfreq>=100 && igfreq<216){
      for( i=0;(i<NUMPIXELS)&&(i<=((igfreq-100)/16));i++){

        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        if(i <6){
          pixels.setPixelColor(i, pixels.Color(0,80,0)); // Moderately bright green color.
          pixels.show();
        }
        if(igfreq>196){
          pixels.setPixelColor(6, pixels.Color(50,50,0)); 
          pixels.show();
        }
        if(igfreq>212){
          pixels.setPixelColor(7, pixels.Color(0,0,100)); 
          pixels.show();
        }

      }
      for(i;(i<NUMPIXELS);i++){

          // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
          pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.

          pixels.show();
        }
    }
    else if(igfreq>=216 && igfreq<230){
      for( i=0;(i<NUMPIXELS);i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

        pixels.setPixelColor(i, pixels.Color(0,0,100)); 
        pixels.show();
        }
        delay(20);
        for( i=0;(i<NUMPIXELS);i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

          pixels.setPixelColor(i, pixels.Color(0,0,0)); 
          pixels.show();
        }
        delay(20);
    }
    else if(igfreq>=230){
      for( i=0;(i<NUMPIXELS);i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

        pixels.setPixelColor(i, pixels.Color(100,0,0)); 
        pixels.show();
        }
        delay(20);
        for( i=0;(i<NUMPIXELS);i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

          pixels.setPixelColor(i, pixels.Color(0,0,0)); 
          pixels.show();
        }
        delay(20);
    }
    else for(i=0;(i<NUMPIXELS);i++){

          // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
          pixels.setPixelColor(i, pixels.Color(0,0,0)); 

          pixels.show();
        }
  }


}
