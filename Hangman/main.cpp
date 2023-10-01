//
//  Title: Hangman
//  Description: Plays a game of hangman with the user. This program has a singleplayer mode with premade words and a multiplayer mode where a second player can choose the word. This program has an easy and a hard difficulty which changes the number of guesses the player gets.
//  Created by: Alexander Caza
//  Created on: March 9th, 2021
//  Last Update: April 14th, 2021
//  Known Limitations: Unable to remove "Program ended with exit code: 0" message; Program will occasionally display letters in the incorrect guesses field (with their memory location shown?????) in the format "\xxx" where the x's are numbers; In singleplayer, the game will occasionally not recognise when the word is fully guessed.
//
///////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <cctype>
#include <iostream>
#include <string>
#include <cstring>
#include <string.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// variables
char gamemode;
char letterSelection;
char difficulty;
char unknownLetterPlaceholder = '_';
string singleplayerWordBank[25] = {"frozen", "castle", "emoji", "soldier", "ocean", "chicken", "pineapple", "swanky", "unarmed", "difficulty", "seagull", "camel", "brick", "olives", "beach", "necessary", "balloon", "skeleton", "xylophone", "zebra", "colonel", "error", "accident", "island", "monkey"}; // singleplayer word bank
string singleplayerWordPreConversion;
string singlePlayerWord;
string multiplayerWord;
vector<char> singleplayerWordComplete; // stores letters from user in a vector for singleplayer
vector<char> singleplayerWordIncomplete; // stores letters from user in a vector for singleplayer
vector<char> singleplayerIncorrectGuesses; // stores incorrect guessed letters for singleplayer
vector<char> singleplayerCorrectGuesses; // stores correct guessed letters for singleplayer
vector<char> multiplayerWordComplete; // stores letters from user in a vector for multiplayer
vector<char> multiplayerWordIncomplete; // stores letters from user in a vector for multiplayer
vector<char> multiplayerIncorrectGuesses; // stores incorrect guessed letters for multiplayer
vector<char> multiplayerCorrectGuesses; // stores correct guessed letters for multiplayer
int singleplayerRandomNumber; // used to choose word from word bank in singleplayer
int wordLength;
int numGuessesLeft;
char singleplayerGuess;
char multiplayerGuess;
char guessWasCorrect = 'y';
char singleplayerWordIsComplete;
char uniqueGuess;
char playAgain;



void introInstructions() { // welcome user and explains instructions
    cout << "Welcome to Hangman! \n";
    cout << "INSTRUCTIONS: \n";
    cout << "This game supports two gamemodes: singleplayer and multiplayer. \n";
    cout << "In singleplayer, the game will choose the word. \n";
    cout << "In multiplayer, the second player will choose the word. \n";
    cout << "Each time, Player 1, will choose a letter. \n";
    cout << "If the letter is in the word, it will appear in the word. \n";
    cout << "If it isn't, it will be added to the list of guessed letters and a part of hangman will be added. \n";
    cout << "If Player 1 completes the word, Player 1 wins. \n";
    cout << "In singleplayer, if the graphic is completed before the word is completed, you lose. \n";
    cout << "In multiplayer, if the graphic is completed before the word Player 2 wins. \n\n";
}



char gamemodeConverter(char gamemode) { // converts gamemode character value put by the user to lowercase to simplify its use in program
    if (gamemode == 's' || gamemode == 'S') { // if gamemode is 's' or 'S', converts to 's'
        return 's';
    }
    else if (gamemode == 'm' || gamemode == 'M') { // if gamemode is 'm' or 'M', converts to 'm'
        return 'm';
    }
    else { // if gamemode input by user is anything else, sets it to 'x' to signifiy unknown
        return 'x';
    }
}

string singleplayerWordSelection() { // chooses a random word from the wordbank for use during singleplayer
    srand((int)time(0));
    singleplayerRandomNumber = rand() % 24; // picks a random number between 0 - 24
    return singleplayerWordBank[singleplayerRandomNumber]; // sends the random word from the word bank back to the main program
}

