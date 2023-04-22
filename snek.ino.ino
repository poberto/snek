#include <LedControl.h>

// LED matrix pins
const byte DIN_PIN = 7;
const byte CS_PIN = 6;
const byte CLK_PIN = 5;
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

// Joystick pins
const int JOY1_X = A0;
const int JOY1_Y = A1;
const int JOY2_X = A2;
const int JOY2_Y = A3;

// Game parameters
const byte ROWS = 32;
const byte COLS = 32;
const int GAME_SPEED = 200;

// Game states
enum GameState { MENU, PLAYING, GAME_OVER };
GameState gameState;

struct Snake {
  int headX, headY;
  int length;
  int tailX[ROWS * COLS];
  int tailY[ROWS * COLS];
  int direction;
  int prevDirection;
};
Snake snake1, snake2;

int foodX, foodY;

// Initialize the game
void setup() {
  // Setup LED matrix
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  // Setup joysticks
  pinMode(JOY1_X, INPUT);
  pinMode(JOY1_Y, INPUT);
  pinMode(JOY2_X, INPUT);
  pinMode(JOY2_Y, INPUT);

  // Initialize game state
  gameState = MENU;

  // Initialize random seed
  randomSeed(analogRead(0));
}

void loop() {
  switch (gameState) {
    case MENU:
      // Display menu and wait for input
      displayMenu();
      break;
    case PLAYING:
      readJoystickInput();
      updateSnakePositions();
      checkCollisions();
      updateFood();
      drawGameState();
      delay(GAME_SPEED);
      break;
    case GAME_OVER:
      // Display game over and wait for input
      displayGameOver();
      break;
  }
}

void displayMenu() {
  // Display a simple menu and wait for input to start the game
  // You can customize this to include instructions or other menu options
  lc.setChar(0, 0, 'P', true);
  lc.setChar(0, 8, 'L', true);
  lc.setChar(0, 16, 'A', true);
  lc.setChar(0, 24, 'Y', true);

  if (analogRead(JOY1_X) < 400 || analogRead(JOY1_Y) < 400 || analogRead(JOY2_X) < 400 || analogRead(JOY2_Y) < 400) {
    initializeGame();
    gameState = PLAYING;
  }
}

void readJoystickInput() {
  // Read input from both joysticks and update snake directions
  // Prevent snakes from reversing direction directly
  updateSnakeDirection(snake1, analogRead(JOY1_X), analogRead(JOY1_Y));
  updateSnakeDirection(snake2, analogRead(JOY2_X), analogRead(JOY2_Y));
}

void updateSnakeDirection(Snake &snake, int x, int y) {
  if (x < 400) {
    if (snake.prevDirection != 1) {
      snake.direction = 0; // Left
    }
  } else if (x > 600) {
    if (snake.prevDirection != 0) {
      snake.direction = 1; // Right
    }
  } else if (y < 400) {
    if (snake.prevDirection != 3) {
      snake.direction = 2; // Up
    }
  } else if (y > 600) {
    if (snake.prevDirection != 2) {
      snake.direction = 3; // Down
    }
  }
}

void updateSnakePositions() {
  // Update snake positions based on their current direction
  moveSnake(snake1);
  moveSnake(snake2);
}

void moveSnake(Snake &snake) {
  // Shift tail positions
  for (int i = snake.length - 1; i > 0; i--) {
    snake.tailX[i] = snake.tailX[i - 1];
    snake.tailY[i] = snake.tailY[i - 1];
  }
  
  // Update the head position based on direction
  switch (snake.direction) {
    case 0: // Left
      snake.headX--;
      break;
    case 1: // Right
      snake.headX++;
      break;
    case 2: // Up
      snake.headY--;
      break;
    case 3: // Down
      snake.headY++;
      break;
  }

  // Update the first tail position to the previous head position
  snake.tailX[0] = snake.headX;
  snake.tailY[0] = snake.headY;

  // Store the previous direction
  snake.prevDirection = snake.direction;
}

void checkCollisions() {
  // Check for collisions with boundaries, food, and other snakes
  checkBoundaryCollision(snake1);
  checkBoundaryCollision(snake2);
  checkFoodCollision(snake1);
  checkFoodCollision(snake2);
  checkSnakeCollision(snake1, snake2);
  checkSnakeCollision(snake2, snake1);
}

void checkBoundaryCollision(Snake &snake) {
  if (snake.headX < 0 || snake.headX >= COLS || snake.headY < 0 || snake.headY >= ROWS) {
    gameState = GAME_OVER;
  }
}

void checkFoodCollision(Snake &snake) {
  if (snake.headX == foodX && snake.headY == foodY) {
    snake.length++;
    spawnFood();
  }
}

void checkSnakeCollision(Snake &snakeA, Snake &snakeB) {
  for (int i = 0; i < snakeB.length; i++) {
    if (snakeA.headX == snakeB.tailX[i] && snakeA.headY == snakeB.tailY[i]) {
      gameState = GAME_OVER;
    }
  }
}

void updateFood() {
  // If there is no food on the board, spawn new food
  if (foodX == -1 && foodY == -1) {
    spawnFood();
  }
}

void spawnFood() {
  bool validPosition;
  
  do {
    validPosition = true;
    foodX = random(COLS);
    foodY = random(ROWS);

    // Check if the food position is valid (not on any snake)
    for (int i = 0; i < snake1.length; i++) {
      if (foodX == snake1.tailX[i] && foodY == snake1.tailY[i]) {
        validPosition = false;
        break;
      }
    }

    for (int i = 0; i < snake2.length; i++) {
      if (foodX == snake2.tailX[i] && foodY == snake2.tailY[i]) {
        validPosition = false;
        break;
      }
    }
  } while (!validPosition);
}

void drawGameState() {
  lc.clearDisplay(0);

  // Draw the food
  lc.setLed(0, foodX, foodY, true);

  // Draw Snake 1
  for (int i = 0; i < snake1.length; i++) {
    lc.setLed(0, snake1.tailX[i], snake1.tailY[i], true);
  }

  // Draw Snake 2
  for (int i = 0; i < snake2.length; i++) {
    lc.setLed(0, snake2.tailX[i], snake2.tailY[i], true);
  }
}