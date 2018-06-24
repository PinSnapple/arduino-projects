#include <TFT.h> // Hardware-specific library
#include <SPI.h>

#define CS   10
#define DC   9
#define RESET  8  

// pin definition for the Leonardo
// #define CS   7
// #define DC   0
// #define RESET  1 

TFT myScreen = TFT(CS, DC, RESET);

// variable to keep track of the elapsed time
int counter = 0;
// char array to print time
char printout[4];

void setup(){
  myScreen.begin();  
  myScreen.background(0,0,0); // clear the screen
  myScreen.stroke(255,0,255);
  // static text
  myScreen.text("Running for",0,0);
  myScreen.text("seconds",0,30);  
  // increase font size for text in loop() 
  myScreen.setTextSize(3);
}

void loop(){
    // get elapsed time
    counter = millis();
    // convert to a string
    String elapsedTime = String(counter/1000);
    // add to an array
    elapsedTime.toCharArray(printout,4);
    // print out and erase
    myScreen.stroke(255,255,255);
    myScreen.text(printout,0,10);
    delay(1000);
    myScreen.stroke(0,0,0);
    myScreen.text(printout,0,10);
}
