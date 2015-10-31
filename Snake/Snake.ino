uint8_t P_PINS[8] = {13, 12, A4, A3, A0, 11, A2, A1};
uint8_t N_PINS[8] = {3, 2, 4, 5, 7, 6, 8, 9};
int NUM_PINS = 8;
int SIDE_LENGTH = 4;

// If HARD_MODE is true, then the player dies when she crashes into
// walls. Otherwise, the player only dies when she crashes into herself.
boolean HARD_MODE = true;

char KEYS[6] = {'e', 'q', 'd', 'a', 'w', 's'};
int8_t HOW_TO_TURN[6][3] = {
  {0, 0, 1},
  {0, 0, -1},
  {0, 1, 0},
  {0, -1, 0},
  {1, 0, 0},
  {-1, 0, 0}
};

uint8_t MAX_BRIGHTNESS = 16;
uint8_t SNAKE_BRIGHTNESS = 16;
uint8_t FOOD_BRIGHTNESS = 5;

uint8_t values[4][4][4];
uint8_t snake[4][4][4];
uint8_t snakeHead[3] = {0, 0, 0};
uint8_t snakeLen = 1;
int8_t snakeDir[3] = {0, 0, 0};
uint8_t food[3];

unsigned int frameTime = 400;


void setup() {
  // put your setup code here, to run once:
  for (uint8_t i = 0; i < NUM_PINS; i++) {
    pinMode(P_PINS[i], OUTPUT);
    digitalWrite(P_PINS[i], HIGH);
    pinMode(N_PINS[i], OUTPUT);
    digitalWrite(N_PINS[i], HIGH);
  }
  
  for (uint8_t x = 0; x < SIDE_LENGTH; x++) {
     for (uint8_t y = 0; y < SIDE_LENGTH; y++) {
       for (uint8_t z = 0; z < SIDE_LENGTH; z++) {
         values[x][y][z] = 0;
         snake[x][y][z] = 0;
       }
     }
   }
   
   snake[snakeHead[0]][snakeHead[1]][snakeHead[2]] = snakeLen;
   values[snakeHead[0]][snakeHead[1]][snakeHead[2]] = SNAKE_BRIGHTNESS;
   pinMode(A5, INPUT);
   int seed = analogRead(A5);
   randomSeed(seed);
   generateFood();
  
   Serial.begin(9600);
}

int loopNum = 0;

void loop() {
  unsigned long startTimeMillis = millis();
  // check for incoming serial data:
  if (Serial.available() == 1) {
    // read incoming serial data:
    char inChar = Serial.read();
    Serial.println(inChar);
    for(int i = 0; i < 6; i++){
       if(inChar == KEYS[i]){
          snakeDir[0] = HOW_TO_TURN[i][0];
          snakeDir[1] = HOW_TO_TURN[i][1];
          snakeDir[2] = HOW_TO_TURN[i][2];
       }
    }
  } else if(Serial.available() > 1){
     while(Serial.available() > 0)
        Serial.read(); 
  }
  
  loopNum++;
  if(snakeDir[0] != 0 || snakeDir[1] != 0 || snakeDir[2] != 0){
    // Only advance the snake if it has a direction, ie the
    // player has started the game
    advanceSnake();
  }
  
  while(true){
    display(values);
    float cycle_fraction = float(millis() - startTimeMillis) / frameTime;
    //Serial.println(cycle_fraction * 8);
    values[food[0]][food[1]][food[2]] = (int(cycle_fraction * 8) % 2) * FOOD_BRIGHTNESS;
    if(cycle_fraction > 1)
      break;
  }
}

