#include <TFT.h>
#include <SPI.h>

#define CS 10
#define DC 9
#define RESET 8

// joysticks
int potPin1 = 2;
int potVal1 = 0;
int potPin2 = 5;
int potVal2 = 0;

int randNumber;

TFT myScreen = TFT(CS, DC, RESET);
// screen width goes from 0 to 160
// screen height goest from 0 to 128

// ball starts in the middle of the screen
int xPos = 80;
int yPos = 64;

// direction and speed of ball
int xDir = 0;
int yDir = 0;

// vars to keep track of the point's location
int xPrev = xPos;
int yPrev = yPos;

// vars for paddle locations
int xPaddlePosition1 = 5;
int xPaddlePosition2 = 152;
int yPaddlePosition1;
int yPaddlePosition2;
int yPadPrev1;
int yPadPrev2;

// mapping of joystick to paddle movement
float paddleMap = 0.1055718475;

void setup() {
  int screenWidth = myScreen.width(); 
  int screenHeight = myScreen.height();
  
  
  // for debugging
  Serial.begin(9600);
  
  Serial.print("screen width: ");
  Serial.println(screenWidth);
  Serial.print("screen height: ");
  Serial.println(screenHeight);
  
  myScreen.begin();
  myScreen.background(0,0,0); // clear the screen

  // draw the pong paddles
  potVal1 = analogRead(potPin1);
  yPaddlePosition1 = potVal1 * paddleMap;
  potVal2 = analogRead(potPin2);
  yPaddlePosition2 = potVal2 * paddleMap;
  
  myScreen.stroke(255, 255, 255); // set stroke to white
  myScreen.fill(255, 255, 255); // set fill to white
  myScreen.rect(xPaddlePosition1, yPaddlePosition1, 3, 20); // paddle1
  myScreen.rect(xPaddlePosition2, yPaddlePosition2, 3, 20); // paddle2
  
  yPadPrev1 = yPaddlePosition1;
  yPadPrev2 = yPaddlePosition2;

  // use random analog noise from pin 0 to generate seed numbers
  randomSeed(analogRead(0));

  // randomly decide the direction of the ball (x-direction)
  randNumber = random(2);

  if (randNumber == 1) {
    xDir = 1;
  } else {
    xDir = -1;
  }

  // randomly decide the direction of the ball (y-direction)
  randNumber = random(2);
  
  if (randNumber == 1) {
    yDir = 1;
  } else {
    yDir = -1;
  }
}

void ballPosition() {
  // update the location of the ball
  xPos = xPos + xDir;
  yPos = yPos + yDir;

  // check if the current location is different than the previous
  if (xPos != xPrev || yPos != yPrev) {
    myScreen.stroke(0,0,0); // set the stroke color to black 
    myScreen.fill(0,0,0); // set the fill color to black
    myScreen.rect(xPrev, yPrev, 3, 3); // color in the previous ball 
  }

  // draw the ball in the current location
  myScreen.stroke(255,255,255);
  myScreen.fill(255, 255, 255); 
  myScreen.rect(xPos, yPos, 3, 3);
  
  // update the point's previous location
  xPrev=xPos;
  yPrev=yPos;
}

void paddle1Position() {
  potVal1 = analogRead(potPin1);
  yPaddlePosition1 = potVal1 * paddleMap;
  
  // check if paddle1 position moved
  if (yPaddlePosition1 != yPadPrev1) {
    myScreen.stroke(0,0,0); // set the stroke color to black 
    myScreen.fill(0,0,0); // set fill color to black
    myScreen.rect(xPaddlePosition1, yPadPrev1, 3, 20); // fill in old paddle black
  }

  // redraw paddle1 in it's new location
  myScreen.stroke(255, 255, 255); // set stroke to white
  myScreen.fill(255,255,255); // set fill color to white
  myScreen.rect(xPaddlePosition1, yPaddlePosition1, 3, 20); // paddle1

  // set current y position to previous y position
  yPadPrev1 = yPaddlePosition1;
}

void paddle2Position() {
  potVal2 = analogRead(potPin2);
  yPaddlePosition2 = potVal2 * paddleMap;
  
  // check if paddle2 position moved
  if (yPaddlePosition2 != yPadPrev2) {
    myScreen.stroke(0,0,0); // set the stroke color to black 
    myScreen.fill(0,0,0); // set fill color to black
    myScreen.rect(xPaddlePosition2, yPadPrev2, 3, 20); // fill in old paddle black
  }

  // redraw paddle2 in it's new location
  myScreen.stroke(255, 255, 255); // set stroke to white
  myScreen.fill(255,255,255); // set fill color to white
  myScreen.rect(xPaddlePosition2, yPaddlePosition2, 3, 20); // paddle2

  // set current y position to previous y position
  yPadPrev2 = yPaddlePosition2;
}

void checkForCollision() {
  
  // if the y position of the ball is at the screen edge, reverse direction
  if (yPos >= 128 || yPos <= 0) {
    yDir = yDir*-1;
  }

  // if the x and y position of the ball is the same as a paddle1, reverse direction
  if (xPos == xPaddlePosition1 && (yPaddlePosition1 <= yPos && yPos <= (yPaddlePosition1 + 20))) {
    xDir = xDir*-1;
    yDir = yDir*-1;
  }

  // if the x and y position of the ball is the same as a paddle2, reverse direction
  if (xPos == xPaddlePosition2 && (yPaddlePosition2 <= yPos && yPos <= (yPaddlePosition2 + 20))) {
    xDir = xDir*-1;
    yDir = yDir*-1;
  }
}

void loop() {
  ballPosition();
  paddle1Position();
  paddle2Position();
  checkForCollision();
  // a 33ms delay means the screen updates 30 times a second
  delay(33);
}
