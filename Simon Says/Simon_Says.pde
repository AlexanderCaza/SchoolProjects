//
//  Title: Simon Says
//  Description: A program that allows a person to play Simon Says 
//  Created by: Alexander Caza
//  Created on: April 25th, 2022
//  Last Update: May 12th, 2022
//  Known Limitations: None
//
///////////////////////////////////////////////////////////////////////////////////////////
int greenBrightTime = 100; //how long until the green section stops being bright
int redBrightTime = 100; //how long until the red section stops being bright
int yellowBrightTime = 100; //how long until the yellow section stops being bright
int blueBrightTime = 100; //how long until the blue section stops being bright
int darkBoard = 0; //how long the board should stay dark for
int flashCount; //how many times the baord should flash
int positionInSequence = 0; //the current position in the sequence being outputted or inputted
int gamePhase; //whether the program is showing the player the next sequence or the player is inputting the current sequence
int highScore; //the high score
int score; //stores the current user's score
float randomColourNumber; //stores the random number used to pick a random color to add to the sequence
boolean isCorrect = true; //if the colour inputted by the user is the next colour in the sequence
boolean restart; //whether the program is restarting
boolean fileExists; //whether the highscore.txt file exists
ArrayList<Character> currentSequence = new ArrayList<Character>(); //the current sequence 
ArrayList<Character> inputtedSequence  = new ArrayList<Character>(); //the sequence being inputted by the user
String[] line; //the line in the highscore.txt file
File highscoreFile; //the highscore.txt file when checking if it exists in the setup() function
PrintWriter highscore; //the highscore.txt file

void setup(){
  
 size(800, 800);  //sets the size of the stage
 addColourToSequence(); //adds another colour to the sequence
 try { //tries to load the highscore from the highscore.txt
   line = loadStrings("highscore.txt"); //gets the line from the highscore.txt  file
   highScore = int(line[0]); //converts the line to an integer
 } catch (Exception e) { //if the program returns an exception because the highscore.txt file does not exist
   highscore = createWriter("highscore.txt"); //creates the highscore.txt file
   highscore.println("0"); //inserts the number "0" into the file
   line = loadStrings("highscore.txt"); //gets the line from the highscore file
   highScore = int(line[0]); //converts the line to an integer
   highscore.flush(); //flushes the PrintWriter object
   highscore.close(); //closes the file
 }
}

