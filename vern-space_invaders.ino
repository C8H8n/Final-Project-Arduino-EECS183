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
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;
// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;
const uint8_t NUM_ENEMIES_ROW = 8;
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
const Color BLACK(0, 0, 0);
const Color RED(4, 1, 1); // faulty display issues (originally [4, 0, 0])
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);
class Invader {
  public:
    // Constructors
   Invader() {
      x = 0;
      y = 0;
      strength = 0;
    }
    // sets values for private date members x and y
    Invader(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg; 
    }
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_strength() const {
      return strength;
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    void move() {
       erase();
       y++;
      draw();
    }
    
    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
      Color temp = RED;
      Color eyes = BLUE;

      switch (get_strength()) {
        case 1:
          temp = RED;
          break;
        case 2:
          temp = ORANGE;
          break;
        case 3:
          temp = YELLOW;
          break;
        case 4:
          temp = GREEN;
          break;
        case 5:
          temp = BLUE;
          break;
        case 6:
          temp = PURPLE;
          break;
        case 7:
          temp = WHITE;
          break;
        default: 
          erase();
          return;
      }
      draw_with_rgb(temp, eyes);
    }

    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
     draw_with_rgb(BLACK, BLACK);
    }    
    
    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      erase();
      strength -= 1;
      draw();
    }

    bool isDead() {
      // nonzero values return false, else true
      return strength <= 0;
    }

    // implement has_hit_bottom()
    bool has_hit_bottom() const {
      return (y >= 11);
    }
    
    // implement has_been_hit()
    bool has_been_hit(int8_t ballX, int8_t ballY) {
      if (!isDead()) {
        if (ballY <= y+3) {
          if (ballX >= x && ballX <= x + 3) {
            hit();
            return true;
          }
        }
      }
      
      return false;
    }


  private:
    int8_t x;
    int8_t y;
    int8_t strength;
    
    // draws the Invader
    void draw_with_rgb(Color body_color, Color eye_color) {
      matrix.drawPixel((x + 1), y, body_color.to_333());
      matrix.drawPixel((x + 2), y, body_color.to_333());
      matrix.drawPixel(x, (y + 1), body_color.to_333());
      matrix.drawPixel((x + 3), (y + 1), body_color.to_333());
      matrix.drawPixel(x, (y + 2), body_color.to_333());
      matrix.drawPixel((x + 1), (y + 2), body_color.to_333());
      matrix.drawPixel((x + 2), (y + 2), body_color.to_333());
      matrix.drawPixel((x + 3), (y + 2), body_color.to_333());
      matrix.drawPixel(x, (y + 3), body_color.to_333());
      matrix.drawPixel((x + 3), (y + 1), body_color.to_333());
      matrix.drawPixel((x + 1), (y + 1), body_color.to_333());
      matrix.drawPixel((x + 2), (y + 1), body_color.to_333());
    }
};

class Cannonball {
  public:
    Cannonball() {
      x = 0;
      y = 0;
      fired = false;
      time = 0; 
    }

    // resets private data members to initial values
    void reset() {
      fired = false;
    }

    // getters
    inline uint8_t get_x() const {
      return x;
    }
    inline int get_y() const {
      return y;
    }
    bool has_been_fired() const {
      return fired;
    }

    // sets private data members
    void fire(int x_arg, int y_arg) {
      if (!fired) {
        x = x_arg;
        y = y_arg;
        fired = true;
      }
    }
    
    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      if (millis() > time + 10) {
        if (fired) {
          erase();
          y--;
          if (y < 0) {
            fired = false;
          }
          else {
            draw();
          }
        }

        time = millis();
      }
    }

    void hit_target() {
      fired = false;
      erase();
    }

  private:
    uint8_t x;
    int y;
    bool fired;
    long time; // declaring class local time

    // draws the Cannonball, if it is fired
    inline void draw() {
      matrix.drawPixel(x, y, ORANGE.to_333());
    }

    // draws black where the Cannonball used to be
    inline void erase() {
      matrix.drawPixel(x, y, BLACK.to_333());
    }
};

class Player {
  public:
    Player() {
      x = 0;
      y = 0;
      lives = 3;
      initialize(15, 15);
    }
    
    // getters
    inline uint8_t get_x() const {
      return x;
    }
    inline uint8_t get_y() const {
      return y;
    }
    uint8_t get_lives() const {
      return lives;
    }
    
    // setter
    void set_x(uint8_t x_arg) {
      x = x_arg;
    }
    
    // Modifies: lives
    void die() {
      lives--;
    }
    
    // draws the Player
    // calls: draw_with_rgb
    void draw() {
      erase();
      draw_with_rgb(WHITE);
    }
    