string multiplayerWordSelection() { // has Player 2 pick a word for player one to guess in multiplayer mode.
    cout << "Player 1, please look away, Player 2, please think of a word.\n"; // tells Player 1 to look away.
    cout << "What is your word, please enter it letter by letter. \n"; // asks Player 2 to enter word letter by letter.
    wordLength = 0;
    do {
        cout << "Please enter letter " << wordLength + 1 << " now. (Words can only include letters. Enter * if there are no more letters in your word. Enter '-' to delete the last letter you entered)"; // asks user to insert word
        cin >> letterSelection; // user inputs letter
        letterSelection = (char) tolower(letterSelection);  // converts letters input by the user to lowercase
        if (letterSelection != '*' && letterSelection != '-') {
            multiplayerWordComplete.push_back(letterSelection); // adds a letter to the vector
            wordLength = wordLength + 1; // updates word length value
        }
        else if (letterSelection == '-') { // if '-' is entered by the user
            multiplayerWordComplete.pop_back(); // removes the last letter in the vector that stores the word (deletes the last letter in the word so far)
            wordLength = wordLength - 1; // removes 1 from the word length value (since this process deleted a letter from the word)
        }
        cout << "You have entered: ";
        for (int i = 0; i < multiplayerWordComplete.size(); i++){
            cout << multiplayerWordComplete.at(i); // displays the word entered so far letter by letter
        } // repeats until all letters in word are shown to user
        cout << " so far. \n"; // says so far
    } while (letterSelection != '*');
    
    multiplayerWord = string(multiplayerWordComplete.data(), multiplayerWordComplete.size());
    

    
    return multiplayerWord;
}
char difficultySelection() {
    do {
        cout << "What difficulty would you like to play at? Easy = 10 guesses, Hard = 7 guesses. (Enter 'e' or 'h')\n\n"; // asks what difficulty the user wants to play at.
        cin >> difficulty; // user inputs difficulty
        if (difficulty == 'e' || difficulty == 'E') { // converts 'E' to 'e'
            difficulty = 'e';
        }
        else if (difficulty == 'h' || difficulty == 'H') { // converts 'H' to 'h'
            difficulty = 'h';
        }
        else if (difficulty != 'e' || difficulty != 'h') { // converts any other value to 'x' to signify unknown
            difficulty = 'x';
        }
        if (difficulty == 'x') {
            cout << "ERROR: Invalid input, enter either 'e' or 'h'.\n";
        }
        if (difficulty == 'e') {
            cout << "You have chosen to play on the Easy difficulty.\n";
        }
        else if (difficulty == 'h') {
            cout << "You have chosen to play on the Hard difficulty.\n";
        }
    } while (difficulty == 'x'); // causes difficulty selection to rerun when an invalid input is entered
    return difficulty;
}

int guessInitialiser(char difficulty) { // if difficulty is easy, sets the number of guesses to 10
    if (difficulty == 'e') {
        return 10;
    }
    else { // if difficulty is hard, sets the number of guesses to 7
        return 7;
    }
}

void hangmanGraphic() { // displays a hangman graphic that changes based on the number of guesses.
    if (numGuessesLeft == 10) {
        cout << "        \n";
        cout << "        \n";
        cout << "        \n";
        cout << "       \n";
        cout << "       \n";
        cout << "       \n";
        cout << "         \n";
    }
    else if (numGuessesLeft == 9) {
        cout << "        \n";
        cout << "        \n";
        cout << "        \n";
        cout << "        \n";
        cout << "        \n";
        cout << "        \n";
        cout << "========= \n";
    }
    else if (numGuessesLeft == 8) {
        cout << "        \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "========= \n";
    }
    else if (numGuessesLeft == 7) {
        cout << "  +---+ \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "========= \n";
    }
    else if (numGuessesLeft == 6) {
        cout << "  +---+ \n";
        cout << "  |   | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "========= \n";
    }
    else if (numGuessesLeft == 5) {
        cout << "  +---+ \n";
        cout << "  |   | \n";
        cout << "  O   | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "========= \n";
    }

    else if (numGuessesLeft == 4) {
        cout << "  +---+ \n";
        cout << "  |   | \n";
        cout << "  O   | \n";
        cout << "  |   | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "========= \n";
    }
    else if (numGuessesLeft == 3) {
        cout << "  +---+ \n";
        cout << "  |   | \n";
        cout << "  O   | \n";
        cout << " /|   | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "========= \n";
    }
    else if (numGuessesLeft == 2) {
        cout << "  +---+ \n";
        cout << "  |   | \n";
        cout << "  O   | \n";
        cout << " /|\\  | \n";
        cout << "      | \n";
        cout << "      | \n";
        cout << "========= \n";
    }
    else if (numGuessesLeft == 1) {
        cout << "  +---+ \n";
        cout << "  |   | \n";
        cout << "  O   | \n";
        cout << " /|\\  | \n";
        cout << " /    | \n";
        cout << "      | \n";
        cout << "========= \n";
    }
    else if (numGuessesLeft == 0) {
        cout << "  +---+ \n";
        cout << "  |   | \n";
        cout << "  O   | \n";
        cout << " /|\\  | \n";
        cout << " / \\  | \n";
        cout << "      | \n";
        cout << "========= \n";
    }
    

    
}