void advanceSnake(){
  // isCollision is true iff the snake is about to run into a wall if it advances
  boolean isCollision = false;
  for(uint8_t i = 0; i < 3; i++){
     if(snakeDir[i] == -1 && snakeHead[i] == 0)
       isCollision = true;
     if(snakeDir[i] == 1 && snakeHead[i] == 3)
       isCollision = true;
  }
  
  Serial.print(snakeHead[0]);
  Serial.print(snakeHead[1]);
  Serial.print(snakeHead[2]);
  Serial.print(" ");
  Serial.print(snakeDir[0]);
  Serial.print(snakeDir[1]);
  Serial.print(snakeDir[2]);
  
  if(isCollision){
    Serial.print("   Collision with wall");
    if(HARD_MODE)
       die();
  } else {
    snakeHead[0] += snakeDir[0];
    snakeHead[1] += snakeDir[1];
    snakeHead[2] += snakeDir[2];
    
    if(snakeHead[0] == food[0] && snakeHead[1] == food[1] && snakeHead[2] == food[2]){
      Serial.print("   nom");
      snakeLen++;
      frameTime *= 0.95; // accelerate each time the snake eats
      generateFood();
    } else if (snake[snakeHead[0]][snakeHead[1]][snakeHead[2]] > 0){
      // If the snake is moving into a bright LED, and it isn't food,
      // it means it is crashing into itself and it's game over.
      die();
    }
    
    for (uint8_t x = 0; x < SIDE_LENGTH; x++) {
       for (uint8_t y = 0; y < SIDE_LENGTH; y++) {
         for (uint8_t z = 0; z < SIDE_LENGTH; z++) {
           if(snake[x][y][z] > 0){
             snake[x][y][z]--;
             if(snake[x][y][z] == 0)
                values[x][y][z] = 0; // If the tail just exited this location, turn off the LED
           }
         }
       }
     }
     
    snake[snakeHead[0]][snakeHead[1]][snakeHead[2]] = snakeLen;
    values[snakeHead[0]][snakeHead[1]][snakeHead[2]] = SNAKE_BRIGHTNESS;
  }
  Serial.println();
}

/*
Creates an item of food at a random unoccupied location.
It assigns "food" to the correct coordinates and sets the LED
at those coordinates to the correct brightness
*/
void generateFood(){
  // the loop ensures the food is not generated within the snake
  do {
    food[0] = random(0, 4);
    food[1] = random(0, 4);
    food[2] = random(0, 4);
  } while(snake[food[0]][food[1]][food[2]] > 0);
  values[food[0]][food[1]][food[2]] = FOOD_BRIGHTNESS;
}

/*
When the player dies, flash all the LED's bright and dark 10 times
to signal the game is over, then wait indefinitely.

TODO: restart game on user input.
*/
void die(){
  Serial.println();
  Serial.print("You scored ");
  Serial.println(snakeLen);
  for(uint8_t j = 0; j < 10; j++){
   for (uint8_t x = 0; x < SIDE_LENGTH; x++) {
     for (uint8_t y = 0; y < SIDE_LENGTH; y++) {
       for (uint8_t z = 0; z < SIDE_LENGTH; z++) {
         values[x][y][z] = 16;
       }
     }
   }
   for(uint8_t i = 0; i < 10; i++){
     display(values);
   }
   delay(60);
  }
  // I'm not sure how to quit so I'll just wait forever
  while(true)
    delay(1000);
}

void print(uint8_t values[4][4][4]) {
  char hex[17] = {' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  for (uint8_t x = 0; x < SIDE_LENGTH; x++) {
     for (uint8_t y = 0; y < SIDE_LENGTH; y++) {
       for (uint8_t z = 0; z < SIDE_LENGTH; z++) {
         Serial.write(hex[values[x][y][z]]);
       }
       Serial.print(" | ");
     }
     Serial.println();
   }
   Serial.println("---------------------------");
}

/*
 Turns on all the LEDs to the specified brightness [0, 15] for approximately 
 1.28 seconds.
 */
void display(uint8_t values[4][4][4]) {
  for (uint8_t i = 0; i < NUM_PINS; i++) {
    digitalWrite(P_PINS[i], LOW);
    for (uint8_t fractional_time = 0; fractional_time <= MAX_BRIGHTNESS; fractional_time++){
      for (uint8_t j = 0; j < NUM_PINS; j++) {
        digitalWrite(N_PINS[j], getValue(values, i, j) <= fractional_time);
      }
      delayMicroseconds(10);
    }
    digitalWrite(P_PINS[i], HIGH);
  }
}

uint8_t getValue(uint8_t values[4][4][4], uint8_t negativePin, uint8_t positivePin) {
  uint8_t layer;
  if(positivePin > 3){
    if(negativePin > 3)
      layer = 2;
    else
      layer = 1;
  } else {
    if(negativePin > 3)
      layer = 3;
    else
      layer = 0;
  }
  uint8_t x = negativePin % 4;
  uint8_t y = positivePin % 4;
  return values[x][y][layer];
}

