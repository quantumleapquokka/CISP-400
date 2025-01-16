//gpaAnalyzer.cpp
//Samantha Siew, CISP 400
//9/24/23

#include <iostream>
#include <ctime>
#include <string>
#include <iomanip>
using namespace std;

// Specification A1 - Date class
// Display date in proper formatting and querying the system date
class Date {
private:
  int mon, day, yr;
public:
  void SetDate(int, int, int);
  void properDate ();
  void CompTest();
  string getDate();
  Date(){ 
    time_t currTime = time(0);
  	tm *ltm = localtime(&currTime);
    mon = ltm->tm_mday;
    day = 5+ltm->tm_hour;
    yr = 1900 + ltm->tm_year;
  }
};
// Specification A2 - External date initialization
// Assign month, day and year to respective variables
void Date::SetDate (int mo, int da, int y) {
  mon = mo;
  day = da;
  yr = y;
}

// Properly format the date mm/dd/yyyy
void Date::properDate(){
  cout << mon << "/" << day << "/" << yr << endl;
}

// Return date with correct format mm/dd/yyyy
string Date::getDate(){
  string date = to_string(mon) + "/" + to_string(day) + "/" + to_string(yr);
  return date;
}

// Specification A3 - Component Test Method in Date
// Test whether the SetDate method works and display whether it works
void Date::CompTest(){
  Date testing;
  testing.SetDate(10,25,1998);
  if(testing.getDate() == "10/25/1998") {
    cout << "SetDate works!" << endl;
  } else {
    cout << "Test failed!" << endl;
  }
}

// Specification B4 - Highlight Failing Grades
// The class will take the string parameter and turn it into red
class FancyText {
public:
  // ANSI escape codes for text colors
  string RESET = "\033[0m";
  string RED = "\033[31m";

  // Constructor
  FancyText() {}

  // Print numbers in red
  string redText(string text) {
    return RED + text + RESET;
  }
};

// Function prototypes
void ProgramGreeting();
void mainMenuP();
int inputValid();
void enterGrade(int*& , int&);
void printScores(int*& , int);
void computeGPA(int*& , int);
char Grade2Lttr(int);
void UnitTest();

int main() {  
  // Set the date to the due date and run UnitTest function
  UnitTest();
  Date date;
  date.SetDate(9,24,2023);
  cout << "Date: ";
  date.properDate();

  // Greet user by welcome
  ProgramGreeting();   
  
  // Variables
  int menuSelect = 0;
  char grdLtr;
  
  // Specification B1 - Dynamic Array
  // Dynamically allocated array for the student's scores entered by user
  int arrSize = 0;
  int* stuScore = new int[arrSize];

  // Display the main menu
  mainMenuP();

  do{
    //Take in user's menu choice  
    cout << "- - - - - - - - - -" << endl;
    menuSelect = inputValid();
    

    // Actions or outputs based on menu choice
    switch(menuSelect){
      // Add a new grade
      case 1: {
        enterGrade(stuScore, arrSize);
        break;
      }
      // Print out all the grades
      case 2: {
        printScores(stuScore, arrSize); 
        break;
      }
      // Calculate and print the GPA
      case 3: {
        computeGPA(stuScore, arrSize);
        break;
      }
      default: {
        break;
      }
    }
  } while (menuSelect != 4);

  return 0;
}

// Accept grade input from user
void enterGrade(int*& stuScore, int& arrSize){
  int grde = 0;
  bool validChoice = false;

  // Take in grade from user and verify that it is an integer
  do {
    cout << "Enter Grade(0-100): ";
    cin >> grde;

    if ( cin.fail()){
      cout << "Invalid Input Type! Enter integer!" << endl;
      cin.clear();
      cin.ignore(100, '\n');  
    } else {
      validChoice = true;
    }
  } while(!validChoice);
  cout << endl;
  
  //Specification B2 - Add Elements
  // Resize the array and add the new grade using the temporary array pTmp as a placeholder
  int* pTmp = new int[arrSize + 1];
  for (int i = 0; i < arrSize; i++) {
    pTmp[i] = stuScore[i];
  }
  pTmp[arrSize] = grde; 
  arrSize++;

  // Deallocate memory for the old array and update the pointer
  delete[] stuScore;
  stuScore = pTmp;
  
}   

