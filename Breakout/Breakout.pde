//
//  Title: Breakout
//  Description: A program that allows a person to play Breakout
//  Created by: Alexander Caza
//  Created on: May 16th, 2022
//  Last Update: June 19th, 2022
//  Known Limitations: Ball occasionally does not bounce off of a block after destroying it; Ball may occasionally bounce off a block in a way that may seem somewhat counterintuitive; Game may occasionally stutter slightly
//
///////////////////////////////////////////////////////////////////////////////////////////

int lives = 3; //the player's lives
int bounces = 0; //the number of times the ball has bounced off of the paddle
int leastBounces; //the record for the least bounces off the paddle needed to win
int loseScreen = 0; //how many frames the lose screen is being displayed for
int winScreen = 0; //how many frames the win screen is being displayed for
int posInArrayList = 0; //the position in the ArrayList of the block being checked for the ball colliding
boolean collisionDetected;
String[] line; //the line in the leastbounces.txt file
ArrayList<Block> blocks = new ArrayList<Block>(); //stores the blocks the player has to destroy
File leastBouncesFile; //the leastbounces.txt file when checking if it exists in the setup() function
PrintWriter leastBouncesPrintWriter; //the leastbounces.txt file
Ball ball = new Ball(); //creates a ball 
Paddle paddle = new Paddle(); //creates the paddle

