//Specification B2 - Source File Header
//pig.cpp
//Samantha Siew, CISP 400
//9/3/23

#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include <cctype>
using namespace std;

// Function Prototypes
void ProgramGreeting();
int D6 ();
int RandomNumber(int, int);
bool replayGame(int&, int&, string, int&, int&, int, int*);
int turnStats(int, int);
void cTurnChoice(int);

int main() {
  ProgramGreeting(); // program greeting

  // Variables
  string firstLastName;
  int menuSelect = 0;
  int playerTScore = 0;
  int pScoreTot = 0;
  int compTScore = 0;
  int cScoreTot = 0;
  bool endGame = false;
  int diceRoll;  

  // Specification B3 - hiScore on Heap
  int* hiScore = nullptr;
  hiScore = new int;
  *hiScore = 0;
  int currHiScore = *hiScore;     // Contains the current high score

  // NOT IN USE - USED IN D6
  // // Specification C1 - Fixed Seed
  // int seed = 7;
  // srand(seed);
  // int diceRoll = (rand() % (6 - 1 + 1)) + 1;


  // Specification C2 - Student Name
  cout << "Enter first + last name (with a space inbetween): ";
  getline(cin, firstLastName);
  cout << "Player Name: " << firstLastName << endl;


  // Title Statements
  cout << endl;
  cout << ". . . . . . . ." << endl;
  cout << "Pig Game Start!" << endl;
  cout << ". . . . . . . ." << endl;
  cout << endl;
  cout << firstLastName << " goes first!" << endl;
  cout << endl;

  // Loop through the game and play as many games as the player wishes until they want to quit
  do {
    // Specification C3 - Numeric Menu
    cout << "Menu:" << endl;
    cout <<"------" << endl;
    cout << "1. Roll Dice" << endl;
    cout << "2. Hold" << endl;
    cout << "3. Quit" << endl;
    cout << endl;


    playerTScore = 0;     // Zero out round score for player
    compTScore = 0;      // Zero out round score for computer
    

    // Player Turn
    do {
      // Specification A4 - Input Validation
      do {
        // Prompt for input
        cout << "Enter a menu option (1-3): ";
        cin >> menuSelect;

        // Check if the input is valid
        if (cin.fail() || menuSelect > 3 || menuSelect < 1) {
          cout << "Invalid input!" << endl;
          cin.clear();
          cin.ignore(100, '\n');
        }
      } while (cin.fail() || menuSelect > 3 || menuSelect < 1);

      
      // Gameplay output according to the menu option entered by the player
      if (menuSelect == 1) {
        diceRoll = D6();       // Reroll
        cout << "  Dice Roll: " << diceRoll << endl;
        
        // Only add the score if the roll is not a 1
        if (diceRoll != 1) {
          playerTScore = diceRoll + playerTScore;
          cout << "  Turn Score: " << playerTScore << endl;
          cout << endl;
        } else {
          playerTScore = 0;
          break;
        }
      } else if (menuSelect == 2){
        break;
      } else if (menuSelect == 3) {
        cout << endl;
        cout << "= = = = = = = = = = = = = = = = =" << endl;
        cout << "You have ended the game. Goodbye!" << endl;
        cout << "= = = = = = = = = = = = = = = = =" << endl;
        exit(0);      // End the whole program
      }
    } while (menuSelect != 2);


    // Determine whether a player has gotten a score of 100 and if the game should restart
    pScoreTot = turnStats(playerTScore, pScoreTot);
    endGame = replayGame(cScoreTot,pScoreTot, firstLastName, playerTScore, compTScore, currHiScore, hiScore); 
    if(endGame){
      cout << endl;
      cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl;
      cout << firstLastName << " Game Score: " << pScoreTot << endl;
      cout << "Computer Game Score: " << cScoreTot << endl;
      cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl;
      cout << endl;
      break;
    }


    // Computer Turn
    cout << endl;
    cout << "- - - - - - - -" << endl;
    cout << "Computer Turn!" << endl;
    cout << "- - - - - - - -" << endl;

    // Randomize computer turn action
    int compTurn = RandomNumber(1,6);
    
    cTurnChoice(compTurn);
    
    // Keep the computer rolling the dice while the dice rolls between 4 and 6 inclusive
    while (compTurn >= 4 && compTurn <= 6) {
      diceRoll = D6();  // Reroll
      cout << "  Dice Roll: " << diceRoll << endl;
  
      if (diceRoll != 1) {
        compTScore = diceRoll + compTScore;
        cout << "  Turn Score: " << compTScore << endl;
        cout << endl;
      } else if (diceRoll == 1) {
        compTScore = 0;
        break;
      }

      // Generate a random number 1-6 to decide the AI's action
      compTurn = RandomNumber(1,6);
      cTurnChoice(compTurn);
    }

    // Add up points from current round
    cScoreTot = turnStats(compTScore, cScoreTot);    

    // Display current score after each round for both players
    cout << endl;
    cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl;
    cout << firstLastName << " Game Score: " << pScoreTot << endl;
    cout << "Computer Game Score: " << cScoreTot << endl;
    cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl;
    cout << endl;

    // Determine whether a player has gotten a score of 100 and if the game should restart   
    endGame = replayGame(cScoreTot,pScoreTot, firstLastName, playerTScore, compTScore, currHiScore, hiScore);
    if(endGame){
      break;
    }

    
    // Announce it is the player's turn
    cout << endl;
    cout << "- - - - - - - - -" << endl;
    cout << firstLastName << "'s Turn!" << endl;
    cout << "- - - - - - - - -" << endl;

  } while (!endGame);

  cout << endl;
  
  
  // Specification B4 – Display hiScore
  cout << "= = = = = = = = = = =" << endl;
  cout << "Game High Score: " << *hiScore << endl;
  cout << "= = = = = = = = = = =" << endl;
  
  delete hiScore;
  
  return 0;
}


