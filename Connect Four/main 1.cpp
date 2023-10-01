//
//  Title: Connect Four
//  Description: A program that allows two people to play Connect Four against each other and has an accounts system and leaderboard system.
//  Created by: Alexander Caza
//  Created on: March 3rd, 2022
//  Last Update: April 30th, 2022
//  Known Limitations: Unable to remove "Program ended with exit code: 0" message;
//
///////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <array>
#include <time.h>
#include <vector>
#include <iomanip>

using namespace std;

char loginOrCreateAccount; // stores whether the user chose to log in or create an account
char loginOrQuit; // stores whether the user chose to log in or quit
char loginToAnotherAccountCreateAccountOrQuit; // stores whether the user chose to log in to another account, create a new account or quit
char playLeaderboardsOrQuit; // stores whether Player 1 chose to play the game, view the leaderboards or quit
char finalChallengeConfirmation; // stores the players' decision on whether or not that they are sure they want to challenge each other
char playAgainAccountHomeOrQuit;  // stores whether the players the players would like to play each other again, return to where Player 1 can choose to challenge someone else or view their stats and the leaderboards or quit after playing each other
char playGameResult; // stores the return of the playGame() function
int whereToPlacePiece; // stores where the player wants to put their piece;
int hashedPassword; // stores the user's hashed password
int turnPlayer; // stores which player's turn it is
int piecePlacementResult; // stores the return value of the board.placePiece(int column, int player) function
float valueBeingMoved; // stores the value being moved during the leaderboard sorting process
string creationUsername; // stores the user's desired username during account creation
string creationPassword; // stores the user's desired password during account creation
string loginUsername; // stores the user's username during login
string loginPassword; // stores the user's password during login
string lineBeingSearched; // stores the line being searched
string creationPasswordConfirmation; // stores the user's desired password when they reenter it during the account creation process
string ASCIIPasswordStringNumber; // stores the user's password in number form as it is being converted to a number so that it can be used to seed the random number generator
string whereToPlacePieceInput; // stores the user's input for where they want to put their piece
string usernameBeingMoved; // stores the username being moved during the leaderboard sorting process
bool usernameAlreadyExists; // whether an account with that username already exists
bool passwordIsCorrect; // whether the user entered the correct password
bool hasWon; // stores whether a player has won
bool boardIsFull; // stores whether the board is full
bool sortingFail; // stores whether the leaderboard sorting algorithm reached the end of the vector that is being sorted without running into an incorrectly sorted value
vector<float> leaderboardsGamesWon; // stores the number of games won for each account in the accounts.txt for use during leaderboard display
vector<float> leaderboardsGamesPlayed; // stores the number of games played for each account in the accounts.txt for use during leaderboard display
vector<float> leaderboardsWinRate; // stores the win rate for each account in the accounts.txt for use during leaderboard display
vector<string> leaderboardsUsernamesGamesWon; // stores the username for each account in the accounts.txt for use during the games won leaderboard display
vector<string> leaderboardsUsernamesGamesPlayed; // stores the username for each account in the accounts.txt for use during the games played leaderboard display
vector<string> leaderboardsUsernamesWinRate; // stores the username for each account in the accounts.txt for use during the games played leaderboard display

class Account { // Class Account
public: // Can be accessed outside the class
    string username; // stores the player's username
    long long int hashedPassword; // stores the player's password
    float gamesWon; // stores the number of games the player has won
    float gamesDrawn; // stores the amount of games the player has drawn
    float gamesLost; // stores the number of games the player has lost
    float gamesPlayed; // stores the number of games the player has played
    float winPercentage; // stores the player's win percentage
    float drawPercentage; // stores the player's lose percentage
    float losePercentage; // stores the player's draw percentage
    int counter = 0; // a counter used by the stats update functions
    vector<string> fileUsernames; // stores the usernames from the accounts.txt during stats updating
    vector<string> fileHashedPasswords; // stores the hashed passwords from the accounts.txt during stats updating
    vector<string> fileGamesWon; // stores the games won stats from the accounts.txt during stats updating
    vector<string> fileGamesDrawn; // stores the games drawn stats from the accounts.txt during stats updating
    vector<string> fileGamesLost; // stores the games lost stats from the accounts.txt during stats updating
    vector<string> fileGamesPlayed; // stores the games played from the accounts.txt during stats updating
    
