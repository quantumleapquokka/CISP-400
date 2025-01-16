#include <iostream>
#include <cstdlib>
#include <random>
using namespace std;

class gmePly{
public:
  // Display Menu
  void MenuOut(){
    cout << "=====================" << endl;
    cout << "|     Main Menu     |" << endl;
    cout << "=====================" << endl;
    cout << " • <R>ock" << endl;
    cout << " • <P>aper" << endl;
    cout << " • <S>cissors" << endl;
    cout << " • <Q>uit Program" << endl;
    cout << "=====================" << endl;
    cout << endl;
  }

  // Input Validation for menu choice
  char inputValid(){
    // Prompt, accept input, and validate the menu input of the user to be character within the menu choices
    char alphaSelect;
    bool validChoice = false;
  
    do {
      // Prompt for the user's menu choice and
      cout << "Enter menu choice: ";
      cin >> alphaSelect;
      alphaSelect = toupper(alphaSelect);
  
      // Validate the input is within the 1 through 4 limits of the menu and is an integer
      if ( cin.fail() || (alphaSelect != 'R' && alphaSelect != 'P' && alphaSelect != 'S' && alphaSelect != 'Q')){
        // Specify to the user that their menu choice is not alphanumeric or that they just entered an invalid option
        if(!isalnum(alphaSelect)){
          cout << "Incorrect Input Type!" << endl;
        } else {
          cout << "Invalid Menu Choice!" << endl;
        }
        cin.clear();
        cin.ignore(100, '\n');  
      } else {
        validChoice = true;    // exit loop
      }
    } while(!validChoice);
  
    return alphaSelect;
  }
};

// Fct Prototype
void ProgramGreeting();

int main() {
  ProgramGreeting();
  
  // Variables
  char humnTrn;
  char comptTrn = ' ';
  int hands = 0;
  int humnWins = 0;
  int comptWins = 0;
  int randomChoice;

  // Human Player Object
  gmePly humnPlay;

  // Random Number Generator (For Computer Moves)
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> dis(0, 2);
  
  do{
    // Human's Turn
    humnPlay.MenuOut();
    humnTrn = humnPlay.inputValid();
    if (humnTrn == 'R'){
      cout << "Your Move: ROCK" << endl;
      hands++;
    } else if (humnTrn == 'P') {
      cout << "Your Move: PAPER" << endl;
      hands++;
    } else if (humnTrn == 'S') {
      cout << "Your Move: SCISSORS" << endl;
      hands++;
    }
    
    // Computer Turn
    if (humnTrn != 'Q'){
      randomChoice = dis(gen);
      if (randomChoice == 0) {
        comptTrn = 'R';
        cout << "Computer Move: ROCK" << endl;
      } else if (randomChoice == 1) {
        comptTrn = 'P';
        cout << "Computer Move: PAPER" << endl;
      } else {
        comptTrn = 'S';
        cout << "Computer Move: SCISSORS" << endl;
      }
    }
      

    // Determine the round winner
    if (humnTrn == comptTrn){
      cout << "TIED!" << endl;
    } else if ((humnTrn == 'R' && comptTrn == 'S') || (humnTrn == 'P' && comptTrn == 'R') || (humnTrn == 'S' && comptTrn == 'P')){
      cout << "YOU won this round!" << endl;
      humnWins++;
    } else if ((comptTrn == 'R' && humnTrn == 'S') || (comptTrn == 'P' && humnTrn == 'R') || (comptTrn == 'S' && humnTrn == 'P')) {
      cout << "Computer won this round" << endl;
      comptWins++;
    }

    if (humnTrn != 'Q') {
      cout << endl;
      cout << "--N E W _ R O U N D--" << endl;
      cout << endl;
    }
   
  } while (humnTrn != 'Q');

  // Display end game results
  cout << endl;
  cout << "~ G A M E _ R E S U L T S ~" << endl;
  cout << "Hands: " << hands << endl;
  cout << "Human Wins: " << humnWins << endl;
  cout << "Computer Wins: " << comptWins << endl;
  if (humnWins > comptWins){
    cout << "Overall Winner: Human Player" << endl;
  } else if (humnWins < comptWins){ 
    cout << "Overall Winner: Computer AI" << endl;
  } else {
    cout << "Overall Winner: TIED!!!!" << endl;
  }
  
  return 0;
}

// Program Greeting
void ProgramGreeting(){
  // Display the welcome to the program, name, and the due date of the assignment.
  cout << endl;
  cout << "= = = = = = = = = = = = = = =" << endl;
  cout << "Welcome to the ROCK PAPER SCISSORS GAME Program." << endl;
  cout << "Made by: Samantha Siew" << endl;
  cout << "Due: Dec 15, 2023" << endl;
  cout << "= = = = = = = = = = = = = = =" << endl;
  cout << endl;
}