class Ball { //the ball
  PVector position = new PVector(500, 400); //the ball's position
  PVector velocity = new PVector(0, 2);  //the ball's velocity
  float bouncePosition; //where the ball bounces off the paddle where the center of the paddle is 0
  float bouncePositionRatio; //the ratio of the relative position of the bounce when it bounces off the paddle to the maximum length of the paddle where the center of the paddle is 0
  void updatePosition() { //updates the ball's position based on it's speed and velocity
    position.x = position.x + velocity.x; //adds the ball's x velocity to it's x position
    position.y = position.y + velocity.y; //adds the ball's y velocity to it's y position
  }
  void display() { 
    stroke(#ffff00); //sets the color of the outline (stroke)
    fill(#ffff00); //sets the color of the object (inside)
    circle(position.x, position.y, 20); //draws a circle
  }
  void checkPosition() { //checks if any action is required due to the position of the ball
    //if the ball is touching the top, left or right side of the screen
    if (position.y > 800) { //the ball has gone out of the bottom of the screen
      if (lives > 0) { //if the player still has lives left
        lives = lives - 1; //removes a life
        position.x = 500; //puts the ball in the middle of the screen horizontally
        position.y = 400; //puts the ball in the middle of the screen vertically
        paddle.position.x = 425; //resets the paddle to the middle of the screen horizontally
        ball.velocity.set(0, 2); //resets the ball's velocity
      }
      if (lives == 0 && loseScreen == 0) { //if the player has no lives left and the lose screen isn't already being displayed
        loseScreen = 900; //makes the lose screen show for 900 frames
      }
    }
    else if (position.x <= 10 && velocity.x < 0) { //if the ball is touching or past the left edge of the screen and is still moving that way
      velocity.x = -(velocity.x); //inverts the direction of the ball's horizontal velocity
    }
    else if (position.x >= 990 && velocity.x > 0) { //if the ball is touching or past the right edge of the screen and is still moving that way
      velocity.x = -(velocity.x); //inverts the direction of the ball's horizontal velocity
    }
    //if the ball is touching the top of the screen
    if (position.y <= 10 && velocity.y < 0) { //if the ball is touching or past the top of the screen and is still moving that way
      velocity.y = -(velocity.y); //inverts the direction of the ball's vertical velocity
    }
    //if the ball hits the paddle
    if (position.x >= paddle.position.x && position.x <= paddle.position.x + 150 && position.y + 10 > paddle.position.y && position.y + 10 < paddle.position.y + 15) { //if the ball is touching the paddle
      bouncePosition = position.x - paddle.position.x - 75; //calculates where on the paddle the ball bounces off based on the center of the padle being x = 0
      bounces = bounces + 1; //increases the number of times the ball has bounced by 1
      if (bouncePosition <= 0) {
        bouncePositionRatio = bouncePosition / 75; //calculates the ratio of the relative position of the bounce when it bounces off the paddle to the maximum length of the paddle where the center of the paddle is 0
        velocity.x = 0 + 1 * bouncePositionRatio; //changes the ball's x velocity based on the bouncePositionRatio variable
        velocity.y = -1 + (-1 * (1 - abs(bouncePositionRatio))); //changes the ball's y velocity based on bouncePositionRatio
      }
      if (bouncePosition > 0) {
        bouncePositionRatio = bouncePosition / 75; //calculates the ratio of the relative position of the bounce when it bounces off the paddle to the maximum length of the paddle where the center of the paddle is 0
        velocity.x = 0 + 1 * bouncePositionRatio; //changes the ball's x velocity based on the bouncePositionRatio variable
        velocity.y = -1 + (-1 * (1 - abs(bouncePositionRatio))); //changes the ball's y velocity based on bouncePositionRatio
      }
    }
  }
}

class Block { //the block that the player must hit
  PVector position = new PVector(); //the position of the block's top left corner
  int hitpoints; //the amount of hitpoints the block has 
  float testY; //the y coordinate of the side of the block being tested for a collision
  float testX; //the x coordinate of the side of the block being tested for a collision
  float distX; //the ball's x distance from the block
  float distY; //the ball's y distance from the block
  float distance; //the ball's total distance from the block
  Block(float x, float y) { //the constructor for the Block object that takes the x and y position of the top eft corner of the block as parameters
    position.x = x; //sets the x position of the top left corner of the block based on the x parameter
    position.y = y; //sets the y position of the top left corner of the block based on the y parameter
    hitpoints = 1; //sets the block's hitpoints to 1
  } 
  void display() { //displays the block
    strokeWeight(2); //sets the stroke width to 2
    stroke(#000000); //sets the color of the outline (stroke)
    fill(#00ff00); //sets the color of the object (inside)
    rect(position.x, position.y, 100, 40); //draws a rectangle
    strokeWeight(4); //resets the stroke width to the default value
  }
  boolean checkCollision() { //checks if the ball has collided with the edge of the block or is slightly inside the block and inverts the block's direction if it has
    if (ball.position.x >= position.x && ball.position.x <= position.x + 100 && ball.position.y + 10 >= position.y && ball.position.y + 10 <= position.y + 5 && ball.velocity.y >= 0) { //if the ball is colliding with or slightly inside the top of block and the ball is not moving away from the center of the block
      ball.velocity.y = -ball.velocity.y; //inverts the ball's y velocity
      hitpoints = hitpoints - 1;
      if (hitpoints <= 0) { //if the block has 0 hitpoints left
        blocks.remove(this); //removes the block from the array list of blocks
        return true; //returns true to indicate that a collision was detected
      }
    }
    else if (ball.position.x >= position.x - 2 && ball.position.x <= position.x + 100 && ball.position.y - 10 >= position.y + 30 && ball.position.y - 10 <= position.y + 40 && ball.velocity.y <= 0) { //if the ball is colliding with or slightly inside the bottom of the block and the ball is not moving away from the center of the block
      ball.velocity.y = -ball.velocity.y; //inverts the ball's y velocity
      hitpoints = hitpoints - 1;
      if (hitpoints <= 0) { //if the block has 0 hitpoints left
        blocks.remove(this); //removes the block from the array list of blocks
        return true; //returns true to indicate that a collision was detected
      }
    }
    if (ball.position.y >= position.y && ball.position.y <= position.y + 40 && ball.position.x + 10 >= position.x && ball.position.x + 10 <= position.x + 10 && ball.velocity.x >= 0) { //if the ball is colliding with or slightly inside the right of block and the ball is not moving away from the center of the block
      ball.velocity.x = -ball.velocity.x; //inverts the ball's x velocity
      hitpoints = hitpoints - 1;
      if (hitpoints <= 0) { //if the block has 0 hitpoints left
        blocks.remove(this); //removes the block from the array list of blocks
        return true; //returns true to indicate that a collision was detected
      }
    }
    else if (ball.position.y >= position.y && ball.position.y <= position.y + 40 && ball.position.x - 10 >= position.x + 90 && ball.position.x - 10 <= position.x + 100 && ball.velocity.x <= 0) { //if the ball is colliding with or slightly inside the left of the block and the ball is not moving away from the center of the block
      ball.velocity.x = -ball.velocity.x; //inverts the ball's y velocity
      hitpoints = hitpoints - 1;
      if (hitpoints <= 0) { //if the block has 0 hitpoints left
        blocks.remove(this); //removes the block from the array list of blocks
        return true; //returns true to indicate that a collision was detected
      }
    }
    
    //determines which edge is closer
    if (ball.position.x <= position.x + 15) { //if the ball is closer to the left edge of the rectangle
      testX = position.x; //sets the x coordinate of the edge being checked to the left edge's x coordinate
    }
    else if (ball.position.x >= position.x + 85) { //if the ball is closer to the right edge of the rectangle
      testX = position.x + 100; //sets the x coordinate of the edge being checked to the left edge's x coordinate
    }
    if (ball.position.y <= position.y + 15) { //if the ball is closer to the top edge of the rectangle
      testY = position.y; //sets the y coordinate of the edge being checked to the top edge's y coordinate
    }
    else if (ball.position.y >= position.y + 25) { //if the ball is closer to the bottom edge of the rectangle
      testY = position.y + 40; //sets the x coordinate of the edge being checked to the top edge's y coordinate
    }
  
    //gets the distance from the closest edges
    float distX = ball.position.x - testX; //the distance horizontally from the ball to the block
    float distY = ball.position.y - testY; //the distance vertically from the ball to the block
    float distance = sqrt((distX * distX) + (distY * distY)); //the total distance from the ball to the block
  
    //if the distance is less than the radius, meaning there is a collision
    if (distance <= 10) {
      if (abs(distX) <= abs(distY)) { //if the ball has collided with the left or right edge of the block
        if (testX <= ball.position.x + 10 && ball.velocity.x >= 0) { //if the ball is colliding with the left edge and is moving towards the center of the block
          ball.velocity.x = -ball.velocity.x; //inverts the ball's horizontal velocity
          hitpoints = hitpoints - 1; //reduces the block's hitpoints by 1
        }
        else if (testX >= ball.position.x - 10 && ball.velocity.x <= 0) { //if the ball is colliding with the right edge and is moving towards the center of the block
          ball.velocity.x = -ball.velocity.x; //inverts the ball's horizontal velocity
          hitpoints = hitpoints - 1; //reduces the block's hitpoints by 1
        }
      }
      if (abs(distY) <= abs(distX)) { //if the ball has collided with the top or bottom edge of the block
        if (testY <= ball.position.y + 10 && ball.velocity.y >= 0) { //if the ball is colliding with the top edge and is moving towards the center of the block
          ball.velocity.y = -ball.velocity.y; //inverts the ball's vertical velocity
          hitpoints = hitpoints - 1; //reduces the block's hitpoints by 1
        }
        else if (testY >= ball.position.y - 10 && ball.velocity.y <= 0) { //if the ball is colliding with the bottom edge and is moving towards the center of the block
          ball.velocity.y = -ball.velocity.y; //inverts the ball's vertical velocity
          hitpoints = hitpoints - 1; //reduces the block's hitpoints by 1
        }
      }
      if (hitpoints <= 0) { //if the block has 0 hitpoints left
        blocks.remove(this); //removes the block from the array list of blocks
        return true; //returns true to indicate that a collision was detected
      }
    }
    return false; //returns false to indicate that a collision was not detected
  }
}

class Paddle {
  PVector position = new PVector(425, 750);
  void display() { //displays the paddle
    stroke(#ffffff); //sets the color of the outline (stroke)
    fill(#ffffff); //sets the color of the object (inside)
    rect(position.x, position.y, 150, 5); //draws a rectangle
  }
}

void setup(){
  frameRate(180); //sets the frameRate is 180 frames per second
  size(1000, 800);  //sets the size of the stage
  for (int x = 0; x < 1000; x = x + 100) { //loops from 0 to 900 in 100 unit increments
    for (int y = 0; y < 320; y = y + 40) { //loops from 0 to 240 in 30 unit increments
      blocks.add(new Block(x, y)); //creates a block with it's top corner at the coordinates of the parameters
    }
  }
  try { //tries to load the record for the least number of bounces off the paddle to win from the leastbounces.txt
    line = loadStrings("leastbounces.txt"); //gets the line from the leastbounces.txt  file
    leastBounces = int(line[0]); //converts the line to an integer
  } catch (Exception e) { //if the program returns an exception because the leastbounces.txt file does not exist
    leastBouncesPrintWriter = createWriter("leastbounces.txt"); //creates the leastbounces.txt file
    leastBouncesPrintWriter.println("0"); //inserts the number "0" into the file as a default value
    leastBouncesPrintWriter.flush(); //writes the remaining data to the file
    leastBouncesPrintWriter.close(); //finishes the file
    line = loadStrings("leastbounces.txt"); //gets the line from the leastbounces.txt file
    leastBounces = int(line[0]); //converts the line to an integer
    leastBouncesPrintWriter.flush(); //flushes the PrintWriter object
    leastBouncesPrintWriter.close(); //closes the file
  }
  background(#000000); //sets the color of the stage
}

void draw(){
  if (blocks.size() == 0 && winScreen == 0) { //if there are no blocks left, meaning that the player has won and the win screen isn't already being shown
    winScreen = 900; //makes the win screen show for 900 frames
  }
  ball.checkPosition();
  posInArrayList = 0; //sets the position in the ArrayList of the block being checked for the ball colliding to 0
  for (int i = 0; i < blocks.size(); i++) { //loops through the blocks
    collisionDetected = blocks.get(i).checkCollision(); //checks if the ball is coliding with the block
    if (collisionDetected == false) { //if no collision is detected
      posInArrayList = posInArrayList + 1; //increases the position of the block being checked by 1
    }
    else if (collisionDetected == false) { //if a collision is detected
       i--; //reduces i by 1
    }
  }
  if (loseScreen == 0 && winScreen == 0) { //if the player has not lost or won
    ball.updatePosition(); //changes where the ball is based on it's velocity
  }
  background(#000000); //sets the color of the stage
  for (int i = 0; i < blocks.size(); i++) { //loops through each block
    blocks.get(i).display(); //draws each block on the screen
  }
  ball.display(); //draws the ball
  paddle.display(); //displays the paddle
  textAlign(LEFT, TOP); //makes the text align to the top left
  textSize(30);
  text("Bounces: " + bounces, 10, 20); //displays the number of bounces off the paddle so far this game\ left
  text("Blocks Left: " + blocks.size(), 10, 70); //displays the number of blocks left
  text("Lives Left: " + lives, 10, 120); //displays the lives left
  if (leastBounces != 0) {; //if the current record of least bounces required to win is not the default value of "0" due to no games having been played yet
    text("Least Bounces: " + leastBounces, 10, 170); //displays the record for the least bounces needed to win
  }
  if (loseScreen != 0) { //if the player has lost and the lose screen is being outputted
    textAlign(CENTER, CENTER); //centers the text
    textSize(60); //sets the font size to 60
    text("You Lose!", 500, 400); //outputs "You Lose!" in the center of the screen
    textSize(30); //resets the font size to 30
    loseScreen = loseScreen - 1; //reduces the number of frames of the lose screen left to display by 1
    if (loseScreen == 1) { //if this is the last time the lose screen will be outputted
      blocks.clear(); //clears the remaining blocks
      setup(); //calls the setup function to reset the blocks
      ball.position.x = 500; //resets the ball to the middle of the screen horizontally
      ball.position.y = 400; //resets the ball to the middle of the screen vertically
      ball.velocity.set(0, 2); //resets the ball's velocity
      paddle.position.x = 425; //resets the paddle to the middle of the screen horizontally
      bounces = 0; //resets the counter for the number of times the ball has bounced off the paddle to 0
      lives = 3; //resets the player's lives to 3
    }
  }
  if (winScreen != 0) { //if the player has won and the win screen is being outputed
    textAlign(CENTER, CENTER); //centers the text
    textSize(60); //sets the font size to 60
    text("You Win!", 500, 400); //outputs "You Win!" in the center of the screen
    textSize(30); //resets the font size to 30
    winScreen = winScreen - 1; //reduces the number of frames of the win screen left to display by 1
    if (winScreen == 1) { //if this is the last time the lose screen will be outputted
      if (bounces < leastBounces || leastBounces == 0) { //if the player required less bounces to win than the current record or the current record is the default value of "0" due to no games having been played yet
        leastBounces = bounces; //changes the record for least bounces to be the number bounces scored by the player
        line[0] = str(bounces); //prepares the new record for least bounces to be inserted into the leastbounces.txt file
        saveStrings("leastbounces.txt", line);
      }
      setup(); //calls the setup function to reset the blocks
      ball.position.x = 400; //resets the ball to the middle of the screen horizontally
      ball.position.y = 500; //resets the ball to the middle of the screen vertically
      paddle.position.x = 425; //resets the paddle to the middle of the screen horizontally
      bounces = 0; //resets the counter for the number of times the ball has bounced off the paddle to 0
      lives = 3; //resets the player's lives to 3
    }
  }
}

void keyPressed() { //if the user is pressing a key
  if (keyCode == LEFT && loseScreen == 0 && winScreen == 0 && paddle.position.x > 0) { //if the left arrow key is pressed and the game isn't over and the padde isn't at the left edge of the board
    paddle.position.x = paddle.position.x - 10; //moves the paddle 10 pixels to the left
  }
  else if (keyCode == RIGHT && loseScreen == 0 && winScreen == 0 && paddle.position.x < 850) { //if the right arrow key is pressed and the game isn't over and the paddle isn't at the left edge of the board
    paddle.position.x = paddle.position.x + 10; //moves the paddle 10 pixels to the right
  }
}