// Specification A1 - D6() function
int D6 (){
  // Specification C1 - Fixed Seed
  int seed = time(NULL);
  srand(seed);
  int randNum = (rand() % (6 - 1 + 1)) + 1;
  return randNum;
}


// Specification A2 - RandomNumber() function
int RandomNumber(int lo, int hi){
  // Specification A3 - Protect RandomNumber() input
  if (hi > lo && lo >= 1 && hi <= 100){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(lo, hi);
    int randomNumb = dis(gen);
    return randomNumb;
  } else {
    return -1;
  }
}


// Check to see if any of the scores has reached 100 each round
// End the game once the score reaches 100
bool replayGame(int& cScoreTot, int& pScoreTot, string name, int& playerTScore, int& compTScore, int currHiScore, int* hiScore){
  char contGame;
  bool game;
  if (cScoreTot >= 100 || pScoreTot >= 100){
    if (cScoreTot >= 100) {
      // Set high score to this score
      currHiScore = cScoreTot;
      if (currHiScore > *hiScore) {
        *hiScore = currHiScore;
      }
      
      cout << "The computer won!" << endl;
      cout << endl;
    } else if (pScoreTot >= 100){
      // Set high score to this score
      currHiScore = pScoreTot;
      if (currHiScore > *hiScore) {
        *hiScore = currHiScore;
      }
      
      cout << endl;
      cout << "= = = = =" << endl;
      cout << name << " won!" << endl;
      cout << "= = = = =" << endl;
      cout << endl;
    }

    // Keep the game going or end it
    cout << "Would you like to play another round? (Y/N)" << endl;
    do{
      cout << "Input: ";
      cin.clear();
      cin.ignore(100, '\n');
      cin >> contGame;
      contGame = tolower(contGame);
    }while (cin.fail() || (contGame != 'y' && contGame != 'n'));
    
    if (contGame == 'y') {
      cout << endl;
      cout << "========" << endl;
      cout << "NEW GAME" << endl;
      cout << "========" << endl;

      game = false;
      cout << endl;

      // Zero out everything before the next game
      playerTScore = 0;
      pScoreTot = 0;
      compTScore = 0;
      cScoreTot = 0;
    } else if (contGame == 'n') {
      game = true;
    }
  }
  return game;
}


// Specification C4 - ProgramGreeting
void ProgramGreeting() {
  cout << "Pig Program Greeting" << endl;
  cout << endl;


  // Test A2 function
  cout << "Test for A2 function:" << endl;
  int lo = 50;
  int hi = 23;
  cout << "Lo: " << lo << endl <<"Hi: " << hi << endl << RandomNumber(lo, hi) << endl;

  lo = -5;
  hi = 23;
  cout << "Lo: " << lo << endl <<"Hi: " << hi << endl << RandomNumber(lo, hi) << endl;

  lo = 5;
  hi = 230;
  cout << "Lo: " << lo << endl <<"Hi: " << hi << endl << RandomNumber(lo, hi) << endl;

  lo = 5;
  hi = 23;
  cout << "Lo: " << lo << endl <<"Hi: " << hi << endl << RandomNumber(lo, hi) << endl;

  cout << endl;
  cout << "========" << endl;
  cout << "PIG GAME" << endl;
  cout << "========" << endl;
  cout << endl;
}


// Specification B1 - Display Turn Stats
int turnStats(int turnScore, int scoreTot){
  scoreTot = turnScore + scoreTot;

  return scoreTot;
}


// Display the computer's choice
void cTurnChoice(int compTurn){
  if (compTurn >= 4 && compTurn <= 6) {
    cout << "Computer rolled the dice." << "(" << compTurn << ")" << endl;
  } else {
    cout << "Computer chose to HOLD." << "(" << compTurn << ")" << endl;
  }
}