    void loadData(string loginUsername, long long int loginHashedPassword) { // loads the player's account data
        username = loginUsername; // sets the player's username in the account object to the username provided as the parameter
        hashedPassword = loginHashedPassword; // sets the player's password in the account object to the username provided as the parameter
        ifstream accounts;
        accounts.open("accounts.txt"); // opens logins.txt
        while(getline(accounts, lineBeingSearched)) { // loops through each line until the end of the file
            if (lineBeingSearched == username) { // runs when it reaches the player's username in the file
                getline(accounts, lineBeingSearched); // skips to the next line in the file which contains the player's password
                getline(accounts, lineBeingSearched); // gets the next line in the file
                gamesWon = stoi(lineBeingSearched); // sets the gamesWon variable to the value in the file
                getline(accounts, lineBeingSearched); // gets the next line in the file
                gamesPlayed = stoi(lineBeingSearched); // sets the gamesDrawn variable to the value in the file
                getline(accounts, lineBeingSearched); // gets the next line in the file
                gamesLost = stoi(lineBeingSearched); // sets the gamesLost variable to the value in the file
                getline(accounts, lineBeingSearched); // gets the next line in the file
                gamesPlayed = stoi(lineBeingSearched); // sets the gamesPlayed variable to the value in the file
                if (gamesPlayed == 0) { // if the number of games played is 0
                    winPercentage = 0; // sets the winPercentage variable to 0
                    drawPercentage = 0; // sets the winPercentage variable to 0
                    losePercentage = 0; // sets the winPercentage variable to 0
                }
                else {
                    winPercentage = gamesWon / gamesPlayed; // calculates the percentage of games that the player has won using "number of games won / number of games played" and stores it as winPercentage
                    drawPercentage = gamesDrawn / gamesPlayed; // calculates the percentage of games that the player has drawn using "number of games drawn / number of games played" and stores it as drawPercentage
                    losePercentage = gamesLost / gamesPlayed; // calculates the percentage of games that the player has lost using "number of games lost / number of games played" and stores it as drawPercentage
                }
            }
        }
        accounts.close(); // closes the accounts.txt file
    }
    void showStats() {
        loadData(loginUsername, hashedPassword); // loads the player's data from the file to ensure it is updated
        cout << "\nYour Stats: \n";
        cout << "Username: " << username << "\n"; // displays the player's username
        cout << "Number of Games Won: " << gamesWon << "\n"; // displays the number of games the player has won
        cout << "Number of Games Drawn: " << gamesDrawn << "\n"; // displays the number of games the player has drawn
        cout << "Number of Games Lost: " << gamesLost << "\n"; // displays the number of games the player has lost
        cout << "Number of Games Lost: " << gamesPlayed << "\n"; // displays the number of games the player has played
        cout << "Percentage of Games Won: " << winPercentage * 100 << "% \n"; // displays the percentage of games the player has won
        cout << "Percentage of Games Drawn: " << drawPercentage * 100 << "% \n"; // displays the percentage of games the player has drawn
        cout << "Percentage of Games Lost: " << losePercentage * 100 << "% \n\n"; // displays the percentage of games the player has lost
    }
    void winStatsUpdate() { // updates the user's account data if they win a game
        fileUsernames.clear(); // clears any leftover data in the fileUsernames vector
        fileHashedPasswords.clear(); // clears any leftover data in the fileHashedPasswords vector
        fileGamesWon.clear(); // clears any leftover data in the fileGamesWon vector
        fileGamesDrawn.clear(); // clears any leftover data in the fileGamesDrawn vector
        fileGamesLost.clear(); // clears any leftover data in the fileGamesLost vector
        fileGamesPlayed.clear(); // clears any leftover data in the fileGamesPlayed vector
        int counter = 0; // resets the counter
        ifstream accounts;
        accounts.open("accounts.txt"); // opens accounts.txt
        while(getline(accounts, lineBeingSearched)) { // loops through each line until the end of the file
            counter = counter + 1; // increases the counter by 1 each time a new line is being searched
            if (counter == 1) { // if a line containing a username is being scanned
                fileUsernames.push_back(lineBeingSearched); // stores the username in the fileUsernames vector
            }
            else if (counter == 2) { // if a line containing a hashed password is being scanned
                fileHashedPasswords.push_back(lineBeingSearched); // stores the hashed password in the fileUsernames vector
            }
            else if (counter == 3) { // if a line containing a games won stat is being scanned
                fileGamesWon.push_back(lineBeingSearched); // stores the games won stat in the fileGamesWon vector
            }
            else if (counter == 4) { // if a line containing a games drawn stat is being scanned
                fileGamesDrawn.push_back(lineBeingSearched); // stores the games drawn stat in the fileGamesWon vector
            }
            else if (counter == 5) { // if a line containing a games lost stat is being scanned
                fileGamesLost.push_back(lineBeingSearched); // stores the games lost stat in the fileGamesWon vector
            }
            else if (counter == 6) { // if a line containing a games played stat is being scanned
                fileGamesPlayed.push_back(lineBeingSearched); // stores the games played stat in the fileGamesWon vector
            }
            if (counter == 6) { // if the counter reaches 6
                counter = 0; // resets the counter back to 0
            }
        }
        for (int i = 0; i < fileUsernames.size(); i++) { // loops through the usernames in the fileUsernames vector
            if (fileUsernames[i] == username) { // when the username is found in the vector
                fileGamesWon[i] = to_string(stoi(fileGamesWon[i]) + 1); //increases the number of games won stat for the account by 1
                fileGamesPlayed[i] = to_string(stoi(fileGamesPlayed[i]) + 1); //increases the number of games played stat for the account by 1
            }
        }
        accounts.close(); // closes the file
        fstream accountsInsertion;
        accountsInsertion.open("accounts.txt"); // opens accounts.txt and overwrite the contents
        for (int i = 0; i < fileUsernames.size(); i++) { // loops through each account
            accountsInsertion << fileUsernames[i] << "\n"; // inserts the account's username into the file
            accountsInsertion << fileHashedPasswords[i] << "\n"; // inserts the account's hashed password into the file
            accountsInsertion << fileGamesWon[i] << "\n"; // inserts the account's games won stat into the file
            accountsInsertion << fileGamesDrawn[i] << "\n"; // inserts the account's games drawn stat into the file
            accountsInsertion << fileGamesLost[i] << "\n"; // inserts the account's games lost stat into the file
            accountsInsertion << fileGamesPlayed[i] << "\n"; // inserts the account's games played stat into the file
        }
        accountsInsertion.close(); // closes the accounts.txt file
    }
    void drawStatsUpdate() { // updates the user's account data if they draw a game
        fileUsernames.clear(); // clears any leftover data in the fileUsernames vector
        fileHashedPasswords.clear(); // clears any leftover data in the fileHashedPasswords vector
        fileGamesWon.clear(); // clears any leftover data in the fileGamesWon vector
        fileGamesDrawn.clear(); // clears any leftover data in the fileGamesDrawn vector
        fileGamesLost.clear(); // clears any leftover data in the fileGamesLost vector
        fileGamesPlayed.clear(); // clears any leftover data in the fileGamesPlayed vector
        int counter = 0; // resets the counter
        ifstream accounts;
        accounts.open("accounts.txt"); // opens accounts.txt
        while(getline(accounts, lineBeingSearched)) { // loops through each line until the end of the file
            counter = counter + 1; // increases the counter by 1 each time a new line is being searched
            if (counter == 1) { // if a line containing a username is being scanned
                fileUsernames.push_back(lineBeingSearched); // stores the username in the fileUsernames vector
            }
            else if (counter == 2) { // if a line containing a hashed password is being scanned
                fileHashedPasswords.push_back(lineBeingSearched); // stores the hashed password in the fileUsernames vector
            }
            else if (counter == 3) { // if a line containing a games won stat is being scanned
                fileGamesWon.push_back(lineBeingSearched); // stores the games won stat in the fileGamesWon vector
            }
            else if (counter == 4) { // if a line containing a games drawn stat is being scanned
                fileGamesDrawn.push_back(lineBeingSearched); // stores the games drawn stat in the fileGamesDrawn vector
            }
            else if (counter == 5) { // if a line containing a games lost stat is being scanned
                fileGamesLost.push_back(lineBeingSearched); // stores the games lost stat in the fileGamesLost vector
            }
            else if (counter == 6) { // if a line containing a games played stat is being scanned
                fileGamesPlayed.push_back(lineBeingSearched); // stores the games played stat in the fileGamesPlayed vector
            }
            if (counter == 6) { // if the counter reaches 6
                counter = 0; // resets the counter back to 0
            }
        }
        for (int i = 0; i < fileUsernames.size(); i++) { // loops through the usernames in the fileUsernames vector
            if (fileUsernames[i] == username) { // when the username is found in the vector
                fileGamesDrawn[i] = to_string(stoi(fileGamesDrawn[i]) + 1); //increases the number of games drawn stat for the account by 1
                fileGamesPlayed[i] = to_string(stoi(fileGamesPlayed[i]) + 1); //increases the number of games played stat for the account by 1
            }
        }
        accounts.close(); // closes the file
        fstream accountsInsertion;
        accountsInsertion.open("accounts.txt"); // opens accounts.txt and overwrite the contents
        for (int i = 0; i < fileUsernames.size(); i++) { // loops through each account
            accountsInsertion << fileUsernames[i] << "\n"; // inserts the account's username into the file
            accountsInsertion << fileHashedPasswords[i] << "\n"; // inserts the account's hashed password into the file
            accountsInsertion << fileGamesWon[i] << "\n"; // inserts the account's games won stat into the file
            accountsInsertion << fileGamesDrawn[i] << "\n"; // inserts the account's games drawn stat into the file
            accountsInsertion << fileGamesLost[i] << "\n"; // inserts the account's games lost stat into the file
            accountsInsertion << fileGamesPlayed[i] << "\n"; // inserts the account's games played stat into the file
        }
        accountsInsertion.close(); // closes the accounts.txt file
    }
    void loseStatsUpdate() { // updates the user's account data if they lose a game
        fileUsernames.clear(); // clears any leftover data in the fileUsernames vector
        fileHashedPasswords.clear(); // clears any leftover data in the fileHashedPasswords vector
        fileGamesWon.clear(); // clears any leftover data in the fileGamesWon vector
        fileGamesDrawn.clear(); // clears any leftover data in the fileGamesDrawn vector
        fileGamesLost.clear(); // clears any leftover data in the fileGamesLost vector
        fileGamesPlayed.clear(); // clears any leftover data in the fileGamesPlayed vector
        int counter = 0; // resets the counter
        ifstream accounts;
        accounts.open("accounts.txt"); // opens accounts.txt
        while(getline(accounts, lineBeingSearched)) { // loops through each line until the end of the file
            counter = counter + 1; // increases the counter by 1 each time a new line is being searched
            if (counter == 1) { // if a line containing a username is being scanned
                fileUsernames.push_back(lineBeingSearched); // stores the username in the fileUsernames vector
            }
            else if (counter == 2) { // if a line containing a hashed password is being scanned
                fileHashedPasswords.push_back(lineBeingSearched); // stores the hashed password in the fileUsernames vector
            }
            else if (counter == 3) { // if a line containing a games won stat is being scanned
                fileGamesWon.push_back(lineBeingSearched); // stores the games won stat in the fileGamesWon vector
            }
            else if (counter == 4) { // if a line containing a games drawn stat is being scanned
                fileGamesDrawn.push_back(lineBeingSearched); // stores the games drawn stat in the fileGamesDrawn vector
            }
            else if (counter == 5) { // if a line containing a games lost stat is being scanned
                fileGamesLost.push_back(lineBeingSearched); // stores the games lost stat in the fileGamesLost vector
            }
            else if (counter == 6) { // if a line containing a games played stat is being scanned
                fileGamesPlayed.push_back(lineBeingSearched); // stores the games played stat in the fileGamesPlayed vector
            }
            if (counter == 6) { // if the counter reaches 6
                counter = 0; // resets the counter back to 0
            }
        }
        for (int i = 0; i < fileUsernames.size(); i++) { // loops through the usernames in the fileUsernames vector
            if (fileUsernames[i] == username) { // when the username is found in the vector
                fileGamesLost[i] = to_string(stoi(fileGamesLost[i]) + 1); //increases the number of games lost stat for the account by 1
                fileGamesPlayed[i] = to_string(stoi(fileGamesPlayed[i]) + 1); //increases the number of games played stat for the account by 1
            }
        }
        accounts.close(); // closes the file
        fstream accountsInsertion;
        accountsInsertion.open("accounts.txt"); // opens accounts.txt and overwrite the contents
        for (int i = 0; i < fileUsernames.size(); i++) { // loops through each account
            accountsInsertion << fileUsernames[i] << "\n"; // inserts the account's username into the file
            accountsInsertion << fileHashedPasswords[i] << "\n"; // inserts the account's hashed password into the file
            accountsInsertion << fileGamesWon[i] << "\n"; // inserts the account's games won stat into the file
            accountsInsertion << fileGamesDrawn[i] << "\n"; // inserts the account's games drawn stat into the file
            accountsInsertion << fileGamesLost[i] << "\n"; // inserts the account's games lost stat into the file
            accountsInsertion << fileGamesPlayed[i] << "\n"; // inserts the account's games played stat into the file
        }
        accountsInsertion.close(); // closes the accounts.txt file
    }
};