    // draws black where the Player used to be
    // calls: draw_with_rgb
    inline void erase() {
      //matrix.fillScreen(0);
        matrix.drawPixel(x-2, y, 0);
        matrix.drawPixel(x+2, y, 0);
        matrix.drawPixel(x-1, y, 0);
        matrix.drawPixel(x+1, y, 0);
        matrix.drawPixel(x-1, y-1, 0);
        matrix.drawPixel(x+1, y-1, 0);
        matrix.drawPixel(x, y-1, 0);
        matrix.drawPixel(x, y, 0);
    }
  private:
    uint8_t x;
    uint8_t y;
    int lives;
    // sets private data members x and y to initial values
    void initialize(uint8_t x_arg, uint8_t y_arg) {
      x = x_arg;
      y = y_arg;
    }
    
    // draws the player
    void draw_with_rgb(Color color) {
      matrix.drawPixel(x, y, color.to_333());
      matrix.drawPixel(x-1, y, color.to_333());
      matrix.drawPixel(x+1, y, color.to_333());
      matrix.drawPixel(x, y-1, color.to_333());
    }
};

class Game {
  public:
    Game() {
      level = 1;
      time = 0;
      
    }

    void restart_player() {
      player = Player();
    }

    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    void setupGame() {
      // Cannonball cannonball;
      // Player player;
      print_level(level);
      delay(2000);
      print_lives(player.get_lives());
      delay(2000);
      matrix.fillScreen(0b000000000); // fills screen black
      int x_arg = 0;
      int y_arg = 0;

      // THIS HERE IS COMPLETELY OPTIONAL

      // this is the power for each enemy
      // rows are handled such that elements 0-7 are the top row, elements 8-15 are the bottom row
      // fixed level arrays for levels 1-4
      // inline const int8_t[NUM_ENEMIES] LEVEL1 {
      //   1, 1, 1, 1, 1, 1, 1, 1
      // }

      // inline const int8_t[NUM_ENEMIES] LEVEL2 {
      //   1, 2, 1, 2, 1, 2, 1, 2,
      //   2, 1, 2, 1, 2, 1, 2, 1
      // }

      // inline const int8_t[NUM_ENEMIES] LEVEL3 {
      //   1, 2, 1, 2, 1, 2, 1, 2,
      //   2, 1, 2, 1, 2, 1, 2, 1
      // }

      // switch statement using inline const arrays
        // insert code lmao

      if (level == 1) {
        for (int i = 0; i < 8; i++) {
          int power = 1;
          enemies[i].initialize(x_arg, y_arg, power);
          enemies[i].draw();
          x_arg = x_arg + 4;
        }

      }
      else if (level == 2) {
        for (int i = 0; i < 7; i = i + 2) {
          enemies[i].initialize(x_arg, y_arg, 1);
          enemies[i].draw();
          x_arg = x_arg + 4;
          enemies[i + 1].initialize(x_arg, y_arg, 2);
          enemies[i + 1].draw();
          x_arg = x_arg + 4;
        }
        y_arg = 5;
        x_arg = 0;

        for (int i = 8; i < NUM_ENEMIES - 1; i = i + 2) {
          enemies[i].initialize(x_arg, y_arg, 2);
          enemies[i].draw();
          x_arg = x_arg + 4;
          enemies[i].initialize(x_arg, y_arg, 1);
          enemies[i + 1].draw();
          x_arg = x_arg + 4;
        }
      }
      else if (level == 3) {
        x_arg = 0;
        y_arg = 0;
        int level3[2][8] = {{1, 2, 3, 4, 5, 1, 2, 3}, {4, 5, 1, 2, 3, 4, 5, 1}};
        for (int i = 0; i < 2; i++) {
          for (int j = 0; j < 8; j++) {
            enemies[i * 8 + j].initialize(x_arg, y_arg, level3[i][j]);
            enemies[i * 8 + j].draw();
            x_arg = x_arg + 4;
          }
          x_arg = 0;
          y_arg = y_arg + 5;
        }
      }
      else if (level == 4) {
        x_arg = 0;
        y_arg = 0;
        int level4[2][8] = {{5, 4, 5, 4, 5, 4, 5, 4}, {2, 3, 2, 3, 2, 3, 2, 3}};
        for (int i = 0; i < 2; i++) {
          for (int j = 0; j < 8; j++) {
            enemies[i * 8 + j].initialize(x_arg, y_arg, level4[i][j]);
            enemies[i * 8 + j].draw();
            x_arg = x_arg + 4;
          }
          y_arg = y_arg + 5;
          x_arg = 0;
        }
      }
      else {
        x_arg = 0;
        y_arg = 0;
        int levelExtra[2][8];
        randomSeed(micros());
        for (int i = 0; i < 2; i++) {
          for (int j = 0; j < 8; j++) {
            levelExtra[i][j] = random(1, 8);
          }
        }
        for (int i = 0; i < 2; i++) {
          for (int j = 0; j < 8; j++) {
            enemies[i * 8 + j].initialize(x_arg, y_arg, levelExtra[i][j]);
            enemies[i * 8 + j].draw();
            x_arg = x_arg + 4;
          }
          y_arg = y_arg + 5;
          x_arg = x_arg + 1;
        }
      }

    }