void victoryOrLossScreen(char gamemode, int guessesLeft, char difficulty) { // displays a win or lose screen to the user. Output changes based on gamemode and difficulty. Program figures out whether the user won or lose by checking to see if the number of guesses is equal to 0.
    string gamemodeFull;
    string difficultyFull;
    if (gamemode == 'm') {
        gamemodeFull = "Multiplayer"; // converts "m" to Multiplayer
    }
    else if (gamemode == 's') {
        gamemodeFull = "Singleplayer"; // converts "s" to Singleplayer
    }
    if (difficulty == 'e') {
        difficultyFull = "Easy"; // converts "m" to Multiplayer
    }
    else if (difficulty == 'h') {
        difficultyFull = "Hard"; // converts "m" to Multiplayer
    }
    if (gamemode == 'm' && guessesLeft != 0) { // victory messages for Player 1 on multiplayer after guessing the word
        hangmanGraphic(); // displays the hangman graphic
        cout << "The word was ";
        for (int i = 0; i < multiplayerWordComplete.size(); i++) {
            cout << multiplayerWordComplete[i];
        }
        cout << ".\n";
        cout << "Congratulations Player 1, you won! \n";
        cout << "You had " << guessesLeft << " guesses remaining on " << difficultyFull << " difficulty.\n" ;
    }
    else if (gamemode == 'm' && guessesLeft == 0) { // victory messages for Player 2 on multiplayer after Player 1 has no guesses left
        cout << "The word was ";
        for (int i = 0; i < multiplayerWordComplete.size(); i++) {
            cout << multiplayerWordComplete[i];
        }
        cout << ".\n";
        cout << "Congratulations Player 2, you won! \n";
        cout << "Player 1 ran out of guesses on " << difficultyFull << " difficulty.\n";
        cout << "The word was: " << multiplayerWord << ".\n";
    }
    else if (gamemode == 's' && guessesLeft != 0) { // victory messages for singleplayer victory
        hangmanGraphic(); // displays the hangman graphic
        cout << "The word was ";
        for (int i = 0; i < singleplayerWordComplete.size(); i++) {
            cout << singleplayerWordComplete[i];
        }
        cout << ".\n";
        cout << "Congratulations, you won! \n";
        cout << "You had " << guessesLeft << " guesses remaining on " << difficultyFull << " difficulty in " << gamemodeFull << ".\n" ;
    }
    else if (gamemode == 's' && guessesLeft == 0) { // victory messages for singleplayer loss
        cout << "The word was ";
        for (int i = 0; i < singleplayerWordComplete.size(); i++) {
            cout << singleplayerWordComplete[i];
        }
        cout << ".\n";
        cout << "Sorry, you lost!! \n";
        cout << "You ran out of guesses on " << difficultyFull << " difficulty in " << gamemodeFull << ".\n";
    }
}


char checkSingleplayerCompletion() { // checks whether the singleplayer word has been guessed fully.
    for (int i = 0; i < singleplayerWordComplete.size(); i++) { // repeats for entire vector
        if (singleplayerWordIncomplete[i] == singleplayerWordComplete[i]) { // continues checking the word if the letters match in the incomplete and complete word vectors
            
        }
        else if (singleplayerWordIncomplete[i] != singleplayerWordComplete[i]) {
            return 'f'; // returns false to the program
        }
    }
    return 't'; // returns true to the program. This will run if the entire word is the same
}