Account player1; // creates an Account object called player1
Account player2; // creates an Account object called player2

class Board { // the Connect Four board
public:
    char board[6][7]; // the Connect Four game board
    Board() { // constructor
        fill(board[0], board[0] + 6 * 7, ' '); // fills the board with spaces
    }
    void displayBoard() { // displays the board
        cout << "-----------------------------\n"; // displays the top of the board
        for (int i = 0; i < 6; i++) { // loops through each row
            cout << "| ";
            for (int j = 0; j < 7; j++) { // loops through each column
                cout << board[i][j] << " | ";
            }
            cout << "\n-----------------------------\n"; // displays the bottom of the board
        }
        cout << "  1   2   3   4   5   6   7\n"; // displays the column numbers at the bottom of the board
    };
    int placePiece(int column, int player) { // places an "X" on the board for Player 1 or a "O" on the board for Player 2 in the column they picked
        for (int i = 5; i > -1; i--) { // loops through each row in the column until an open space is found or it reaches the top of the column
            if (board[i][column] == ' ') { // if the spot in the board is empty
                if (player == 1) { // if it is Player 1's turn
                    board[i][column] = 'X'; // sets the spot in the board to an "X"
                    return 0; // returns "0" if the function was able to place the piece
                }
                else if (player == 2) { // if it is Player 2's turn
                    board[i][column] = 'O'; // sets the spot in the board to an "O"
                    return 0; // returns "0" if the function was able to place the piece
                }
            }
        }
        return 1; // returns "1" if the column was completely full
    };
    bool checkWin(int player) { // checks whether the player won by putting 4 pieces in a row horizontally, vertically or diagonally. Returns "0" if the Player hasn't won and "1" if the has won
        if (player == 1) { // if the player being checked is Player 1
            for (int y = 0; y < 6; y++) { // loops through the rows
                for (int x = 0; x < 4; x++) { // loops through each piece in each row up to the 4th row
                    if (board[y][x] == 'X' && board[y][x + 1] == 'X' && board[y][x + 2] == 'X' && board[y][x + 3] == 'X') { // checks if there are 4 of the Player's pieces in a row horizontally
                        return true; // returns "true" if the player has 4 pieces in a row horizontally and has therefore won
                    }
                }
            }
            for (int x = 0; x < 7; x++) { // loops through the columns
                for (int y = 0; y < 3; y++) { // loops through each piece in each column up to the 3rd column
                    if (board[y][x] == 'X' && board[y + 1][x] == 'X' && board[y + 2][x] == 'X' && board[y + 3][x] == 'X') { // checks if there are 4 of the Player's pieces in a row vertically
                        return true; // returns "true" if the player has 4 pieces in a row vertically and has therefore won
                    }
                }
            }
            for (int y = 0; y < 3; y++) { // loops through the rows up to the 3rd row
                for (int x = 0; x < 4; x++) { // loops through each piece in each row up to the 5th column
                    if (board[y][x] == 'X' && board[y + 1][x + 1] == 'X' && board[y + 2][x + 2] == 'X' && board[y + 3][x + 3] == 'X') { // checks if there are 4 of the Player's pieces in a row diagonally going from the top left to the bottom right
                        return true; // returns "true" if the player has 4 pieces in a row diagonally going from the top left to the bottom right and has therefore won
                    }
                }
            }
            for (int y = 5; y > 2; y--) { // loops through each piece in rows 6 to 4
                for (int x = 0; x < 4; x++) { // loops through each piece in each row up to the 5th column
                    if (board[y][x] == 'X' && board[y - 1][x + 1] == 'X' && board[y - 2][x + 2] == 'X' && board[y - 3][x + 3] == 'X') { // checks if there are 4 of the Player's pieces in a row diagonally going from the bottom left to the top right
                        return true; // returns "true" if the player has 4 pieces in a row diagonally going from the bottom left to the top right and has therefore won
                    }
                }
            }
        }
        if (player == 2) { // if the player being checked is Player 2
            for (int y = 0; y < 6; y++) { // loops through the rows
                for (int x = 0; x < 4; x++) { // loops through each piece in each row up to the 4th row
                    if (board[y][x] == 'O' && board[y][x + 1] == 'O' && board[y][x + 2] == 'O' && board[y][x + 3] == 'O') { // checks if there are 4 of the Player's pieces in a row horizontally
                        return true; // returns "true" if the player has 4 pieces in a row horizontally and has therefore won
                    }
                }
            }
            for (int x = 0; x < 7; x++) { // loops through the columns
                for (int y = 0; y < 3; y++) { // loops through each piece in each column up to the 3rd column
                    if (board[y][x] == 'O' && board[y + 1][x] == 'O' && board[y + 2][x] == 'O' && board[y + 3][x] == 'O') { // checks if there are 4 of the Player's pieces in a row vertically
                        return true; // returns "true" if the player has 4 pieces in a row vertically and has therefore won
                    }
                }
            }
            for (int y = 0; y < 3; y++) { // loops through the rows up to the 3rd row
                for (int x = 0; x < 4; x++) { // loops through each piece in each row up to the 5th column
                    if (board[y][x] == 'O' && board[y + 1][x + 1] == 'O' && board[y + 2][x + 2] == 'O' && board[y + 3][x + 3] == 'O') { // checks if there are 4 of the Player's pieces in a row diagonally going from the top left to the bottom right
                        return true; // returns "true" if the player has 4 pieces in a row diagonally going from the top left to the bottom right and has therefore won
                    }
                }
            }
            for (int y = 5; y > 2; y--) { // loops through each piece in rows 6 to 4
                for (int x = 0; x < 4; x++) { // loops through each piece in each row up to the 5th column
                    if (board[y][x] == 'O' && board[y - 1][x + 1] == 'O' && board[y - 2][x + 2] == 'O' && board[y - 3][x + 3] == 'O') { // checks if there are 4 of the Player's pieces in a row diagonally going from the bottom left to the top right
                        return true; // returns "true" if the player has 4 pieces in a row diagonally going from the bottom left to the top right and has therefore won
                    }
                }
            }
        }
        return false; // returns "false" if 4 of the player's pieces are not found in a row horizontally, vertically or diagonally
    }
    bool checkIsFull() { // checks if the entire board is filled and returns "false" if it isn't full and "true" if it is full
        for (int x = 0; x < 7; x++) { // loops through each column
            if (board[0][x] == ' ') { // checks if the top spot of each column is still empty
                return false; // returns "false" if an empty spot at the top of a column is found
            }
        }
        return true; // returns "true" if no empty spot is found at the top of any of the columns
    }
};

