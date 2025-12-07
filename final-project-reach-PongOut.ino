#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>
#define FULLSCREEN_REFRESH
// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;
// define the wiring of the inputs

// for p1
const int POTENTIOMETER_A_PIN_NUMBER = 5;
const int BUTTON_A_PIN_NUMBER = 10;

// for p2
const int POTENTIOMETER_B_PIN_NUMBER = 5; // CHANGE THIS
const int BUTTON_B_PIN_NUMBER = 10; // CHANGE THIS

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();
class Color {
  public:
    byte red;
    byte green;
    byte blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(byte r, byte g, byte b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

/*  
    position as a variable type rather than individual xy
    Note that the positions are unsigned integers, so you cannot use "< 0" as a check 
    If you prefer though, we can just use int8_t x and int8_t y in place of this union... it might be better
*/


const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0); 
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);

long globalTime = 0;
// Constants declared above: DO NOT EDIT

// written by Reed
// note the Paddle class I've sketched out also acts as a Player class
class Paddle {
public:
  bool player;       
  int x, y;
  int rallies;
  int score;
  Color paddleColor;

  // default constructor to make PongBall work. NEVER CALL THIS OTHERWISE
  Paddle() {
    // defaults to 0
    player = 0;

    // Set paddle colors
    if (player == 0) { 
    paddleColor = RED;      // left paddle
    }
    
    else {   
    paddleColor = BLUE;   // right paddle
    }

    score = 0;
    rallies = 0;

    resetPos();
  }

  Paddle(bool p) {
    player = p;

    // Set paddle colors
    if (player == 0) { 
    paddleColor = RED;      // left paddle
    }
    
    else {   
    paddleColor = BLUE;   // right paddle
    }

    score = 0;
    rallies = 0;

    resetPos();
  }

  void resetPos() {
    // 32 x 16 matrix
    if (player) {
    x = 29;
    }   
    else {
    x = 2;
    }          // P2 on the right, P1 on the left
    y = 6;                           // centered on 16-pixel height
  }

  void incrementPoint() {
    score++;
  }

  void incrementRally() {
    rallies++;
  }

 
  int16_t getPaddleSpeed() {
    return y_difference;
  }

  void update() {
    draw();
  }

  int getPaddleX() {
    return x;
  }

  int getPaddleY() {
    return y;
  }

  // this function takes in the raw input from the potentiometer
  void setY(int y_input) {
    y_difference = getDifference(y_input >> 1);
    rawY = y_input >> 1; // 512 positions (to reduce noise)
    y = y_input >> 5; // division by 32
  }

  void draw() {
    // draw paddle (4 pixels tall)
    for (int i = 0; i < 4; i++) {
      matrix.drawPixel(x, y + i, paddleColor.to_333());
    }
  }

  void erase() {
    for (int i = 0; i < 4; i++) {
      matrix.drawPixel(x, y + i, BLACK.to_333());
    }
  }

  private:
  // for gauging speed with higher precision
  int rawY;
  int y_difference;

  int16_t getDifference(int y_org) {
    return (y_org - rawY);
  }
};

// written by Jo
class PongBall {

  public:
    // default constructor. Should not be called (generally)
    PongBall() {
      x = 0;
      y = 0;
      isTouchingPaddle = false;
      longEdgeCollision = false;
      x_vel = 0;
      y_vel = 0;
    }

    
    PongBall(int8_t xSpeed, int8_t ySpeed) {
      x = 0;
      y = 0;
      isTouchingPaddle = false;
      longEdgeCollision = false;
      x_vel = xSpeed;
      y_vel = ySpeed;
    }

    bool getHitLREdge() {
      return longEdgeCollision;
    }

    bool paddleCollision() {
      bool temp = isTouchingPaddle;
      isTouchingPaddle == false;
      return temp;
    }

  private:
    int8_t x;
    int8_t y;
    // x,y velocity are scaled by 8 (so a value of 8 moves 1 pixel per tick update?)
    // I know this isn't the proper way to do things, but it'll do cus I'm lazy
    // note currently, x_vel and y_vel are not influenced by paddle velocity
    int8_t x_vel;
    int8_t y_vel;

    Paddle p1;
    Paddle p2;

    bool isTouchingPaddle; // if it hit a paddle
    bool longEdgeCollision; // if the pongball hit left or right

    // for bounce detection
    bool paddle;
    int8_t presentPaddleX;
    int8_t presentPaddleY;
    int16_t paddleSpeed;

    // member functions