    // advances the game simulation one step and renders the graphics
    // see spec for details of game
    // Modifies: global variable matrix
    void update(int potentiometer_value, bool button_pressed) {
      if (player.get_lives() <= 0)
      {
        game_over();
        delay(1000);
        level = 1;
        restart_player();
        setupGame();
      }

      player.erase();
      player.set_x((matrix.width() * potentiometer_value) / 1024 );
      player.draw();

      if(button_pressed && !ball.has_been_fired()) {
        ball.fire(player.get_x(), 14);
      }
      else {

        // === Cannonball Updates
        ball.move();
        

        // wait please check this because right now you can't shoot behind the lines
        if (bottom_row_cleared()) {
          for (uint8_t i = 0; i < NUM_ENEMIES_ROW; i++) {
            // this is the cannonball collision detection
            if (ball.has_been_fired()) {
              if (enemies[i].has_been_hit(ball.get_x(), ball.get_y())) {  
                ball.hit_target();
              }
            }
          }
        } else {
          for (uint8_t i = NUM_ENEMIES_ROW; i < NUM_ENEMIES; i++) {
            // this is the cannonball collision detection
            if (ball.has_been_fired()) {
              if (enemies[i].has_been_hit(ball.get_x(), ball.get_y())) {  
                ball.hit_target();
              }
            }
          }
        }

        // === Invader Updates

        // we run the onscreen/mechanic updates ~2000ms
        if (millis() >= time + 2000) {
          // for loop is embedded into the if to increase efficiency
          // less updates when for loop iterates only through 8
          if (bottom_row_cleared()) {
            for (uint8_t i = 0; i < NUM_ENEMIES_ROW; i++) {
            
              // game movement tick
              if (enemies[i].get_strength() > 0) {
                if (i >= 8) {
                  enemies[i].move();
                }
                if ((i < 8) && (bottom_row_cleared())) {
                  enemies[i].move();
                }
              }

            }
          } else {
            for (uint8_t i = NUM_ENEMIES_ROW; i < NUM_ENEMIES; i++) {
            
              // game movement tick
              if (enemies[i].get_strength() > 0) {
                if (i >= 8) {
                  enemies[i].move();
                }
                if ((i < 8) && (bottom_row_cleared())) {
                  enemies[i].move();
                }
              }

            }
          }

          // tick update
          time = millis();

        }
          
          // it seems here, the player dies either when the enemies have hit the bottom or hit the player. However, since the shoot can only shoot from a set height
          // we only need to check "has hit bottom" right?
          // I'm too lazy rn to make this a conditional that checks only the bottom or top rows
          for (uint8_t i = 0; i < NUM_ENEMIES; i++) {
            // death condition
            if (enemies[i].has_hit_bottom()) {
              player.die(); // player dies when this condition happens
              matrix.fillScreen(BLACK.to_333());
              matrix.setCursor(1,0);
              matrix.println("Lost");
              matrix.print("Life");
              delay(2000);
              
              setupGame();
            }
          }
        
      }
 
        if (level_cleared()) {
          reset_level();
        }
    }

  private:
  int level;
  unsigned long time;
  Player player;
  Cannonball ball;
  Invader enemies[NUM_ENEMIES] = {Invader()};

    // check if Player defeated all Invaders in current level
    bool level_cleared() {
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() > 0) {
          return false;
        }
      }
      return true;
    }

    // set up a level
    void reset_level() {
      level = level + 1;
      setupGame();
    }

    // implement bottom_row_cleared()
    bool bottom_row_cleared() const {
      for (int i = NUM_ENEMIES_ROW; i < NUM_ENEMIES; i++) {
        if (!enemies[i].isDead()) {
          return false;
        }
      }
      return true;
    }
};
// a global variable that represents the game Space Invaders
Game game;
// DEBUGGING / TESTING GLOBAL VARS
long time = 0;
int startX = 0;
int startY = 0;
// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  title();
  game.setupGame();

}
// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
  game.update(potentiometer_value, button_pressed);
  
}

// displays Level
void print_level(int level) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(1, 0);
  matrix.print("LEVEL:");
  matrix.print(level);
}

// displays number of lives
void print_lives(int lives) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(1, 0);
  matrix.print("LIVES:");
  matrix.print(lives);
}

// displays "game over"
void game_over() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(1, 0);
  matrix.println("Game Over");
}

void title() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextSize(1);

  int textWidth = 4 * strlen("SPACE INVADERS");

  int x = matrix.width();

  while (x > -textWidth) {
    matrix.fillScreen(BLACK.to_333());
    matrix.setCursor(x - 28, 0);
    matrix.setTextColor(RED.to_333());
    matrix.print("SPACE");
    matrix.setTextColor(WHITE.to_333());
    matrix.print("INVADERS");

    delay(150);
    x--;
  }
}