void generateLoginsFile() { // generates the logins.txt file if it doesn't exist yet
    // tries to open logins.txt to read
    ifstream ifile;
    ifile.open("logins.txt");
    if (ifile) { // checks if the logins.txt file already exists and if it does, it goes back to main()
        return;
    }
    else {
        ofstream MyFile("logins.txt"); // if the file doesn't exist, it creates and opens logins.txt
        // Closes the file
        MyFile.close();
    }
}

void generateAccountsFile() { // generates the accounts.txt file if it doesn't exist yet
    // tries to open accounts.txt to read
    ifstream ifile;
    ifile.open("accounts.txt");
    if (ifile) { // checks if the accounts.txt file already exists and if it does, it goes back to main()
        return;
    }
    else {
        ofstream MyFile("accounts.txt"); // if the file doesn't exist, it creates and opens questions.txt
        // Closes the file
        MyFile.close();
        return;
    }
}

bool queryLogins(string query) { // Checks if the "query" string is a line in the logins.txt file
    ifstream logins;
    logins.open("logins.txt"); // opens logins.txt
    while(getline(logins, lineBeingSearched)) { // loops through each line until the end of the file
        if (lineBeingSearched == query) { // if the "query" string is a line, returns true and breaks the loop
            logins.close(); // closes the file
            return true; // returns true and breaks the loop
        }
    }
    logins.close(); // closes the file if the "query" string is not found
    return false; // returns false
}

bool checkPassword(string username, string password) { // Checks if the hashed password in the logins.txt file for the account name that given matches the hashed password created during the login process
    ifstream logins;
    logins.open("logins.txt"); // opens logins.txt
    while(getline(logins, lineBeingSearched)) { // loops through each line until it finds the line containing the username of the user
        if (lineBeingSearched == username) { // runs when the username is reached in the file
            getline(logins, lineBeingSearched); // gets the line after the username, which is the hashed password
            if (lineBeingSearched == password) { // returns true if the hashed password in the logins.txt file matches the hashed password that was geneerated during the login process
                logins.close(); // closes the file
                return true; // returns true
            }
            else if (lineBeingSearched != password) { // returns false if the hashed password in the logins.txt file does not match the hashed password that was geneerated during the login process
                logins.close(); // closes the file
                
                return false; // returns false
            }
        }
    }
    return false; // returns false if the while loop reaches the end of the file before finding the username
}

int stringToASCII(string unconvertedPassword) {
    ASCIIPasswordStringNumber.clear(); // clears the string
    for (int i = 0; i < unconvertedPassword.length(); i++) { // loops through each character in the user's password
        ASCIIPasswordStringNumber = ASCIIPasswordStringNumber + to_string(int(unconvertedPassword.at(i) % 9)); // converts each char to it's ascii value and then gets the remainder of that value when divided by 9 and adds number onto the end of ASCIIPasswordStringNumber
    }
    return stoll(ASCIIPasswordStringNumber) % 1233483823; // converts the ASCIIPasswordStringNumber string to a long long int and then divides it by 1233483823 and returns that number
}

void createAccount(int player) {
    do {
        cout << "What would you like your username to be? \n"; // asks the user what they would like their username to be
        cin >> creationUsername; // stores the user's desired username as creationUsername
        usernameAlreadyExists = queryLogins(creationUsername); // checks if an account with that name already exist and stores the result in usernameAlreadyExists. Returns true if an account with that name already exists. Returns false if an account with that username does not exist.
        if (usernameAlreadyExists == true) { // if an account with that username already exists
            cout << "Sorry, an account with this name already exists! Please pick a different username. \n"; // tells the user that an account with that username already exists and asks them to pick a different username
        }
    } while (usernameAlreadyExists == true); // if an account with that username already exists, loops back to the start to ask the user to enter a different username
    do {
        cout << "What would you like your password to be? \n"; // akss the user what they would like their password to be
        cin >> creationPassword; // stores their desired password as creationPassword
        cout << "Please reenter your desired password. \n"; // asks the user to reenter their desired password
        cin >> creationPasswordConfirmation; // stores their reentered password as creationPasswordConfirmation
        if (creationPassword != creationPasswordConfirmation) { // if the two passwords do not match
            cout << "Sorry, those passwords do not match! Please try entering your password again. \n"; // tells the user that the two passwords do not match and asks them to try entering it again
        }
    } while (creationPassword != creationPasswordConfirmation); // loops if the passwords do not match
    srand(stringToASCII(creationPassword)); // converts creationpassword to an integer and then uses that as the seed for any random numbers generated
    hashedPassword = rand() % 1000000000; // generates a random number between 1 and 1000000000 using the user password's as the seed
    ofstream logins;
    logins.open("logins.txt", std::ios_base::app); // opens logins.txt at the end of the file
    logins << creationUsername << "\n"; // stores the player's username at the end of login.txt file
    logins << hashedPassword << "\n"; // stores the player's hashed password at the end of login.txt file
    logins.close(); // closes the logins.txt file
    ofstream accounts;
    accounts.open("accounts.txt", std::ios_base::app); // opens logins.txt at the end of the file
    accounts << creationUsername << "\n"; // stores the player's username at the end of login.txt file
    accounts << hashedPassword << "\n"; // stores the player's hashed password at the end of login.txt file
    accounts << "0" << "\n"; // stores the number of games the player has won, which is 0, at the end of login.txt file
    accounts << "0" << "\n"; // stores the number of games the player has drawn, which is 0, at the end of login.txt file
    accounts << "0" << "\n"; // stores the number of games the player has lost, which is 0, at the end of login.txt file
    accounts << "0" << "\n"; // stores the number of games the player has played, which is 0, at the end of login.txt file
    do { // repeats until the user enters a valid input for loginOrQuit
        cout << "Would you like to log in to your account or quit the program. Type \"L\" to login to your account or \"Q\" to quit the program. \n"; // asks the user if they would like to login to their account or quit the program
        cin >> loginOrQuit; // gets the user's decision and stores it as loginOrQuit
        if (loginOrQuit != 'L' && loginOrQuit != 'l' && loginOrQuit != 'Q' && loginOrQuit != 'q') { // if the user enters an invalid input
            cout << "Sorry, this is not a valid input. Please try again. \n"; // tells the user that they entered an invalid input and asks them to try entering again
        }
    } while (loginOrQuit != 'L' && loginOrQuit != 'l' && loginOrQuit != 'Q' && loginOrQuit != 'q'); // loops if the user enters an invalid input
    if (loginOrQuit == 'L' || loginOrQuit == 'l') { // if the user decides to log in to their account
        if (player == 1) { // if the player logging in is Player 1
            player1.loadData(creationUsername, hashedPassword); // creates an object of Account named player1 with the account's username and password as parameters
            passwordIsCorrect = true; // sets passwordIsCorrect to true
            return;
        }
        if (player == 2) { // if the player logging in is Player 2
            player2.loadData(creationUsername, hashedPassword); // creates an object of Account named player2 with the account's username and password as parameters
            passwordIsCorrect = true; // sets passwordIsCorrect to true
            return;
        }
    }
    else if (loginOrQuit == 'Q' || loginOrQuit == 'q')
    {
        return;
    }
}