void printScores(int*& stuScore, int arrSize){
  // Specification C2 - Print Scores
  // Print out the test scores entered when the user chooses the number 2 in the menu
  char grdLtr;
  int grdeNum = 0;

  cout << endl;
  cout << "#    Score    Grade" << endl;
  cout << "-    -----    -----" << endl;
  for (int i = 0; i < arrSize; i++){    
    grdLtr = Grade2Lttr(stuScore[i]);    // Set the grade letter accordingly
    
    // Print out everything
    // If a grade is an F, make the grade number and letter print out in red, else print out regularly
    FancyText failGrde;
    if (grdLtr == 'F') {
      string tempLtr = string(1, grdLtr);
      cout << grdeNum << setw(17) << right << failGrde.redText(to_string(stuScore[i]));
      cout << setw(17) << failGrde.redText(tempLtr) << endl;
    } else {
      cout << grdeNum << setw(8) << stuScore[i] << setw(8);
      cout << grdLtr << endl;
    }
    
    grdeNum++;
  }
  cout << endl;
  
}

char Grade2Lttr(int stuScore){
  // Specification C3 - Letter Grades
  // Takes the numeric score from the user input and convert it into a letter grade only if it is a valid score
  char gradeLttr;

  if (stuScore > 100 || stuScore < 0){
    gradeLttr = 'X';
  } else if (stuScore >= 90) {
    gradeLttr = 'A';
  } else if (stuScore >= 80) {
    gradeLttr = 'B';
  } else if (stuScore >= 70) {
    gradeLttr = 'C';
  } else if (stuScore >= 60) {
    gradeLttr = 'D';
  } else {
    gradeLttr = 'F';
  }

  return gradeLttr;
}

// Compute the GPA
void computeGPA(int*& stuScore, int arrSize){
  // Specification C4 - Compute GPA
  // Take all the entered scores and compute the average grade when requested by the user
  int avgGrade = 0;
  int totalScre = 0;
  char grdLtr;
  int totScores = 0;

  // Only add the scores between 1 and 100 to the total score and then divide by the amount of valid scores for the average
  for (int i = 0; i < arrSize; i++){
    if (stuScore[i] <= 100 && stuScore[i] >= 0){
      totalScre = totalScre + stuScore[i];
      totScores++;
    }
  }
  avgGrade = totalScre/totScores;
  
  grdLtr = Grade2Lttr(avgGrade);  // Set the grade letter according to the calculated gpa

  // Display numeric GPA with the grade letter
  cout << endl;
  cout << "~ . ~ . ~ . ~ . ~ . ~" << endl;
  cout << "Student GPA:" << endl;
  cout << avgGrade << "  ";
  cout << grdLtr << endl;
  cout << "~ . ~ . ~ . ~ . ~ . ~" << endl;
  cout << endl;
}

// Input Validation for menu choice
int inputValid(){
  // Specification B3 - Menu Input Validation
  // Prompt, accept input, and validate the menu input of the user to be a number from 1 through 4
  int menuSelect;
  bool validChoice = false;

  do {
    // Prompt and accept user's menu choice
    cout << "Enter menu choice: ";
    cin >> menuSelect;

    // Validate the input is within the 1 through 4 limits of the menu and is an integer
    if ( cin.fail() || menuSelect < 1 || menuSelect > 4){
      cout << "Invalid Input! (Please enter another choice)" << endl;
      cin.clear();
      cin.ignore(100, '\n');  
    } else {
      validChoice = true;    // exit loop
    }
  } while(!validChoice);

  return menuSelect;
}

// Display Menu
void mainMenuP(){
  //Display the main menu and its four options
  cout << "==========" << endl;
  cout << "Main Menu:" << endl;
  cout << "==========" << endl;
  cout << "1. Add Grade" << endl;
  cout << "2. Display All Grades" << endl;
  cout << "3. Process All Grades" << endl;
  cout << "4. Quit the program" << endl;
  cout << "=====================" << endl;
  cout << endl;
  
}

void UnitTest(){
  // Specification A4 - Unit Test
  // Run CompTest and test the Grade2Lttr Function
  Date randDate;
  cout << "   -   " << endl;
  randDate.CompTest();
  cout << "   -   " << endl;

  // Grade to Letter function tester
  // Correct Grade Output: F, F, A, X, B, C, D, X
  int testNum[] = {0, 10, 100, 200, 85, 79, 60, -23};
  
  char testGrde;
  for(int i = 0; i < 8; i++){
    testGrde = Grade2Lttr(testNum[i]);
    cout << testNum[i] << ":";
    cout << testGrde << "  ";    
  }
  cout << endl;
  cout << endl;
}

// Program Greeting
void ProgramGreeting(){
  // Specification C1 - Program Greeting Function
  // Display the welcome to the program, name, and the due date of the assignment.
  cout << endl;
  cout << "= = = = = = = = = = = = = = =" << endl;
  cout << "Welcome to the GPA Analyzer." << endl;
  cout << "Made by: Samantha Siew" << endl;
  cout << "Due: September 24, 2023" << endl;
  cout << "= = = = = = = = = = = = = = =" << endl;
  cout << endl;
}