    void move() {
      erase(x, y);
      // division is taxing, so we're using the fastest operator: bit shifting!!!
      x += (x_vel >> 4) + 1;
      y += (y_vel >> 4) + 1;

      // check for collisions?
      
      // PLEASE IMPLEMENT THESE FUNCTIONS
      // this current code doesn't factor in speed of the paddle
      paddle = (x >> 4);
      if (paddle) {
        // let paddle size be 4 pixels tall

        presentPaddleX = p1.getPaddleX();
        presentPaddleY = p1.getPaddleY();
        paddleSpeed = p1.getPaddleSpeed();

        if (x >= presentPaddleX - 1 && x <= presentPaddleX) {
          if (y >= presentPaddleY && y <= presentPaddleY + 3) {
            bounce(true, true); // true is horz bounce
          }
        }

      } else {
        // let paddle size be 4 pixels tall

        presentPaddleX = p2.getPaddleX();
        presentPaddleY = p2.getPaddleY();
        paddleSpeed = p2.getPaddleSpeed();

        if (x >= presentPaddleX && x <= presentPaddleX + 1) {
          if (y >= presentPaddleY && y <= presentPaddleY + 3) {
            bounce(true, true); // true is horz bounce
          }
        }
      }

      // end of plea of help
      // so basically, we need p1, p2 (as global objects of Paddle class), getPaddleX, getPaddleY

      // check edge conditions
      if (y <= 0 || y >= 15) {
        bounce(false, false);
      }

      longEdgeCollision = (x <= 0 || y >= 31);

      //
      drawBall(x, y);
    }

    // cause the ball to bounce off the top, bottom edges, the paddles, and the breakout walls
    void bounce(bool direction, bool hitPaddle) {

      // the y velocity of the ball depends on the paddle's speed
      if (direction) {
        x_vel *= -1;
      } else {
        if (hitPaddle) {
          y_vel += (paddleSpeed >> 2); 
        } else {
          y_vel *= -1;
        }
      }
      
      // clamp velocities at 128 (after recalculation, that means the ball will move 9 pixels/tick)
      if (x_vel >> 7) {
        x_vel = 127;
      }

      if (y_vel >> 7) {
        y_vel = 127;
      }
      
      // direction is 0 for X, 1 for y

    }

    void drawBall(int8_t x, int8_t y) {
      matrix.drawPixel(x, y, WHITE.to_333());
    }

    void erase(int8_t x, int8_t y) {
      matrix.drawPixel(x, y, BLACK.to_333());
    }



};

// let's make these tiles 2 wide, 4 tall (pixels)
class BreakoutTile {

  public:

    BreakoutTile() {
      // default constructor
    }

    // when the wall is hit by the pong ball, reduce its health by 1
    void wallHit() {
      // code
    }

    // generates a wall of tiles 3 tiles tall, 2 tiles thick
    void generateWall(bool player) {
      if (player) {

      } else {

      }
    }


  private:
    int8_t x;
    int8_t y;
    int8_t health;
    


};

// written by Brenna
class Game {

  public:

    Game() {
      // game constructor
    }

    Game(Paddle p1, Paddle p2) {
      // game constructor
      player1 = p1;
      player2 = p2;
    }

    void setupGame() {
      // self explanatory
    }

    void updateGame() {
      // self explanatory
    }


  private:

    Paddle player1;
    Paddle player2;

    bool checkWinner(Paddle p1, Paddle p2) {
      return true; // dud code
    }

};  

class Menu {

  public:

    Menu() {

    }

    // basically a while loop that runs until a menu is selected
    // should allow for scrolling menus (if needed)
    // text can be 8 pixels tall
    // we can later decide how many menu choices, but I'm thinking currently 2: singleplayer and multiplayer
    // if we have the time, we could do singleplayer endless
    void runMenu() {

    }

  private:
    uint8_t menuPosition;

    uint8_t getMenuChoice() {
      return 0; // temporary
    }

    // should only run when the potentiometer value changes or button pressed
    void updateVisibleMenu() {

    }
    
    // if the line is too long, scroll text
    // rate is ms of delay between each character scrolling
    void scrollText(int rate) {

    }

    void printLine(bool highlighted) {
      // should use printChar()
      // if highlighted, use white bg, else black bg
    }

    // let text
    void printChar() {
      // should print out a character
    }



    void erase() {
      matrix.fillScreen(BLACK.to_333());
    }

};



// creating a game object
// TEMPORARY until Menu class is setup

// Main Code
void setup() {
  // put your setup code here, to run once:

  // creating two player classes (global variables)
  Paddle player1 = Paddle(0);
  Paddle player2 = Paddle(1);

  Serial.begin(9600);
  pinMode(BUTTON_A_PIN_NUMBER, INPUT);
  pinMode(BUTTON_B_PIN_NUMBER, INPUT);
  matrix.begin();
  title();
  game.setupGame();

  // we need to initialize the LED board (basically)

}

void loop() {
  // put your main code here, to run repeatedly:

  int p1PotVal = analogRead(POTENTIOMETER_A_PIN_NUMBER);
  int p2PotVal = analogRead(POTENTIOMETER_B_PIN_NUMBER);
  bool button_A_pressed = (digitalRead(BUTTON_A_PIN_NUMBER) == HIGH);
  bool button_B_pressed = (digitalRead(BUTTON_B_PIN_NUMBER) == HIGH);
  game.update(potentiometer_value, button_pressed);

  // read potentiometer, then run game updates

}





// some end comments

// basically, we have it so it's like pong, except if you gain enough rally points, you can press the button
// this generates a wall of tiles so the opponent has to play breakout
// once they break through the wall, then it's pong again
// let's make it so that the wall of tiles generated is centered with the player's paddle