void login(int player) {
    do {
        usernameAlreadyExists = false; // sets usernameAlreadyExists to a value that does nothing if the loop repeats
        do { // loops if Player 2 tries to log into the account that Player 1 has logged into
            cout << "What is the username of the account that you would like to log in to? \n"; // asks the user what the username of the account they would like to log in to is
            cin >> loginUsername; // stores the username of the account the user is trying to log in to
            if (player == 2 && player1.username == loginUsername) { // if Player 2 tries to log into the account that Player 1 has logged into
                cout << "Sorry, Player 1 has already logged into this account, please log into a different account! \n"; // tells the user Player 1 has already logged into this account and asks them to log into a different account
            } // loops if Player 2 tries to log into the account that Player 1 has logged into
        } while (player == 2 && player1.username == loginUsername);
        usernameAlreadyExists = queryLogins(loginUsername); // checks if an account with that name and stores the result in usernameAlreadyExists. Returns true if an account with that name exists. Returns false if an account with that username does not exist.
        if (usernameAlreadyExists == false) { // if an account with that username does not exist
            if (player == 1) { // if Player 1 is logging in
                do { // repeats until the user enters a valid input for loginToAnotherAccountCreateAccountOrQuit
                    cout << "Sorry, an account with this name does not exist! Press \"L\" log into a different account, \"C\" create a new account or \"Q\" to quit the program. \n"; // tells Player 1 that an account with that username does not exist and gives them the option to log into a different account, create a new account or quit the program
                    cin >> loginToAnotherAccountCreateAccountOrQuit;
                    if (loginToAnotherAccountCreateAccountOrQuit != 'L' && loginToAnotherAccountCreateAccountOrQuit != 'l' && loginToAnotherAccountCreateAccountOrQuit != 'C' && loginToAnotherAccountCreateAccountOrQuit != 'c' && loginToAnotherAccountCreateAccountOrQuit != 'Q' && loginToAnotherAccountCreateAccountOrQuit != 'q') { // if the user enters an invalid input
                        cout << "Sorry, this is not a valid input. Please try again. \n"; // tells the user that they entered an invalid input and asks them to try entering it again
                    }
                } while (loginToAnotherAccountCreateAccountOrQuit != 'L' && loginToAnotherAccountCreateAccountOrQuit != 'l' && loginToAnotherAccountCreateAccountOrQuit != 'C' && loginToAnotherAccountCreateAccountOrQuit != 'c' && loginToAnotherAccountCreateAccountOrQuit != 'Q' && loginToAnotherAccountCreateAccountOrQuit != 'q'); // loops if the user enters an invalid input
                if (loginToAnotherAccountCreateAccountOrQuit == 'C' || loginToAnotherAccountCreateAccountOrQuit == 'c') {
                    createAccount(player);
                    return; // leaves the function after they create an account
                }
                else if (loginToAnotherAccountCreateAccountOrQuit == 'Q' || loginToAnotherAccountCreateAccountOrQuit == 'q') {
                    return;
                }
            }
            if (player == 2) { // if Player 2 is logging in
                do { // repeats until the user enters a valid input for loginToAnotherAccountCreateAccountOrQuit
                    loginToAnotherAccountCreateAccountOrQuit = 'f'; // unsets the loginToAnotherAccountCreateAccountOrQuit variable
                    cout << "Sorry, an account with this name does not exist! Press \"L\" log into a different account or \"C\" create a new account. \n"; // tells Player 2 that an account with that username does not exist and gives them the option to log into a different account or create a new account
                    cin >> loginToAnotherAccountCreateAccountOrQuit;
                    if (loginToAnotherAccountCreateAccountOrQuit != 'L' && loginToAnotherAccountCreateAccountOrQuit != 'l' && loginToAnotherAccountCreateAccountOrQuit != 'C' && loginToAnotherAccountCreateAccountOrQuit != 'c' && loginToAnotherAccountCreateAccountOrQuit != 'Q' && loginToAnotherAccountCreateAccountOrQuit != 'q') { // if the user enters an invalid input
                        cout << "Sorry, this is not a valid input. Please try again. \n" ; // tells the user that they entered an invalid input and asks them to try entering it again
                    }
                } while (loginToAnotherAccountCreateAccountOrQuit != 'L' && loginToAnotherAccountCreateAccountOrQuit != 'l' && loginToAnotherAccountCreateAccountOrQuit != 'C' && loginToAnotherAccountCreateAccountOrQuit != 'c'); // loops if the user enters an invalid input
                if (loginToAnotherAccountCreateAccountOrQuit == 'C' || loginToAnotherAccountCreateAccountOrQuit == 'c') {
                    createAccount(player);
                    return; // leaves the function after they create an account
                }
            }
        }
    } while (loginToAnotherAccountCreateAccountOrQuit == 'L' || (loginToAnotherAccountCreateAccountOrQuit == 'l' && usernameAlreadyExists == false)); // if an account with that username already exists and the user decided to log into a different account, loops back to the start to ask the user to enter a different username
    do {
        do {
            loginToAnotherAccountCreateAccountOrQuit = 'w'; // sets loginToAnotherAccountCreateAccountOrQuit to a character that doesn't do anything
            cout << "What is your password? \n"; // asks the user what their password is
            cin >> loginPassword; // stores the password that the user inputs password as a string
            srand(stringToASCII(loginPassword)); // converts loginPassword to an integer using the same process that is used during account creation and then uses that as the seed for any random numbers generated
            hashedPassword = rand() % 1000000000; // generates a random number between 1 and 1000000000 using the user password  as the seed
            passwordIsCorrect = checkPassword(loginUsername, to_string(hashedPassword)); // checks if the hashed password generated during the login process matches the one in the login.txt file
            if (passwordIsCorrect == false) { // if the user enters an incorrect password
                if (player == 1) { // if Player 1 is logging in
                    do { // repeats until the user enters a valid input for loginToAnotherAccountCreateAccountOrQuit
                        cout << "Sorry, this is not the correct password! Press \"T\" to try a different password, \"C\" create a new account or \"Q\" to quit the program. \n"; // tells Player     1 that they entered the wrong password and gives them the option to try a different password, create a new account or quit the program
                        cin >> loginToAnotherAccountCreateAccountOrQuit;
                        if (loginToAnotherAccountCreateAccountOrQuit != 'T' && loginToAnotherAccountCreateAccountOrQuit != 't' && loginToAnotherAccountCreateAccountOrQuit != 'C' && loginToAnotherAccountCreateAccountOrQuit != 'c' && loginToAnotherAccountCreateAccountOrQuit != 'Q' && loginToAnotherAccountCreateAccountOrQuit != 'q') { // if the user enters an invalid input
                            cout << "Sorry, this is not a valid input. Please try again. \n"; // tells the user that they entered an invalid input and asks them to try entering it again
                        }
                    } while (loginToAnotherAccountCreateAccountOrQuit != 'T' && loginToAnotherAccountCreateAccountOrQuit != 't' && loginToAnotherAccountCreateAccountOrQuit != 'C' && loginToAnotherAccountCreateAccountOrQuit != 'c' && loginToAnotherAccountCreateAccountOrQuit != 'Q' && loginToAnotherAccountCreateAccountOrQuit != 'q'); // loops if the user enters an invalid input
                    if (loginToAnotherAccountCreateAccountOrQuit == 'C' || loginToAnotherAccountCreateAccountOrQuit == 'c') {
                        createAccount(player);
                    }
                    else if (loginToAnotherAccountCreateAccountOrQuit == 'Q' || loginToAnotherAccountCreateAccountOrQuit == 'q') {
                        return;
                    }
                }
                if (player == 2) { // if Player 2 is logging in
                    do { // repeats until the user enters a valid input for loginToAnotherAccountCreateAccountOrQuit
                        cout << "Sorry, this is not the correct password! Press \"T\" to try a different password or \"C\" create a new account. \n"; // tells Player 2 that they entered the wrong password and gives them the option to try a different password or create a new account
                        cin >> loginToAnotherAccountCreateAccountOrQuit;
                        if (loginToAnotherAccountCreateAccountOrQuit != 'T' && loginToAnotherAccountCreateAccountOrQuit != 't' && loginToAnotherAccountCreateAccountOrQuit != 'C' && loginToAnotherAccountCreateAccountOrQuit != 'c' && loginToAnotherAccountCreateAccountOrQuit != 'Q' && loginToAnotherAccountCreateAccountOrQuit != 'q') { // if the user enters an invalid input
                            cout << "Sorry, this is not a valid input. Please try again. \n"; // tells the user that they entered an invalid input and asks them to try entering it again
                        }
                    } while (loginToAnotherAccountCreateAccountOrQuit != 'T' && loginToAnotherAccountCreateAccountOrQuit != 't' && loginToAnotherAccountCreateAccountOrQuit != 'C' && loginToAnotherAccountCreateAccountOrQuit != 'c'); // loops if the user enters an invalid input
                    if (loginToAnotherAccountCreateAccountOrQuit == 'C' || loginToAnotherAccountCreateAccountOrQuit == 'c') {
                        createAccount(player);
                    }
                }
            }
        } while (loginToAnotherAccountCreateAccountOrQuit == 'T' || loginToAnotherAccountCreateAccountOrQuit == 't');
    } while (passwordIsCorrect == false); // reasks the user what their password is if they get it wrong and say that they would like to try entering a different password
    if (player == 1) { // if the player logging in is Player 1
        player1.loadData(loginUsername, hashedPassword); // creates an object of Account named player1 with the account's username and password as parameters
        passwordIsCorrect = true; // sets passwordIsCorrect to true
    }
    if (player == 2) { // if the player logging in is Player 2
        player2.loadData(loginUsername, hashedPassword); // creates an object of Account named player2 with the account's username and password as parameters
        passwordIsCorrect = true; // sets passwordIsCorrect to true
    }
}