void uniqueGuessChecker() { // checks whether the letter the user guessed has already been guessed
    if (gamemode == 's') { // singleplayer
        for (int i = 0; i < singleplayerIncorrectGuesses.size(); i++) { // tests for the letter in the incorrect guess vector
            if (singleplayerGuess == singleplayerIncorrectGuesses[i]) {
                uniqueGuess = 'f'; // returns false to main program if the letter is found in the vector
            }
        }
        for (int i = 0; i < singleplayerCorrectGuesses.size(); i++) { // tests for the letter in the correct guess vector
            if (singleplayerGuess == singleplayerCorrectGuesses[i]) {
                uniqueGuess = 'f';
            }
        }
    }
    else if (gamemode == 'm') { // multiplayer
        for (int i = 0; i < multiplayerIncorrectGuesses.size(); i++) { // tests for the letter in the incorrect guess vector
            if (multiplayerGuess == multiplayerIncorrectGuesses[i]) {
                uniqueGuess = 'f'; // returns false to main program if the letter is found in the vector
            }
        }
        for (int i = 0; i < multiplayerCorrectGuesses.size(); i++) { // tests for the letter in the correct guess vector
            if (multiplayerGuess == multiplayerCorrectGuesses[i]) {
                uniqueGuess = 'f'; // returns false to main program if the letter is found in the vector
            }
        }
    }
    else {
        uniqueGuess = 't'; // if the letter isn't found in any of the vectors (which means it has not been previously guessed) it returns true to the main program
    }
}