void draw(){
  if (gamePhase == 0 && flashCount == 0) { //if the program is currently showing the player the next sequence and the board isn't flashing
    if (greenBrightTime == 0 && redBrightTime == 0 && yellowBrightTime == 0 && blueBrightTime == 0 && positionInSequence != currentSequence.size()) { //if none of the colours are bright currently and there are still colours left in the sequence to display to the player
      background(#000000); //sets the color of the stage
      positionInSequence = positionInSequence + 1; //adds 1 to the location in the sequence being outputted
      loadNextColour(); //prepares next colour to be shown as bright to the player
    }
    if (greenBrightTime == 0 && redBrightTime == 0 && yellowBrightTime == 0 && blueBrightTime == 0 && positionInSequence == currentSequence.size() && currentSequence.size() != 0) { //if the program has finished showing the player the current sequence
      gamePhase = 1; //sets the gamePhase to 1 where it will get the user to repeat the sequence
      positionInSequence = 0; //sets the position in the sequence to 0
    }
  }
  if (inputtedSequence.size() == currentSequence.size() && currentSequence.size() != 1) { //if the user has entered the sequence
    //makes all the colours bright for 15 frames
    flashCount = 1; //makes the board flash once
    addColourToSequence();
    gamePhase = 0; //makes the game output the next sequence
    positionInSequence = 0; //resets the position in the sequence
  }
  if (isCorrect == false) { //if the inputted colour is not the next colour in the sequence
    //makes all the colours on the board flash for 3 times for 30 frames each time
    isCorrect = true; //unsets the isCorrect variable
    restart = true; //tells the program to restart the game
    if (score > highScore) { //if the player's is greater than the current high score
      highScore = score; //changes the high score to be the score scored by the player
      line[0] = str(score); //prepares the new high score to be inserted into the highscore.txt file
      saveStrings("highscore.txt", line);
    }
    flashCount = 3; //makes the board flash 3 times
    //restarts the game
  }
  if (flashCount > 0) { //if the board is supposed to flash and nothing is being outputted
    if (greenBrightTime == 0 && redBrightTime == 0 && yellowBrightTime == 0 && blueBrightTime == 0) {
      darkBoard = 30; //makes the board dark for 30 frames
      //makes all the colours bright at once for 30 frames
      greenBrightTime = 30;
      redBrightTime = 30;
      yellowBrightTime = 30; 
      blueBrightTime = 30;
      flashCount = flashCount - 1; //reduces the flashCount counter by 1
    }
  }  
  if (restart == true && greenBrightTime == 0 && redBrightTime == 0 && yellowBrightTime == 0 && blueBrightTime == 0 && flashCount == 0 && darkBoard == 0) { //if the program has finished restarting and is done displaying
    currentSequence.clear(); //clears the currentSequence ArrayList
    for (int i = 0; i < currentSequence.size(); i++) {
    }
    inputtedSequence.clear(); //clears the inputtedSequence ArrayList
    positionInSequence = 0; //resets what position of the sequence is being outputted
    score = 0; //clears the player's score
    restart = false; // makes the program stop trying to restart
    gamePhase = 0; //makes the game start outputting a new sequence
    addColourToSequence(); //adds a colour to the sequence to restart it
    darkBoard = 120; //makes the board dark for 30 frames
  }
  background(#000000); //sets the color of the stage
  //displays the board
  drawBoard(); //draws the board
}

void drawBoard() {
  //the score
  fill(#ffffff); //sets the color of the object (inside)
  textSize(40); //sets the text size
  text("Score: " + score, 20, 60); //displays the score
  
  //the highscore
  fill(#ffffff); //sets the color of the object (inside)
  textSize(40); //sets the text size
  text("High Score: " + highScore, 20, 110); //displays the score
  
  //the green section
  if (greenBrightTime == 0 || darkBoard != 0) { //if the green section shouldn't be bright
    stroke(#000000); //sets the color of the outline (stroke)
    fill(#00b300); //sets the color of the object (inside)
    rect(200, 200, 200, 200); //draws a rectangle
    if (darkBoard != 0) { //if darkBoard is greater than 0
      darkBoard = darkBoard - 1; //reduces the darkBoard counter by 1
    }
  }
  else if (greenBrightTime != 0) { //if the green section should be bright
    stroke(#000000); //sets the color of the outline (stroke)
    fill(#1aff1a); //sets the color of the object (inside)
    rect(200, 200, 200, 200); //draws a rectangle
    greenBrightTime = greenBrightTime - 1; //reduces greenBrightTime by 1
  }
  
  //the red section
  if (redBrightTime == 0 || darkBoard != 0) { //if the red section should be bright
    stroke(#000000); //sets the color of the outline (stroke)
    fill(#b32400); //sets the color of the object (inside)
    rect(400, 200, 200, 200); //draws a rectangle
  }
  else if (redBrightTime != 0) { //if the red section shouldn't be bright
    stroke(#000000); //sets the color of the outline (stroke)
    fill(#ff0000); //sets the color of the object (inside)
    rect(400, 200, 200, 200); //draws a rectangle
    redBrightTime = redBrightTime - 1; //reduces greenBrightTime by 1
  }
  
  //the yellow section
  if (yellowBrightTime == 0 || darkBoard != 0) { //if the yellow section should be bright
    stroke(#000000); //sets the color of the outline (stroke)
    fill(#cccc00); //sets the color of the object (inside)
    rect(200, 400, 200, 200); //draws a rectangle
  }
  else if (yellowBrightTime != 0 || darkBoard != 0) { //if the yellow section shouldn't be bright
    stroke(#000000); //sets the color of the outline (stroke)
    fill(#ffff00); //sets the color of the object (inside)
    rect(200, 400, 200, 200); //draws a rectangle
    yellowBrightTime = yellowBrightTime - 1; //reduces yellowBrightTime by 1
  }
  
  //the blue section
  if (blueBrightTime == 0 || darkBoard != 0) { //if the blue section should be bright
    stroke(#000000); //sets the color of the outline (stroke)
    fill(#008fb3); //sets the color of the object (inside)
    rect(400, 400, 200, 200); //draws a rectangle
  }
  else if (blueBrightTime != 0) { //if the blue section shouldn't be bright
    stroke(#000000); //sets the color of the outline (stroke)
    fill(#00ccff); //sets the color of the object (inside)
    rect(400, 400, 200, 200); //draws a rectangle
    blueBrightTime = blueBrightTime - 1; //reduces blueBrightTime by 1
  }
}

void drawDarkBoard() { //draws the board with no colors being bright
  //the green section
  stroke(#000000); //sets the color of the outline (stroke)
  fill(#00b300); //sets the color of the object (inside)
  rect(200, 200, 200, 200); //draws a rectangle
  
  //the red section
  stroke(#000000); //sets the color of the outline (stroke)
  fill(#b32400); //sets the color of the object (inside)
  rect(400, 200, 200, 200); //draws a rectangle
  
  //the yellow section
  stroke(#000000); //sets the color of the outline (stroke)
  fill(#cccc00); //sets the color of the object (inside)
  rect(200, 400, 200, 200); //draws a rectangle
  
  //the blue section
  stroke(#000000); //sets the color of the outline (stroke)
  fill(#008fb3); //sets the color of the object (inside)
  rect(400, 400, 200, 200); //draws a rectangle
}

void addColourToSequence() { //adds another colour to the sequence
  randomColourNumber = int(random(1, 5)); //picks a random integer from 1 to 4 and stores it as randomColourNumber
  if (randomColourNumber == 1) { //if randomColourNumber is 1
    currentSequence.add('g'); //adds a 'g' to the currentSequence to represent green
  }
  if (randomColourNumber == 2) { //if randomColourNumber is 2
    currentSequence.add('r'); //adds a 'r' to the currentSequence to represent red
  }
  if (randomColourNumber == 3) { //if randomColourNumber is 3
    currentSequence.add('y'); //adds a 'y' to the currentSequence to represent yellow
  }
  if (randomColourNumber == 4) { //if randomColourNumber is 4
    currentSequence.add('b'); //adds a 'b' to the currentSequence to represent blue
  }
}

void loadNextColour() { //prepares next colour to be shown as bright to the player
  darkBoard = 15; //makes the board dark for 15 frames
  if (currentSequence.get(positionInSequence - 1) == 'g') { //if the next colour to be displayed is green
    greenBrightTime = 100; //sets the time for green to be displayed as being bright to 100
  }
  else if (currentSequence.get(positionInSequence - 1) == 'r') { //if the next colour to be displayed is red
    redBrightTime = 100; //sets the time for red to be displayed as being bright to 100
  }
  else if (currentSequence.get(positionInSequence - 1) == 'y') { //if the next colour to be displayed is yellow
    yellowBrightTime = 100; //sets the time for yellow to be displayed as being bright to 100
  }
  else if (currentSequence.get(positionInSequence - 1) == 'b') { //if the next colour to be displayed is blue
    blueBrightTime = 100; //sets the time for blue to be displayed as being bright to 100
  }
}

void mouseClicked() { //if the mouse is clicked
  if (gamePhase == 1 && inputtedSequence.size() < currentSequence.size() && restart == false) { //if the player is supposed to be repeating the sequence, there are colours left to input and the program is not restarting
    if (mouseX >= 200 && mouseX <= 400 && mouseY >= 200 && mouseY <= 400) { //if the mouse is inside the green box
      inputtedSequence.add('g'); //adds green to the inputtedSequence ArrayList
      greenBrightTime = 15; //makes the green bright for 0.25 secs
    }
    else if (mouseX >= 400 && mouseX <= 600 && mouseY >= 200 && mouseY <= 400) { //if the mouse is inside the red box
      inputtedSequence.add('r'); //adds red to the inputtedSequence ArrayList
      redBrightTime = 15; //makes the red bright for 0.25 secs
    }
    else if (mouseX >= 200 && mouseX <= 400 && mouseY >= 400 && mouseY <= 600) { //if the mouse is inside the yellow box
      inputtedSequence.add('y'); //adds yellow to the inputtedSequence ArrayList
      yellowBrightTime = 15; //makes the yellow bright for 0.25 secs
    }
    else if (mouseX >= 400 && mouseX <= 600 && mouseY >= 400 && mouseY <= 600) { //if the mouse is inside the yellow box
      inputtedSequence.add('b'); //adds blue to the inputtedSequence ArrayList
      blueBrightTime = 15; //makes the blue bright for 0.25 secs
    }
    if (mouseX >= 200 && mouseX <= 600 && mouseY >= 200 && mouseY <= 600) { //if the mouse was clicked in any of the squares
      positionInSequence = positionInSequence + 1; //increases positionInSequence by 1
      isCorrect = checkInput(); //checks whether the inputted colour is the next colour in the sequence
      if (isCorrect == true && positionInSequence == currentSequence.size() && currentSequence.size() != 0) { //if the input is correct and is the last colour in the current sequence
        flashCount = 1; //makes the board flash once
        addColourToSequence(); //adds another colour to the sequence
        gamePhase = 0; //makes the game output the next sequence
        positionInSequence = 0; //makes the game start outputting the new sequence from the beginning
        inputtedSequence.clear(); //clears the inputtedSequence ArrayList
        score = score + 1; //increases the player's score by 1
      }
    }
  }
}

boolean checkInput() { //checks whether the colour clicked by the player is the next colour in the sequence
  if (inputtedSequence.get(positionInSequence - 1) == currentSequence.get(positionInSequence - 1)) {
    return true;
  }
  else {
    return false;
  }
}
  