char playGame() {
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; // clears the screen
    do { // repeats until the user enters a valid input for loginOrCreateAccount
        cout << "You have selected to play the game. You are Player 1. Player 2 now needs to login. \n"; // tells the user they have selected to play the game and are Player 2 and tells Player 1 that Player 2 needs to login now.
        cout << "Welcome Player 2! Press \"L\" to login or \"C\" to create an new account. \n"; // asks Player 2 if they would like to log in or create a new account
        cin >> loginOrCreateAccount; // stores their answer as loginOrCreateAccount
        if (loginOrCreateAccount == 'L' || loginOrCreateAccount == 'l') { // calls the login() function with int player = 2 as a parameter if Player 2 wants to log in
            login(2);
        }
        else if(loginOrCreateAccount == 'C' || loginOrCreateAccount == 'c') { // calls the createAccount function with int player = 1 as a parameter if the user wants to log in
            createAccount(2);
        }
        if (loginOrCreateAccount != 'L' && loginOrCreateAccount != 'l' && loginOrCreateAccount != 'C' && loginOrCreateAccount != 'c') { // if Player 2 enters an invalid input
            cout << "Sorry, this is not a valid input. Please try again. \n"; // tells the user that they entered an invalid input and asks them to try entering it again
        }
    } while (loginOrCreateAccount != 'L' && loginOrCreateAccount != 'l' && loginOrCreateAccount != 'C' && loginOrCreateAccount != 'c'); // loops if the user enters an invalid input
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; // clears the console so that Player 2's password isn't shown
    cout << "You are now logged in as Player 2. \n";
    do { // loops if the players decide to play again
        playAgainAccountHomeOrQuit = 'z'; // unsets the playAgainAccountHomeOrQuit value on loop
        do { // loops if the players enter an invalid input
            cout << "Are you both sure you would like to challenge each other to a game of Connect Four? Enter \"Y\" to confirm or \"N\" if you do not want to challenge each other. \n"; // asks the Players whether they are sure they would like to challenge each other
            cin >> finalChallengeConfirmation; // stores the Players' decision
            if (finalChallengeConfirmation != 'Y' && finalChallengeConfirmation != 'y' && finalChallengeConfirmation != 'N' && finalChallengeConfirmation != 'n') {
                cout << "Sorry, this is not a valid input. Please try again. \n"; // tells the Players that they entered an invalid input and asks them to try entering it again
            }
        } while (finalChallengeConfirmation != 'Y' && finalChallengeConfirmation != 'y' && finalChallengeConfirmation != 'N' && finalChallengeConfirmation != 'n'); // loops if the players enter an invalid input
        if (finalChallengeConfirmation == 'N' || finalChallengeConfirmation == 'n') { // if the users decide not to challenge each other
            cout << "Cancelling the challenge. You are still logged in, Player 1. \n";
            return 'c'; // returns 'c' if the players decide not to challenge each other
        }
        Board board; // generates a new empty Connect Four board
        srand(time(0)); // seeds the random number generator with the current time so that the resulting numbers are different every time the program is run
        turnPlayer = rand() % 2 + 1; // picks either 1 or 2 randomly to determine which player will move first
        cout << "You will be making the first move Player " << turnPlayer << ". You will play as the \"X\"s and Player 2 will play as the \"O\"s. \n"; // tells the Players who will will be making the first move.
        do { // keeps repeating until a player wins
            do {
                do {
                    board.displayBoard(); // displays the board to the players
                    cout << "Player " << turnPlayer << " please enter the number of the column where you would like to put your piece. \n"; // asks the player in which column they would like to place their piece
                    cin >> whereToPlacePieceInput; // stores the player's input on where they would like to put their piece
                    if (whereToPlacePieceInput != "1" && whereToPlacePieceInput != "2" && whereToPlacePieceInput != "3" && whereToPlacePieceInput != "4" && whereToPlacePieceInput != "5" && whereToPlacePieceInput != "6" && whereToPlacePieceInput != "7") { // if the user does not enter a valid input
                        cout << "Sorry, that is not a valid column number, please try inputting where you would like to put your piece again. \n"; // tells the user they entered an invalid input
                    }
                } while (whereToPlacePieceInput != "1" && whereToPlacePieceInput != "2" && whereToPlacePieceInput != "3" && whereToPlacePieceInput != "4" && whereToPlacePieceInput != "5" && whereToPlacePieceInput != "6" && whereToPlacePieceInput != "7"); // repeats until the user enters a valid column number
                whereToPlacePiece = stoi(whereToPlacePieceInput);
                piecePlacementResult = board.placePiece(whereToPlacePiece - 1, turnPlayer); // places the piece in the column where the player wants to put it if possible and stores the return value as placePlacementResult. The result wll be "0" if the piece was sucressfully placed and "1" if the column was already full
                if (piecePlacementResult == 1) { // if the piece cannot be placed because the column is full
                    cout << "Sorry, that column is full so you cannot place your piece there. Please pick a different column to place your piece in. \n"; // tells the player the column was full and asks them to pick a different column to place their piece in
                }
            } while (piecePlacementResult == 1); // repeats if the column where the player decided to place their piece is full
            hasWon = board.checkWin(turnPlayer); // checks whether the player who just made their turn has won and returns "true" if they have and "false" if they haven't
            boardIsFull = board.checkIsFull(); // checks whether the board is full and so it is a draw and returns "true" if the board is full and the game is drawn or "false" if it isn't full yet and so the game can continue
            if (hasWon == false) { // if the player who just played didn't win
                if (boardIsFull == true) { // if the board is full and no one won
                    cout << "The board is full and so the game is a draw! \n"; // tells the players that the board is full and that the game is a draw
                }
                else if (turnPlayer == 1) { // if the player who just played is Player 1
                    turnPlayer = 2; // makes Player 2 the player who makes the next move
                    cout << "It is now Player 2's turn to place a piece. \n"; // tells Player 2 it is now their turn to place a piece
                }
                else if (turnPlayer == 2) { // if the player who just played is Player 2
                    turnPlayer = 1; // makes Player 1 the player who makes the next move
                    cout << "It is now Player 1's turn to place a piece. \n"; // tells Player 2 it is now their turn to place a piece
                }
            }
        } while (hasWon == false && boardIsFull == false); // keeps repeating until a player wins or the game is drawn
        board.displayBoard(); // displays the board to the players
        if (hasWon == true && turnPlayer == 1) {
            cout << "Congratulations Player 1, you won the game! Player 2 lost! \n";
            player1.winStatsUpdate(); // adds the win to player 1's account stats
            player2.loseStatsUpdate(); // adds the lose to player 2's account stats
        }
        else if (hasWon == true && turnPlayer == 2) {
            cout << "Congratulations Player 2, you won the game! Player 1 lost! \n";
            player1.loseStatsUpdate(); // adds the lose to player 1's account stats
            player2.winStatsUpdate(); // adds the win to player 2's account stats
        }
        else if (hasWon == false && boardIsFull == true) {
            cout << "The board is full and no one won. The game is a draw! \n";
            player1.drawStatsUpdate(); // adds the draw to player 1's account stats
            player2.drawStatsUpdate(); // adds the draw to player 2's account stats
        }
        do {
            cout << "Press \"A\" to play each other again, \"B\" to return to where Player 1 can choose to challenge someone else or view their stats and the leaderboards or \"Q\" to quit. \n"; // asks the players if they would like to play each other again, return to where Player 1 can choose to challenge someone else or view their stats and the leaderboards or quit
            cin >> playAgainAccountHomeOrQuit; // stores the players' choice
            if (playAgainAccountHomeOrQuit != 'A' && playAgainAccountHomeOrQuit != 'a' &&  playAgainAccountHomeOrQuit != 'B' && playAgainAccountHomeOrQuit != 'b' && playAgainAccountHomeOrQuit != 'Q' && playAgainAccountHomeOrQuit != 'q') { // if the players enter an invalid input
                cout << "Sorry that is not a valid input. Please try again. \n"; // tells the players they entered an invalid input
            }
        } while (playAgainAccountHomeOrQuit != 'A' && playAgainAccountHomeOrQuit != 'a' &&  playAgainAccountHomeOrQuit != 'B' && playAgainAccountHomeOrQuit != 'b' && playAgainAccountHomeOrQuit != 'Q' && playAgainAccountHomeOrQuit != 'q'); // loops if players enter an invalid input
    } while (playAgainAccountHomeOrQuit == 'A' || playAgainAccountHomeOrQuit == 'a'); // loops if the players decide to play each other again
    return 'e'; // returns 'e' if the game ends
}

