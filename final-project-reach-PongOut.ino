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

// note the Paddle class I've sketched out also acts as a Player class
class Paddle {
public:
  bool player;       
  int x, y;
  int rallies;
  int score;
  Color paddleColor;

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

 
  void getPaddleSpeed() {

  }

  void update() {
    draw()
  }

  int getPaddleX() {
    return x;
  }

  int getPaddleY() {
    return y;
  }

  void setY(int y_input) {
    y = y_input;
  }

  void draw() {
    // draw paddle (4 pixels tall)
    for (int i = 0; i < 4; i++) {
      matrix.drawPixel(x, y + i, paddleColor);
    }
  }

  void erase() {
    for (int i = 0; i < 4; i++) {
      matrix.drawPixel(x, y + i, BLACK);
    }
  }
};

class PongBall {

  public:
    // i legit don't know what to put here i'm rushing these just so I can finish my SI110 assignment kms
    PongBall() {
      // constructor code
    }

    void drawBall(int8_t x, int8_t y) {
      
    }

  private:
    int8_t x;
    int8_t y;
    bool isTouchingPaddle; // if it hit a paddle
    bool longEdgeCollision; // if the pongball hit left or right

    // member functions

    // cause the ball to bounce off the top, bottom edges, the paddles, and the breakout walls
    void bounce(bool direction) {
      // direction is 0 for X, 1 for y

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

class Game {

  public:

    Game() {
      // game constructor
    }

    void setupGame() {
      // self explanatory
    }

    void updateGame() {
      // self explanatory
    }


  private:


};

// Main Code
void setup() {
  // put your setup code here, to run once:

  // we need to initialize the LED board (basically)

}

void loop() {
  // put your main code here, to run repeatedly:

  // read potentiometer, then run game updates

}





// some end comments

// basically, we have it so it's like pong, except if you gain enough rally points, you can press the button
// this generates a wall of tiles so the opponent has to play breakout
// once they break through the wall, then it's pong again
// let's make it so that the wall of tiles generated is centered with the player's paddle