int main() {
    
    
    do { // reruns the program if the player wants to play again
        // insert code here...
        introInstructions();
        // clearing all values in certain variables and vectors to enable the ability to play again. Without this, the values stored in the vectors would remain from previous games and would break the program. ERROR HERE
        if (playAgain == 'y' || playAgain == 'Y') {
            singleplayerWordComplete.clear();
            singleplayerWordIncomplete.clear();
            singleplayerIncorrectGuesses.clear();
            singleplayerCorrectGuesses.clear();
            multiplayerWordComplete.clear();
            multiplayerWordIncomplete.clear();
            multiplayerIncorrectGuesses.clear();
            multiplayerCorrectGuesses.clear();
            singleplayerWordIsComplete = 'f';
            singleplayerWordPreConversion.clear();
            singlePlayerWord.clear();
            multiplayerWord.clear();
        }
        // end of clearing

        do {
        cout << "Which gamemode would you like to play? Singleplayer or multiplayer? (enter S or M) \n"; // asks user to input
        cin >> gamemode; // where the user inputs the difficulty
            gamemode = gamemodeConverter(gamemode); // calls the difficultyConverter function and ives it the difficulty value the user input as a parameter
            if (gamemode == 'x') {
                cout << "ERROR: Please enter a difficulty value (S/M) \n"; // if the difficultyConverter function returns x (which means the user input an invalid input), it tells the user to enter a valid difficulty
            }
        } while (gamemode == 'x'); // reruns the difficulty selection until a valid value is entered.
        
        // Singleplayer: Program chooses a word.
        if (gamemode == 's') {
            difficultySelection();
        }
        
        if (gamemode == 's') { // checks if gamemode is singleplayer
            singleplayerWordPreConversion = singleplayerWordSelection(); // calls the singleplayer word selection function to choose a word.
            
            // assigning value to string s
            string s = singleplayerWordPreConversion;
            
            int long n = s.length();
            
            // declaring character array
            char singleplayerWord[n + 1];
            
            // copying the contents of the
            // string to char array
            strcpy(singleplayerWord, s.c_str());
            
            for (int i: singleplayerWord) {
                singleplayerWordComplete.push_back(i);
            }
            
            numGuessesLeft = guessInitialiser(difficulty); // sets number of guesses based on difficulty
            
            for (int i = 0; i < singleplayerWordComplete.size() - 1; i++) {
                singleplayerWordIncomplete.push_back(unknownLetterPlaceholder); // creates a copy of singleWordComplete where all letters are _' (_'s are letters that haven't been guessed)
            }
        
            while (singleplayerWordIsComplete != 't' && numGuessesLeft != 0) { // runs when the word hasn't been guessed yet
                hangmanGraphic(); // displays the hangman graphic
                for (int i = 0; i < singleplayerWordIncomplete.size(); i++) { // displays guessed letters and underscores for unguessed letters
                    cout << singleplayerWordIncomplete.at(i) << " ";
                }
                // asks user for guess
                cout << "\n\n\n";
                do {
                    uniqueGuess = 't';
                    cout << "Previous incorrect guesses: ";
                    for (int i = 0; i < singleplayerIncorrectGuesses.size(); i++){
                        cout << singleplayerIncorrectGuesses.at(i); // displays previous incorrect guesses to the user
                        cout << ", ";
                    } // repeats until all incorrect letters in the singleplayerIncorrectGuesses vector are shown to the user
                    cout << "\n";
                    cout << "What is your guess?\n\n\n"; // asks for user guess
                    cin >> singleplayerGuess; // stores user guess
                    uniqueGuessChecker();
                    if (uniqueGuess == 'f') {
                        cout << "You have already guessed this letter, try guessing another one! \n";
                    }
                } while (uniqueGuess == 'f'); // prevents the program from continuing until a unique guess is made
                guessWasCorrect = 'n'; // sets guessWasCorrect as false before it is checked for in the word
                // checks for guess letter in word
                int elem = singleplayerGuess; // sets elem as the guess letter
                
                int i = 0; // defines check starting position
                
                do {
                    
                    while (i <= singleplayerWordIncomplete.size()) { // runs while the entire word hasn't been checked for the letter
                        if (singleplayerWordComplete[i] == elem && singleplayerWordIncomplete[i] != elem) { // if the letter is found in the word and hasn't already been found
                            break;
                        }
                        i++;
                    }
                    
                    if (i <= singleplayerWordIncomplete.size() && singleplayerWordComplete[i] == elem && singleplayerWordIncomplete[i] != elem) { // if the letter is found in the word and hasn't already been found
                        cout << "Letter " << singleplayerGuess << " is the letter in position " << i + 1 // tells the user what position the letter is in
                        << " in the word\n";
                        singleplayerWordIncomplete[i] = singleplayerGuess; // fills in the letter in the incomplete word
                        guessWasCorrect = 'y'; // stores the guess as correct
                        singleplayerCorrectGuesses.push_back(singleplayerGuess); // stores letter in correct guesses array
                    }
                    
                
                } while (guessWasCorrect == 'y' && i < singleplayerWordIncomplete.size()); // returns until entire word is searched for the letter
        
        
        
        
                if (guessWasCorrect == 'n') { // if the letter isn't in the word
                    guessWasCorrect = 'n'; // stores the guess as incorrect
                    numGuessesLeft = numGuessesLeft - 1; // removes 1 guess
                    cout << "The letter " << singleplayerGuess << " is not in the word \n"; // tells Player 1 that the letter was not in the word
                    cout << "You have " << numGuessesLeft << " guesses left. \n"; // tells Player 1 how many guesses remain
                    singleplayerIncorrectGuesses.push_back(singleplayerGuess); // stores letter in incorrect guess array
                    if (numGuessesLeft == 0) { // displays graphic if the number of guesses is 0
                        hangmanGraphic();
                    }
                }
                singleplayerWordIsComplete = checkSingleplayerCompletion();
            }
        }
        // Multiplayer: Player 2 chooses a word.
        else if (gamemode == 'm') { // checks if gamemode is multiplayer
            multiplayerWordSelection() = multiplayerWord; // calls the multiplayer word selection function to choose a word
            cout << "You have chosen the word " << multiplayerWord << ".\n";
            sleep(3); // gives Player time to see the word they entered before the screen is cleared
            cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; // wipes the screen
        
            cout << "Player 2, please ask Player 1 to turn around, it's time for them to start guessing.\n";
            difficultySelection();
            numGuessesLeft = guessInitialiser(difficulty); // sets the number of guesses based on the difficulty
        
            for (int i = 0; i < multiplayerWordComplete.size(); i++)
                multiplayerWordIncomplete.push_back(unknownLetterPlaceholder); // creates a copy of multiplayerWordComplete where all letters are _' (_'s are letters that haven't been guessed)
        
        
            // replaces all letters in multiplayerWordIncomplete with "_"
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'a', '_'); // replaces a
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'b', '_'); // replaces b
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'c', '_'); // replaces c
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'd', '_'); // replaces d
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'e', '_'); // replaces e
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'f', '_'); // replaces f
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'g', '_'); // replaces g
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'h', '_'); // replaces h
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'i', '_'); // replaces i
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'j', '_'); // replaces j
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'k', '_'); // replaces k
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'l', '_'); // replaces l
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'm', '_'); // replaces m
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'n', '_'); // replaces n
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'o', '_'); // replaces o
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'p', '_'); // replaces p
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'q', '_'); // replaces q
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'r', '_'); // replaces r
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 's', '_'); // replaces s
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 't', '_'); // replaces t
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'u', '_'); // replaces u
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'v', '_'); // replaces v
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'w', '_'); // replaces w
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'x', '_'); // replaces x
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'y', '_'); // replaces y
            replace (multiplayerWordIncomplete.begin(), multiplayerWordIncomplete.end(), 'z', '_'); // replaces z
        
            
            while (multiplayerWordIncomplete != multiplayerWordComplete && numGuessesLeft != 0) { // runs when the word hasn't been guessed yet
                hangmanGraphic(); // displays the hangman graphic
                for (int i = 0; i < multiplayerWordIncomplete.size(); i++) { // displays guessed letters and underscores for unguessed letters
                    cout << multiplayerWordIncomplete.at(i) << " ";
                }
                
                cout << "\n\n\n";
                do {
                    uniqueGuess = 't';
                    cout << "Previous incorrect guesses: ";
                    for (int i = 0; i < multiplayerIncorrectGuesses.size(); i++){
                        cout << multiplayerIncorrectGuesses.at(i); // displays previous incorrect guesses to the user
                        cout << ", ";
                    } // repeats until all incorrect letters in the multiplayerIncorrectGuesses vector are shown to the user
                    cout << "\n";
                    cout << "What is your guess?\n\n\n"; // asks for user guess
                    cin >> multiplayerGuess; // stores user guess
                    uniqueGuessChecker();
                    if (uniqueGuess == 'f') {
                        cout << "You have already guessed this letter, try guessing another one! \n";
                    }
                } while (uniqueGuess == 'f'); // prevents the program from continuing until a unique guess is made
                guessWasCorrect = 'n'; // sets guessWasCorrect as false before it is checked for in the word
                // checks for guess letter in word
                int elem = multiplayerGuess; // sets elem as the guess letter
                
                int i = 0; // defines check starting position
                
                do {
                    
                    while (i <= multiplayerWordIncomplete.size()) { // runs while the entire word hasn't been checked for the letter
                        if (multiplayerWordComplete[i] == elem && multiplayerWordIncomplete[i] != elem) { // if the letter is found in the word and hasn't already been found
                                break;
                            }
                            i++;
                        }
                    
                        if (i <= multiplayerWordIncomplete.size() && multiplayerWordComplete[i] == elem && multiplayerWordIncomplete[i] != elem) { // if the letter is found in the word and hasn't already been found
                            cout << "Letter " << multiplayerGuess << " is the letter in position " << i + 1 // tells the user what position the letter is in
                            << " in the word\n";
                            multiplayerWordIncomplete[i] = multiplayerGuess; // fills in the letter in the incomplete word
                            guessWasCorrect = 'y'; // stores the guess as correct
                            multiplayerCorrectGuesses.push_back(multiplayerGuess); // stores letter in correct guesses array
                        }
                    
                
                } while (guessWasCorrect == 'y' && i < multiplayerWordIncomplete.size()); // returns until entire word is searched for the letter
                
                if (guessWasCorrect == 'n') { // if the letter isn't in the word
                    guessWasCorrect = 'n'; // stores the guess as incorrect
                    numGuessesLeft = numGuessesLeft - 1; // removes 1 guess
                    cout << "The letter " << multiplayerGuess << " is not in the word \n"; // tells Player 1 that the letter was not in the word
                    cout << "You have " << numGuessesLeft << " guesses left. \n"; // tells Player 1 how many guesses remain
                    multiplayerIncorrectGuesses.push_back(multiplayerGuess); // stores letter in incorrect guess array
                    if (numGuessesLeft == 0) { // displays graphic if the number of guesses is 0
                        hangmanGraphic();
                    }
                }
                
            }
            
            // guessChecker
        
        }
        sleep(2);
        victoryOrLossScreen(gamemode, numGuessesLeft, difficulty); // displays information to the user about their victory
        do {
            cout << "Would you like to play again? (y/n) \n"; // asks Player whether they would like to play again
            cin >> playAgain; // stores the player's response
            if (playAgain != 'y' && playAgain != 'Y' && playAgain != 'n' && playAgain != 'N' ) { // tells player they entered an invalid input
                cout << "ERROR: Invalid input! \n";
            }
        } while (playAgain != 'y' && playAgain != 'Y' && playAgain != 'n' && playAgain != 'N'); // asks the user whether they would like to play again until a valid input is entered.
        sleep(2); // pauses program before it restarts and the screen is cleared
        if (playAgain == 'y' || playAgain == 'Y') { // only runs if the user has decided to play again
            cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; // clears the screen
        }
    } while (playAgain == 'y' || playAgain == 'Y'); // reruns the program if the player wants to play again
    
    
    return 0;
}