void showLeaderboards() {  // shows the leaderboards for the palyers with the most games won, most games played and highest percentage of games won
    leaderboardsGamesWon.clear(); // clears any leftover data in the leaderboardsGamesWon vector
    leaderboardsGamesPlayed.clear(); // clears any leftover data in the leaderboardsGamesPlayed vector
    leaderboardsWinRate.clear(); // clears any leftover data in the leaderboardsWinRate vector
    leaderboardsUsernamesGamesWon.clear(); // clears any leftover data in the leaderboardsUsernamesGamesWon vector
    leaderboardsUsernamesGamesPlayed.clear(); // clears any leftover data in the leaderboardsUsernamesGamesPlayed vector
    leaderboardsUsernamesWinRate.clear(); // clears any leftover data in the fileGamesPlayed vector
    int counter = 0; // defines a counter
    ifstream accounts;
    accounts.open("accounts.txt"); // opens logins.txt
    while(getline(accounts, lineBeingSearched)) { // loops through each line until the end of the file
        counter = counter + 1; // increases the counter by 1 each time a new line is being searched
        if (counter == 1) { // if a line containing a username is being scanned
            leaderboardsUsernamesGamesWon.push_back(lineBeingSearched); // stores the username in the leaderboardsUsernamesGamesWon vector
        }
        else if (counter == 3) { // if a line containing the number of games won for an account is being scanned
            leaderboardsGamesWon.push_back(stoi(lineBeingSearched)); // stores the number of games won in the leaderboardsUsernamesGamesWon vector
        }
        else if (counter == 6) { // if a line containing the number of games played for an account is being scanned
            leaderboardsGamesPlayed.push_back(stoi(lineBeingSearched)); // stores the number of games played in the leaderboardsUsernamesGamesWon vector
        }
        if (counter == 6) { // if the counter reaches 6
            counter = 0; // resets the counter back to 0
        }
    }
    leaderboardsUsernamesGamesPlayed = leaderboardsUsernamesGamesWon; // copies the contents of the leaderboardsUsernamesGamesWon vector containing the usernames to the leaderboardsUsernamesGamesPlayed
    leaderboardsUsernamesWinRate = leaderboardsUsernamesGamesWon; // copies the contents of the leaderboardsUsernamesGamesWon vector containing the usernames to the leaderboardsUsernamesWinRate
    for (int i = 0; i < leaderboardsGamesWon.size(); i++) { // loops through each account
        setprecision(1); // sets the maximum number of digits as the result of calculations to 1
        if (leaderboardsGamesPlayed[i] == 0) { // if the umber of games played by the account is 0
            leaderboardsWinRate.push_back(0); // sets the account's win rate to 0
        }
        else {
            leaderboardsWinRate.push_back(leaderboardsGamesWon[i] / leaderboardsGamesPlayed[i]); // calculates the win rate using "Games Won / Games Played" and then stores the number of games played in the leaderboardsUsernamesGamesWon vector
        }
    }
    do { // loops if the leaderboard sorting algorithm runs into an incorrectly sorted value before it reaches the end of the leaderboardsGamesWon vector
        sortingFail = false; // unsets the sortingFail value on loop
        for (int i = 0; i < leaderboardsGamesWon.size(); i++) { // loops through each value in the leaderboardsGamesWon vector
            if (leaderboardsGamesWon[i] < leaderboardsGamesWon[i + 1]) { // if the sorting algorithm finds an unsorted value
                sortingFail = true; // sets sortingFail to "true"
                valueBeingMoved = leaderboardsGamesWon[i]; // stores the value being moved in the vector
                leaderboardsGamesWon.erase(leaderboardsGamesWon.begin() + i); // erases the value from the vector
                leaderboardsGamesWon.insert(leaderboardsGamesWon.begin() + i + 1, valueBeingMoved); // inserts the value into the array one position further to the right
                usernameBeingMoved = leaderboardsUsernamesGamesWon[i]; // stores the username of the account who's value got moved in the vector so that the username can be moved
                leaderboardsUsernamesGamesWon.erase(leaderboardsUsernamesGamesWon.begin() + i); // erases the username from the username vector
                leaderboardsUsernamesGamesWon.insert(leaderboardsUsernamesGamesWon.begin() + i + 1, usernameBeingMoved); // inserts the username into the username array one position further to the right
            }
        }
    } while (sortingFail == true);
    do { // loops if the leaderboard sorting algorithm runs into an incorrectly sorted value before it reaches the end of the leaderboardsGamesPlayed vector
        sortingFail = false; // unsets the sortingFail value on loop
        for (int i = 0; i < leaderboardsGamesPlayed.size(); i++) { // loops through each value in the leaderboardsGamesPlayed vector
            if (leaderboardsGamesPlayed[i] < leaderboardsGamesPlayed[i + 1]) { // if the sorting algorithm finds an unsorted value
                sortingFail = true; // sets sortingFail to "true"
                valueBeingMoved = leaderboardsGamesPlayed[i]; // stores the value being moved in the vector
                leaderboardsGamesPlayed.erase(leaderboardsGamesPlayed.begin() + i); // erases the value from the vector
                leaderboardsGamesPlayed.insert(leaderboardsGamesPlayed.begin() + i + 1, valueBeingMoved); // inserts the value into the array one position further to the right
                usernameBeingMoved = leaderboardsUsernamesGamesPlayed[i]; // stores the username of the account who's value got moved in the vector so that the username can be moved
                leaderboardsUsernamesGamesPlayed.erase(leaderboardsUsernamesGamesPlayed.begin() + i); // erases the username from the username vector
                leaderboardsUsernamesGamesPlayed.insert(leaderboardsUsernamesGamesPlayed.begin() + i + 1, usernameBeingMoved); // inserts the username into the username array one position further to the right
            }
        }
    } while (sortingFail == true);
    do { // loops if the leaderboard sorting algorithm runs into an incorrectly sorted value before it reaches the end of the leaderboardsGamesPlayed vector
        sortingFail = false; // unsets the sortingFail value on loop
        for (int i = 0; i < leaderboardsWinRate.size(); i++) { // loops through each value in the leaderboardsGamesPlayed vector
            if (leaderboardsWinRate[i] < leaderboardsWinRate[i + 1]) { // if the sorting algorithm finds an unsorted value
                sortingFail = true; // sets sortingFail to "true"
                valueBeingMoved = leaderboardsWinRate[i]; // stores the value being moved in the vector
                leaderboardsWinRate.erase(leaderboardsWinRate.begin() + i); // erases the value from the vector
                leaderboardsWinRate.insert(leaderboardsWinRate.begin() + i + 1, valueBeingMoved); // inserts the value into the array one position further to the right
                usernameBeingMoved = leaderboardsUsernamesWinRate[i]; // stores the username of the account who's value got moved in the vector so that the username can be moved
                leaderboardsUsernamesWinRate.erase(leaderboardsUsernamesWinRate.begin() + i); // erases the username from the username vector
                leaderboardsUsernamesWinRate.insert(leaderboardsUsernamesWinRate.begin() + i + 1, usernameBeingMoved); // inserts the username into the username array one position further to the right
            }
        }
    } while (sortingFail == true);
    cout << "Leaderboards: \n";
    cout << "\nPlayers with the Most Wins: \n";
    if (leaderboardsGamesWon.size() >= 10) { // if there are at least 10 accounts in the system
        for (int i = 0; i < 10; i++) { // loops 10 times
            cout << i + 1 << ": " << leaderboardsUsernamesGamesWon[i] << ": " << leaderboardsGamesWon[i] << " \n"; // outputs the username and the number of wins for each of the top 10 accounts with the most wins
        }
        cout << "\nPlayers with the Most Games Played: \n";
        for (int i = 0; i < 10; i++) { // loops 10 times
            cout << i + 1 << ": " << leaderboardsUsernamesGamesPlayed[i] << ": " << leaderboardsGamesPlayed[i] << " \n"; // outputs the username and the number of wins for each of the top 10 accounts with the most games played
        }
        cout << "\nPlayers with the Highest Win Percentages: \n";
        for (int i = 0; i < 10; i++) { // loops 10 times
            cout << i + 1 << ": " << leaderboardsUsernamesWinRate[i] << ": " << leaderboardsWinRate[i] * 100 << "% \n"; // outputs the username and the number of wins for each of the top 10 accounts with the most wins
        }
    }
    else if (leaderboardsGamesWon.size() < 10) { // if there are less than 10 accounts in the system
        for (int i = 0; i < leaderboardsGamesWon.size(); i++) { // loops for the number of accounts in the system
            cout << i + 1 << ": " << leaderboardsUsernamesGamesWon[i] << ": " << leaderboardsGamesWon[i] << " \n"; // outputs the username and the number of wins for each account in the system
        }
        cout << "\nPlayers with the Most Games Played: \n";
        for (int i = 0; i < leaderboardsGamesWon.size(); i++) { // loops for the number of accounts in the system
            cout << i + 1 << ": " << leaderboardsUsernamesGamesPlayed[i] << ": " << leaderboardsGamesPlayed[i] << " \n"; // outputs the username and the number of games played for each account in the system
        }
        cout << "\nPlayers with the Highest Win Percentages: \n";
        for (int i = 0; i < leaderboardsGamesWon.size(); i++) { // loops for the number of accounts in the system
            cout << i + 1 << ": " << leaderboardsUsernamesWinRate[i] << ": " << leaderboardsWinRate[i] * 100 << "% \n"; // outputs the username and the win rate for each account in the system
        }
    }
    cout << "\n"; // leaves a blank line after printing out the leaderboards
    return;
}

