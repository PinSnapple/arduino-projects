#include <TFT.h>
#include <SPI.h>

#define CS 10
#define DC 9
#define RESET 8

#define SPEED 4

// keep track of each player's score
int player1 = 0;
int oldPlayer1;
char player1Score[4];
int player2 = 0;
int oldPlayer2;
char player2Score[4];

// joysticks
int potPin1 = 2;
int potVal1 = 0;
int potPin2 = 5;
int potVal2 = 0;

int randNumber;

bool goalScored = false;

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
int xPaddlePosition1 = 4;
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

  beginPong();
}

void beginPong() {
  Serial.println("start!");
  Serial.println("player1 scores: ");
  Serial.println(player1);
  Serial.println("player2 scores: ");
  Serial.println(player2);
  
  myScreen.begin();
  myScreen.background(0,0,0); // clear the screen

  // starting screen
  myScreen.stroke(255,0,255);
  myScreen.setTextSize(5);
  myScreen.text("PONG", 23, 45);
  delay(1000);
  myScreen.background(0,0,0); // clear the screen
  delay(500);

  // draw division line
  myScreen.stroke(255,0,0);
  myScreen.line(80,0,80,128);

  // draw score
  myScreen.stroke(255,0,255);
  myScreen.setTextSize(2);
  String(player1).toCharArray(player1Score,4);
  myScreen.text(player1Score,50,0);
  String(player2).toCharArray(player2Score,4);
  myScreen.text(player2Score,102,0);

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
    xDir = SPEED;
  } else {
    xDir = -SPEED;
  }

  // randomly decide the direction of the ball (y-direction)
  randNumber = random(2);
  
  if (randNumber == 1) {
    yDir = SPEED;
  } else {
    yDir = -SPEED;
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
  if (yPos >= 125 || yPos <= 0) {
    yDir = yDir*-1;
  }

  // if the x and y position of the ball is the same as a paddle1, reverse direction
  if (xPos == xPaddlePosition1 && ((yPaddlePosition1-3) <= yPos && yPos <= (yPaddlePosition1 + 20))) {
    xDir = xDir*-1;
    yDir = yDir*-1;
  }

  // if the x and y position of the ball is the same as a paddle2, reverse direction
  if (xPos == xPaddlePosition2 && ((yPaddlePosition2-3) <= yPos && yPos <= (yPaddlePosition2 + 20))) {
    xDir = xDir*-1;
    yDir = yDir*-1;
  }
}

void updateScore() {
  // erase old score
  myScreen.stroke(0,0,0); // set black
  myScreen.setTextSize(2);
  String(oldPlayer1).toCharArray(player1Score,4);
  myScreen.text(player1Score,50,0);
  String(oldPlayer2).toCharArray(player2Score,4);
  myScreen.text(player2Score,102,0);
  
  // draw score
  myScreen.stroke(255,0,255);
  myScreen.setTextSize(2);
  String(player1).toCharArray(player1Score,4);
  myScreen.text(player1Score,50,0);
  String(player2).toCharArray(player2Score,4);
  myScreen.text(player2Score,102,0);

    // draw division line
  myScreen.stroke(255,0,0);
  myScreen.line(80,0,80,128);
}

void newRound() {

  xPos = 80;
  yPos = 64;

  // randomly decide the direction of the ball (x-direction)
  randNumber = random(2);

  if (randNumber == 1) {
    xDir = SPEED;
  } else {
    xDir = -SPEED;
  }

  // randomly decide the direction of the ball (y-direction)
  randNumber = random(2);
  
  if (randNumber == 1) {
    yDir = SPEED;
  } else {
    yDir = -SPEED;
  }
}

void checkForGoal() {
  if (xPos < 0) {
    oldPlayer1 = player1;
    player1 = player1+1;
    goalScored = true;
  } else if (xPos > 160) {
    oldPlayer2 = player2;
    player2 = player2+1;
    goalScored = true;
  }

  if (goalScored) {
    if (player1 >= 5 || player2 >= 5) {
      gameOver();
    } else {
      newRound();
    }
  }
  goalScored = false;
}

void gameOver() {
  Serial.println("game over!");
  if (player1 >= 5) {
    myScreen.background(0,0,0); // clear the screen
    myScreen.stroke(255,0,255);
    myScreen.setTextSize(2);
    myScreen.text("Player1 Wins!", 4, 45);
    delay(1000);
    myScreen.background(0,0,0); // clear the screen
    delay(500);
  } else {
    myScreen.background(0,0,0); // clear the screen
    myScreen.stroke(255,0,255);
    myScreen.setTextSize(3);
    myScreen.text("Player2 Wins!", 4, 45);
    delay(1000);
    myScreen.background(0,0,0); // clear the screen
    delay(500);
  }
  
  // ball starts in the middle of the screen
  xPos = 80;
  yPos = 64;
  
  // keep track of each player's score
  player1 = 0;
  oldPlayer1 = 0;
  player1Score[4];
  player2 = 0;
  oldPlayer2 = 0;
    
  // vars to keep track of the point's location
  xPrev = xPos;
  yPrev = yPos;

  beginPong();
}

void loop() {
  ballPosition();
  paddle1Position();
  paddle2Position();
  checkForCollision();
  checkForGoal();
  updateScore();
  
  // a 33ms delay means the screen updates 30 times a second
  delay(33);
}
