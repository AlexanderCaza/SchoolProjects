//
//  Title: Vending Machine
//  Description: Sells and dispenses snacks and drinks to the customer. Accepts money from the customer. Dispenses change to the user in coins. Allows an admin to make changes to the system and view a variety of information via a secret admin panel.
//  Created by: Alexander Caza
//  Created on: February 24th, 2021
//  Last Update: March 1st, 2021
//  Known Limitations: Unable to remove "Program ended with exit code: "0" message; Values input must be the valid types (i.e. integers, floats); user can only buy one item at a time; Change will dispense even when there are none of the coins being dispensed in the machine; If you use only nickels to pay for an item, the vending machine will require 1 additional nickel for payment but will return it as change.
//
///////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
using namespace std;

int main() {
    
    // variables
    string namesOfItems[9] = {"Aero Bar", "Smarties", "Kit-Kat", "Coke", "Pepsi", "Mountain Dew", "Mars Bar", "Twix Bar", "Water Bottle"}; // the names of the products.
    string adminNewName;
    float priceOfItems[9] = {2.00, 3.00, 2.50, 1.50, 2.00, 4.00, 2.75, 2.25, 3.00}; // the prices of the products
    float adminNewPrice;
    int inventoryOfItems[10] = {9, 9, 9, 9, 9, 9, 9, 9, 9, 10}; // 10th position is a fake item that will never run out of stock that ensures access to the admin control panel is always possible. The rest are the number items in stock.
    int coinsStored[5] = {10, 10, 10, 10, 0}; // position 0 = Nickels, position 1 = Dimes, position 2 = Quarters, position 3 = Loonies, position 4 = Toonies. Vending Machine starts with 10 nickels, dimes, quarters and loonies to be used for change.
    int desiredItem;
    int adminFunction;
    int changeNickels;
    int changeDimes;
    int changeQuarters;
    int changeLoonies;
    int changeAmountWhole;
    int adminNewInventory;
    char coinUsed;
    char adminUseAgain = 'n';
    char adminAccess = 'n';
    float storedMoneyTotal = 0;
    float storedMoneyTransaction = 0;
    float changeAmount;
    
    do {
        
        storedMoneyTransaction = 0; // resets storedMoneyTransaction after each transaction.
        
        // vending Machine Graphic
        cout << " ____________________________________________\n";
        cout << "|############################################|\n";
        cout << "|#|                           |##############|\n";
        cout << "|#|  =====           |~~``|   |##|````````|##|\n";
        cout << "|#|  |   |           :    |   |##| Coins  |##|\n";
        cout << "|#|  |___|   ____    | ___|   |##|        |##|\n";
        cout << "|#|  /=__\\   |__|    |/,_ |   |##| Only   |##|\n";
        cout << "|#|  |___|   |__|    \\___//   |##|________|##|\n";
        cout << "|#|===========================|##############|\n";
        cout << "|#|```````````````````````````|##############|\n";
        cout << "|#|   __      +++     //////  |##############|\n";
        cout << "|#| \\/  \\     | |     \\   \\   |#|`````````|##|\n";
        cout << "|#|  \\___\\    |_|     /___/   |#| _______ |##|\n";
        cout << "|#|  / __\\   /|_|\\   // ___\\  |#| |1|2|3| |##|\n";
        cout << "|#|  \\__//-  \\|_//   -\\___//  |#| |4|5|6| |##|\n";
        cout << "|#|===========================|#| |7|8|9| |##|\n";
        cout << "|#|```````````````````````````|#| ``````` |##|\n";
        cout << "|#|   --    ______   .--._.   |#|[=======]|##|\n";
        cout << "|#|  |  |   |    |   |    |   |#|  _   _  |##|\n";
        cout << "|#|  |__|   :____:   | ___|   |#| ||| ( ) |##|\n";
        cout << "|#|  |__|   |    |   // __\\\\  |#| |||  -  |##|\n";
        cout << "|#|  |__|   |    |   /_\\__//  |#|  -      |##|\n";
        cout << "|#|  |__|   |____|   /_\\__//  |#|         |##|\n";
        cout << "|#|===========================|#|_________|##|\n";
        cout << "|#|```````````````````````````|##############|\n";
        cout << "|############################################|\n";
        cout << "|#|||||||||||______||||||||||||####```````###|\n";
        cout << "|#||||||||||||PUSH|||||||||||||####\\|||||/###|\n";
        cout << "|############------##########################|\n";
        cout << "|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\///////////////////////|\n";
        cout << "|_________________________________| THIC |___|\n";
        
        // greetings the customer
        cout << "Welcome customer!\n";
        
        // lists items
        do {
            for (int i = 0; i < 9; i++) {
                cout << i + 1 << ". Name: " << namesOfItems[i] << " Price: $" << priceOfItems[i] << " In stock: " << inventoryOfItems[i] << "\n";
            }
            
            
            // ask the user what they want to buy
            cout << "What would you like to buy? Enter the corresponding number. \n\n";
            cin >> desiredItem;
            if (desiredItem != 1337 && desiredItem != 1 && desiredItem != 2 && desiredItem != 3 && desiredItem != 4 && desiredItem != 5 && desiredItem != 6 && desiredItem != 7 && desiredItem != 8 && desiredItem != 9) {
                cout << "ERROR: Invalid selection. Please try again! \n";
            }
            else if (desiredItem == 1337) {
                desiredItem = 10;
                adminAccess = 'y';
            }
            else if (inventoryOfItems[desiredItem - 1] == 0) {
                cout << "ERROR: Item out of stock. Please choose a different item!\n"; // tells the user to choose a different item, if the selected item is out of stock.
            }
                
        } while ((desiredItem != 1337 && desiredItem != 1 && desiredItem != 2 && desiredItem != 3 && desiredItem != 4 && desiredItem != 5 && desiredItem != 6 && desiredItem != 7 && desiredItem != 8 && desiredItem != 9 && desiredItem != 10) || inventoryOfItems[desiredItem - 1] == 0); // forces user to enter a valid slection or the passcode for the Admin Panel. Restarts the selection process if the chosen item is out of stock.
        
        // opens Admin Panel if 1337 is entered.
        if (adminAccess == 'y') {
            do {
                
                cout << "Admin Panel Opened! \n";
                cout << "1. Display money stored and coins stored\n";
                cout << "2. Display number of each item left\n";
                cout << "3. Edit the name of an item in the machine.\n";
                cout << "4. Change the price of an item.\n";
                cout << "5. Update the number of items in stock.\n";
                cout << "What would you like to do? Please input the corresponding number.\n";
                do {
                    cin >> adminFunction; // stores the Admin's selection
                    if (adminFunction != 1 && adminFunction != 2 && adminFunction != 3 && adminFunction != 4 && adminFunction != 5 && adminFunction) {
                        cout << "ERROR: Invalid input. Please try again.";
                    }
                } while (adminFunction != 1 && adminFunction != 2 && adminFunction != 3 && adminFunction != 4 && adminFunction != 5 && adminFunction);
                
                if (adminFunction == 1) { // Displays money stored and coins stored when option 1 is chosen
                    cout << "Total Money stored: " << storedMoneyTotal << "$.\n"; // total money in vending machine including coins stored on start of program for change
                    cout << "Money earned: " << storedMoneyTotal << "$.\n"; // money earned by vending machine during use (does not include coins that were stored in the machine on start to create change.
                    cout << "Coins Stored:\n";
                    cout << "Toonies: " << coinsStored[4] - 0 << "\n";
                    cout << "Loonies: " << coinsStored[3] << "\n";
                    cout << "Quarters: " << coinsStored[2] << "\n";
                    cout << "Dimes: " << coinsStored[1] << "\n";
                    cout << "Nickels: " << coinsStored[0] << "\n";
                }
                else if (adminFunction == 2) { // Displays number of each item left
                    cout << "Items stored: " << "\n";
                    for (int i = 0; i < 9; i++) { // repeats for each item
                        cout << namesOfItems[i] << ": " << inventoryOfItems[i] << "\n"; // displays item name and the amount left in stock
                    } // end of for loop
                }
                else if (adminFunction == 3) { // allows the Admin to change the name of an item.
                    for (int i = 0; i < 9; i++) { // repeats for each item
                        cout << i + 1 << ": " << namesOfItems[i] << "\n"; // displays a numbered list of item names.
                    } // end of for loop
                    cout << "What is the number of the item you would like to change? Please enter the corresponding number.\n"; // asks the Admin what item they would like to edit
                    do {
                        cin >> desiredItem; // stores the Admin's selection
                        if (desiredItem != 1 && desiredItem != 2 && desiredItem != 3 && desiredItem != 4 && desiredItem != 5 && desiredItem != 6 && desiredItem != 7 && desiredItem != 8 && desiredItem != 9) {
                            cout << "ERROR: Invalid input. Please try again.";
                        }
                    } while (desiredItem != 1 && desiredItem != 2 && desiredItem != 3 && desiredItem != 4 && desiredItem != 5 && desiredItem != 6 && desiredItem != 7 && desiredItem != 8 && desiredItem != 9);
                    
                    desiredItem = desiredItem - 1; // changes the value of the Admin's selection to align with the position of the item in the array.
                    cout << "What would you like the new name of the item to be?"; // asks the Admin what they would like the new name of the item to be
                    cin >> adminNewName; // stores the Admin's desired new name for the item.
                    namesOfItems[desiredItem] = adminNewName; // changes the name of the item in the system.
                }
                else if (adminFunction == 4) { // allows the Admin to change the name of an item.
                    for (int i = 0; i < 9; i++) { // repeats for each item
                        cout << i + 1 << ": " << namesOfItems[i] << " Price: " << priceOfItems[i] << "$ \n"; // displays a numbered list of item names and their prices.
                    } // end of for loop
                    cout << "What is the number of the item who's price you would like to change? Please enter the corresponding number.\n"; // asks the Admin which item's price they would like to change
                    do {
                        cin >> desiredItem; // stores the Admin's selection
                        if (desiredItem != 1 && desiredItem != 2 && desiredItem != 3 && desiredItem != 4 && desiredItem != 5 && desiredItem != 6 && desiredItem != 7 && desiredItem != 8 && desiredItem != 9) {
                            cout << "ERROR: Invalid input. Please try again.";
                        }
                    } while (desiredItem != 1 && desiredItem != 2 && desiredItem != 3 && desiredItem != 4 && desiredItem != 5 && desiredItem != 6 && desiredItem != 7 && desiredItem != 8 && desiredItem != 9);
                    desiredItem = desiredItem - 1; // changes the value of the Admin's selection to align with the position of the item in the array.
                    cout << "What would you like the new price of the item to be? (please enter a number like '3' or '2.50'.) "; // asks the Admin what they would like the new price of the item to be
                    cin >> adminNewPrice; // stores the Admin's desired new price for the item.
                    priceOfItems[desiredItem] = adminNewPrice; // changes the price of the item in the system.
                }
                else if (adminFunction == 5) { // allows the Admin to change the name of an item.
                    for (int i = 0; i < 9; i++) { // repeats for each item
                        cout << i + 1 << ": " << namesOfItems[i] << " In Stock: " << inventoryOfItems[i] << " \n"; // displays a numbered list of item names and the number in stock.
                    } // end of for loop
                    cout << "What is the number of the item who's amount you'd like to update? Please enter the corresponding number.\n"; // asks the Admin which item's amount they would like to change
                    do {
                        cin >> desiredItem; // stores the Admin's selection
                        if (desiredItem != 1 && desiredItem != 2 && desiredItem != 3 && desiredItem != 4 && desiredItem != 5 && desiredItem != 6 && desiredItem != 7 && desiredItem != 8 && desiredItem != 9) {
                            cout << "ERROR: Invalid input. Please try again.";
                        }
                    } while (desiredItem != 1 && desiredItem != 2 && desiredItem != 3 && desiredItem != 4 && desiredItem != 5 && desiredItem != 6 && desiredItem != 7 && desiredItem != 8 && desiredItem != 9);
                    desiredItem = desiredItem - 1; // changes the value of the Admin's selection to align with the position of the item in the array.
                    cout << "What would you like the new number in stock to be? "; // asks the Admin what they would like the new price of the item to be
                    cin >> adminNewInventory; // stores the Admin's desired new value for number of items in stock.
                    inventoryOfItems[desiredItem] = adminNewInventory; // changes the number of items in stock in the system.
                }
                
                
                
                cout << "Would you like to do anything else? (y/n)";
                cin >> adminUseAgain;
            } while (adminUseAgain == 'y' || adminUseAgain == 'Y');
            
        }
        else {
            
            desiredItem = desiredItem - 1; // reduces the number of the item chosen by the user by one, thus alighing the variable with the position of the item in the arrays.
            
            // tells the customer what item costs and asks the customer what coins they will use.
            cout << "The cost of the item is: $" << priceOfItems[desiredItem] << ". What coins will you use to pay? Enter 'N' for nickel, 'D' for dime, 'Q' for quarter, 'L' for loonie and 'T' for Toonie. \nONLY COINS ARE ACCEPTED BY THIS MACHINE!\n\n";
            // tells the customer how much the item costs.
            do {
            cout << namesOfItems[desiredItem] << " costs $" << priceOfItems[desiredItem] << ".\n";
            cout << "You have paid $" << storedMoneyTransaction << " so far. Please insert your next coin. (N/D/Q/L/T)\n\n"; // tells the customer how much they have paid so far.
                cin >> coinUsed;
                if (coinUsed == 'N' || coinUsed == 'n') { // updates amount of money paid so far in this transaction, the stored coin inventory and the total amount of money in the machine
                    storedMoneyTransaction = storedMoneyTransaction + 0.05;
                    storedMoneyTotal = storedMoneyTotal + 0.05;
                    coinsStored[0] = coinsStored[0] + 1;
                }
                if (coinUsed == 'D' || coinUsed == 'd') {
                    storedMoneyTransaction = storedMoneyTransaction + 0.10;
                    storedMoneyTotal = storedMoneyTotal + 0.10;
                    coinsStored[1] = coinsStored[1] + 1;
                }
                if (coinUsed == 'Q' || coinUsed == 'q') {
                    storedMoneyTransaction = storedMoneyTransaction + 0.25;
                    storedMoneyTotal = storedMoneyTotal + 0.25;
                    coinsStored[2] = coinsStored[2] + 1;
                }
                if (coinUsed == 'L' || coinUsed == 'l') {
                    storedMoneyTransaction = storedMoneyTransaction + 1.00;
                    storedMoneyTotal = storedMoneyTotal + 1.00;
                    coinsStored[3] = coinsStored[3] + 1;
                }
                if (coinUsed == 'T' || coinUsed == 't') {
                    storedMoneyTransaction = storedMoneyTransaction + 2.00;
                    storedMoneyTotal = storedMoneyTotal + 2.00;
                    coinsStored[4] = coinsStored[4] + 1;
                }
                else if (coinUsed != 'N' && coinUsed != 'n' && coinUsed != 'D' && coinUsed != 'd' && coinUsed != 'Q' && coinUsed != 'q' && coinUsed != 'L' && coinUsed != 'l' && coinUsed != 'T' && coinUsed != 't') {
                    cout << "Error: Invalid input. Please try again! \n";
                }
            } while (storedMoneyTransaction < priceOfItems[desiredItem]); // keeps asking the customer to insert money until the amount paid is equal to the amount required
            cout << namesOfItems[desiredItem] << " dispensed! Please collect your change and have a nice day!\n";

            inventoryOfItems[desiredItem] = inventoryOfItems[desiredItem] - 1; // removes 1 from the number of the item chosen by the customer left in the machine
            
            changeAmount = storedMoneyTransaction - priceOfItems[desiredItem]; // calculates amount of change due
            
            // calculates loonies required for change.
            changeAmountWhole = changeAmount * 100;
            changeLoonies = changeAmountWhole / 100;
            coinsStored [3] = coinsStored[3] - changeLoonies;
            changeAmountWhole = changeAmountWhole % 100;
            
            // calculates quarters required for change.
            changeQuarters = changeAmountWhole / 25;
            coinsStored [2] = coinsStored[2] - changeQuarters;
            changeAmountWhole = changeAmountWhole % 25;
            
            // calculates dimes required for change.
            changeDimes = changeAmountWhole / 10;
            coinsStored [1] = coinsStored[1] - changeDimes;
            changeAmountWhole = changeAmountWhole % 10;
            
            // calculates nickels required for change.
            changeNickels = changeAmountWhole * 0.25;
            coinsStored [0] = coinsStored[0] - changeNickels;
            
            cout << "Your change is loonies: " << changeLoonies << ", quarters: " << changeQuarters << ", dimes: " << changeDimes << " and nickels: " << changeNickels << "\n";

        }

        
        sleep(7); // causes program to wait 7 seconds which allows the last user to see their change before the program restarts
        cout << "\n\n\n\n\n\n";
        adminAccess = 'n'; // Changes the adminAccess variable so that the user won't be able to access it on restart of the vending machine without entering the secret code again.
        
        
        
    } while (inventoryOfItems[0] != 0 && inventoryOfItems[1] != 0 && inventoryOfItems[2] != 0 && inventoryOfItems[3] != 0 && inventoryOfItems[4] != 0 && inventoryOfItems[5] != 0 && inventoryOfItems[6] != 0 && inventoryOfItems[7] != 0 && inventoryOfItems[8] != 0); // reruns while there are still items left to buy in the machine
        
    cout << "ERROR: Please ask an Administrator to refill this vending machine!\n";
    return 0;
}