int main(int argc, const char * argv[]) {
    generateLoginsFile(); // generates the logins.txt file if it doesn't exist yet
    generateAccountsFile(); // generates the accounts.txt file if it doesn't exist yet
    do { // loops back to the start of the program if the user decides they want to quit
        do { // loops back to the start of the program if the players decide to quit after playing each other
            do { // returns to the start of the program if the user decides to quit after creating an account
                cout << "Welcome to Connect Four!\n"; // welcomes the user to Connect Four
                do { // repeats until the user enters a valid input for loginOrCreateAccount
                    loginOrQuit = 'h'; // unsets the loginOrQuit variable on loop
                    cout << "Would you like to log into an existing account or create a new account? Please type \"L\" to login or \"C\" to create a new account. \n"; // asks the user if they would like to log into an account or create a new account
                    cin >> loginOrCreateAccount; // stores their answer as loginOrCreateAccount
                    if (loginOrCreateAccount == 'L' || loginOrCreateAccount == 'l') { // calls the login() function with int player = 1 as a parameter if the user wants to log in
                        login(1);
                    }
                    else if(loginOrCreateAccount == 'C' || loginOrCreateAccount == 'c') { // calls the createAccount function with int player = 1 as a parameter if the user wants to log in
                        createAccount(1);
                    }
                    if (loginOrCreateAccount != 'L' && loginOrCreateAccount != 'l' && loginOrCreateAccount != 'C' && loginOrCreateAccount != 'c') {
                        cout << "Sorry, this is not a valid input. Please try again. \n"; // tells the user that they entered an invalid input and asks them to try entering it again
                    }
                } while (loginOrCreateAccount != 'L' && loginOrCreateAccount != 'l' && loginOrCreateAccount != 'C' && loginOrCreateAccount != 'c'); // loops if the user enters an invalid input
            } while (loginOrQuit == 'Q' || loginOrQuit == 'q' || loginToAnotherAccountCreateAccountOrQuit == 'Q' || loginToAnotherAccountCreateAccountOrQuit == 'q'); // loops back to the start of the program if the user decides to quit after creating an account or if Player 1 decides to quit while logging in or after entering the wrong password to an account
            cout << "You are now logged into your account. \n";
            do {
                do { // loops if Player 1 decides to go back to where they can challenge someone else or view the leaderboards after playing a game or after displaying the leaderboards to the player
                    playAgainAccountHomeOrQuit = 'z'; // unsets the playAgainAccountHomeOrQuit variable on loop
                    do {
                        cout << "Press \"P\" to play the game, \"L\" to view your stats and the leaderboards, or \"Q\" to quit the program. \n"; // asks Player 1 if they would like play the game, view the leaderboards or quit the program
                        cin >> playLeaderboardsOrQuit; // stores Player 1's choice
                        if (playLeaderboardsOrQuit != 'P' && playLeaderboardsOrQuit != 'p' && playLeaderboardsOrQuit != 'L' && playLeaderboardsOrQuit != 'l' && playLeaderboardsOrQuit != 'Q' && playLeaderboardsOrQuit != 'q') {
                            cout << "Sorry, this is not a valid input. Please try again. \n"; // tells the user that they entered an invalid input and asks them to try entering it again
                            cout << "Press \"P\" to play the game, \"L\" to view your stats and the leaderboards, or \"Q\" to quit the program. \n"; // lists the choices to the user again
                        }
                    } while (playLeaderboardsOrQuit != 'P' && playLeaderboardsOrQuit != 'p' && playLeaderboardsOrQuit != 'L' && playLeaderboardsOrQuit != 'l' && playLeaderboardsOrQuit != 'Q' && playLeaderboardsOrQuit != 'q'); // repeats the choice if the user enters an invalid input
                    if (playLeaderboardsOrQuit == 'P' || playLeaderboardsOrQuit == 'p') { // if the user says they want to play the game
                        playGameResult = playGame(); // starts the game and stores and stores any return value
                    }
                    else if (playLeaderboardsOrQuit == 'L' || playLeaderboardsOrQuit == 'l') { // if the user says they want to view the leaderboards
                        player1.showStats(); // shows the player their account's stats
                        showLeaderboards(); // shows the leaderboards for the palyers with the most games won, most games played and highest percentage of games won
                    } 
                } while (playAgainAccountHomeOrQuit == 'B' || playAgainAccountHomeOrQuit == 'b' || playLeaderboardsOrQuit == 'L' || playLeaderboardsOrQuit == 'l'); // loops if Player 1 decides to go back to where they can challenge someone else or view the leaderboards after playing a game or after displaying the leaderboards to the player
            } while (playGameResult == 'C' || playGameResult == 'c'); // loops back to right after Player 1 logs in if the players decide to not challenge each other after Player 2 logs in
        } while (playAgainAccountHomeOrQuit == 'Q' || playAgainAccountHomeOrQuit == 'q'); // loops if the players decide to quit after playing each other
    } while (playLeaderboardsOrQuit == 'Q' || playLeaderboardsOrQuit == 'q'); // loops back to the start of the program if the user decides they want to quit
    return 0;
}


