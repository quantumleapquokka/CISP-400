//Hokeemon.cpp
//Samantha Siew, CISP 400
//11/26/2023

#include <iostream>
#include <string>
#include <cstdlib> 
#include <random>
#include <ctime>
using namespace std;

// Date Class to return system date formatted
class Date {
private:
  int mon, day, yr;
public:
  Date(){
    time_t currTime = time(0);
    tm *ltm = localtime(&currTime);
    day = ltm->tm_mday;
    mon = 1 + ltm->tm_mon;
    yr = 1900 + ltm->tm_year;
  }

  string getDate() {
    // Return the date as a formatted string
    return to_string(mon) + "/" + to_string(day) + "/" + to_string(yr);
  }

  // Return the year
  int getYr(){
    return yr;
  }

  // Component test to check that the correct year is returned
  void ComponentTest(){
    Date testing;
    cout << "DATE CLASS" << endl;
    cout << "- - - - - -" << endl;

    // Test getYr function to return 2023
    if(testing.getYr() == 2023) {
      cout << "getYr: PASSED" << endl;
    } else {
      cout << "getYr: FAILED" << endl;
    }

    // Test that getDate is returning the system date
    if(testing.getDate() == (to_string(mon) + "/" + to_string(day) + "/" + to_string(yr))) {
      cout << "getDate: PASSED" << endl;
    } else {
      cout << "getDate: FAILED" << endl;
    }
    cout << endl;
  }
};

// struct to make an addable menu
struct MenuItem {
    char letter;
    string description;

    // Constructor to initialize the menu item
    MenuItem() : letter(' '), description(""){}
    MenuItem(char ltr, const string& descr) : letter(ltr), description(descr) {}
};

struct Menu {
    static const int maxItems = 10; // Maximum number of menu items
    MenuItem items[maxItems];
    int itemCount = 0;

    // Add the menu items
    void makeMenu(char ltrC, const string& descr) {
      if (itemCount < maxItems) { // Ensure you don't exceed the array size
        items[itemCount] = MenuItem(ltrC, descr);
        itemCount++;
      }
    }

    // Method to print all menu items
    void prtMenu() {
      for (int i = 0; i < itemCount; i++) {
        cout << " ➡ [" << items[i].letter << "]" << items[i].description << endl;
      }
    }

    // First menu header for start of program
    void menuHeader() {
      cout << "☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆" << endl;
      cout << "  H O K E E M O N ~ P R O G R A M ~ M E N U" << endl;
      cout << "☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆" << endl;
    }

    // Method for menu header
    void header() {
      cout << endl;
      cout << "~ M E N U ~" << endl;
      cout << "- - - - - -" << endl;
    }

  bool hasCombined = false;
  
  // Method to remove the 'C' option from the menu
  void removeCombineOption() {
    for (int i = 0; i < itemCount; i++) {
      if (items[i].letter == 'C') {
        // Shift items to remove the 'C' option
        for (int j = i; j < itemCount - 1; j++) {
          items[j] = items[j + 1];
        }
        itemCount--;
        hasCombined = true; // Set the flag to true
        break;
      }
    }
  }
};

// Specification C2 - Creature class
// Used to instantiate hokeemon (not in use, child class is used to instantiate)
// Specification B2 - Virtual Class Creature
// Turned into Abstract Base Class
class Creature{
  friend ostream& operator<<(ostream& out, const Creature& hmon);
  friend istream& operator>>(istream& in, Creature& hmon);
private:
  int boredom;
  int hunger;
  string name;
  char menuSt;
  bool alve = true;
  string species;
public:
  Creature(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, 5);
    hunger = distribution(gen);
    boredom = distribution(gen);
  }

  // Abstract Base Class
  virtual void makeATurn() = 0;

  // Prototypes
  char Menu();
  void PassTime();
  void nameIt(string name);
  int interaction(int ranMin, int ranMax);
  bool checkAliveness(bool display);
  void setName(string n, string sp);
  string getName() const;
  void ComponentTest();
};

void Creature::setName(string n, string sp){
  name = n;
  species = sp;
}

string Creature::getName() const{
  return name;
}

// Specification C3 - Validate Input
// Take the menu choice of the user and validate it until it is a valid choice and assign it to the menuSt variable
char Creature::Menu(){
  char action;

  // Allow the user to enter an action and verify that it is a valid choice
  do {
    cout << endl;
    cout << "- - - - - - - - -" << endl;
    cout << "ENTER ACTION: ";
    cin >> action;
    cout << "- - - - - - - - -" << endl;

    action = toupper(action);

    if(action != 'L' && action != 'P' && action != 'F' && action != 'Q'){
      cout << "INVALID ACTION!" << endl;
      cin.clear();
      cin.ignore(1000, '\n');
      cout << endl;
    } else if (action == 'Q'){
      // Quit program if they enter Q
      cout << endl;
      cout << "☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆" << endl;
      cout << "Exited Program..." << endl;
      cout << "☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆" << endl;
      exit(0);
    }
  } while (cin.fail());

  return action;
}

// Specification C1 - PassTime()
// Increase boredom by one and decrease hunger by one
void Creature::PassTime() {
  hunger--;
  boredom++;
}

// Generate a random number based on the maximum and minimum which will apply to the interactions play and feed
int Creature::interaction(int ranMin, int ranMax){
  int ranNum;
  ranNum = rand() % (ranMax - ranMin + 1) + ranMin;

  return ranNum;
}

// Check if the hokeemon is alive based on the hunger and boredom levels
bool Creature::checkAliveness(bool display){
  bool aliveAndWell = true;
  
  if (hunger < 0 && display){
    cout << "☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~" << endl;
    cout << "Your Hokeemon has died of starvation!" << endl;
    cout << "☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~" << endl;
    cout << endl;
    aliveAndWell = false;
  }
  
  if (boredom > 20 && display){
    cout << "☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆" << endl;
    cout << "Your Hokeemon has slipped into an unresponsive catatonic state!" << endl;
    cout << "☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆" << endl;
    aliveAndWell = false;
  }

  if (boredom > 20 && hunger < 0) {
    aliveAndWell = false;
  }

  return aliveAndWell;
}

void Creature::ComponentTest(){
  bool display = false;
  hunger = -1;
  boredom = 31;

  cout << "CREATURE CLASS" << endl;
  for (int i = 0; i < 8; i++){
    cout << "- ";
  }
  cout << endl;

  // Check to see that the method is correctly assessing passing hunger and boredom levels
  if (checkAliveness(display) == false){
    cout << "checkAliveness: PASSED" << endl;
  } else {
    cout << "checkAliveness: FAILED" << endl;
  }

  // Check to see method is correctly setting name
  setName("Test", "(Test)");
  if (name == "Test"){
    cout << "setName: PASSED" << endl;
  } else {
    cout << "setName: FAILED" << endl;
  }
  
}

// Specification C4 - Overload «
// Overload the insertion operator to display Hokeemon information
ostream& operator<<(ostream& out, const Creature& hmon) {
  if (hmon.alve) {
    out << "Name(Species): " << hmon.name << " " << hmon.species << endl;
    out << "Boredom: " << hmon.boredom << endl;
    out << "Hunger: " << hmon.hunger << endl;
  } else {
    out << "☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆" << endl;
    out << hmon.name << " is dead :(" << endl;
    out << "☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆" << endl;
  }
  return out;
}

istream& operator>>(istream& in, Creature& hmon) {
  bool alive;
  int rand;
  hmon.menuSt = hmon.Menu();
  //Have the creature display its current hunger and boredom status
  if(hmon.menuSt == 'L'){
    cout << hmon;
  } else if (hmon.menuSt == 'P') {
    //Randomly decrease the boredom levels
    rand = hmon.interaction(4, 8);
    hmon.boredom = hmon.boredom - rand;
    cout << "-" << rand << " boredom" << endl;    // Display the decrease 
    cout << endl;
  } else if (hmon.menuSt == 'F') {
    // Randomly decrease hunger levels
    rand = hmon.interaction(4, 8);
    hmon.hunger = hmon.hunger + rand;
    cout << "-" << rand << " hunger" << endl;    // Display the decrease
    cout << endl;
  }

  // Increment the hunger and boredom levels
  hmon.PassTime();
  
  // Check if it is alive
  alive = hmon.checkAliveness(true);
  if (!alive) {
    hmon.alve = false;
  }
  return in;
}



// Specification B1 - Child Class
// Child class to the creature class that will be used to play the game
class Hokeemon : public Creature {
  friend Hokeemon operator+(const Hokeemon& hmon1, const Hokeemon& hmon2);

private:
  string name;
  int boredom;
  int hunger;
public:
  Hokeemon():Creature() { }

  //Specification B3 - Overload Assignment Operator
  // Allow copying of atributes from one Hokeemon to another
  Hokeemon& operator=(const Hokeemon& other) {
    if (this != &other) {
      // Copy attributes from other Hokeemon
      setName(other.getName(), "(Dwaekki)");
    }
    return *this;
  }
  
  void makeATurn() override {
    cout << "I'm a Hokeemon making a turn" << endl;
  }
};

//Specification B4 - Overload + Operator
// Merging two hokeemon's boredom and hunger legels by taking the average and asking for a name
Hokeemon operator+(const Hokeemon& hmon1, const Hokeemon& hmon2) {
  Hokeemon mergedHokeemon;

  // Average the two hokeemon's boredom and hunger levels
  mergedHokeemon.boredom = (hmon1.boredom + hmon2.boredom) / 2;
  mergedHokeemon.hunger = (hmon1.hunger + hmon2.hunger) / 2;

  // Specification A1 - Critter Name
  // Allowing the user to name the child of the two hokeemon
  do {
    cout << "Enter the name of the new Hokeemon: ";
    cin >> mergedHokeemon.name;
  } while (cin.fail());

  // Set the name of the new Hokeemon child
  mergedHokeemon.setName(mergedHokeemon.name, "(Dwaekki-evolved)");

  return mergedHokeemon;
}

// Specification A3 - Second Child Class
// Child class to the creature class(not used in current game, only for component test)
class Ditto : public Creature {
private:
  string name;
  int boredom;
  int hunger;
public:
  Ditto():Creature() { }

  // Specification A2 - Copy Constructor
  // Copy basic attributes of the creature such as name, boredom, and hunger to a second ditto creature
  Ditto(const Ditto& hmon) : Creature(hmon){
    name = hmon.name;
    boredom = hmon.boredom;
    hunger = hmon.hunger;
  }

  void makeATurn() override {
    cout << "I'm a Ditto making a turn" << endl;
  }

  void ComponentTest() {
    cout << endl;
    cout << "DITTO CLASS (Copy Constructor Test)" << endl;
    for (int i = 0; i < 3; i++) {
      cout << "- - - - - - ";
    }
    cout << endl;

    // Create ditto object and initialize its features
    Ditto ditto1;
    ditto1.name = "Bob";
    ditto1.boredom = 10;
    ditto1.hunger = 5;

    // Copy constructor usage to copy over the attributes
    Ditto ditto2(ditto1);

    // Test if the constructor sucessfuly coppied over the attributes
    if (ditto1.name == ditto2.name && ditto1.boredom == ditto2.boredom && ditto1.hunger == ditto2.hunger){
      cout << "Copy Constructor: SUCCESSFUL" << endl;
    } else {
      cout << "Copy Constructor: FAILED" << endl;
    }
  }
};

// Functs Prototypes
void ProgramGreeting();
void UnitTest();

int main() {
  // Functions
  ProgramGreeting();
  UnitTest();
  cout << endl;

  // Variables
  char hChoice;
  int min = 1;
  int max = 2;
  bool exit = true;

  // Create object
  //Creature pig;
  Hokeemon pig;
  Hokeemon rabbit;
  Hokeemon dwaekki;

  // Set the name and species of the first two hokeemon
  pig.setName("Oink", "(Dwaekki-unevolved)");
  rabbit.setName("Tokki", "(Dwaekki-unevolved)");

  // Create Main Menu
  Menu menu;
  menu.menuHeader();
  menu.makeMenu('L', "isten: get the stats of the hokeemon");
  menu.makeMenu('P', "lay: play and decrease boredom");
  menu.makeMenu('F', "eed: feed and decrease hunger");
  menu.makeMenu('Q', "uit Program");
  menu.prtMenu();
  cout << "☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆" << endl;
  cout << endl;

  // Menu of hokeemons that exist in the game
  Menu hokeemons;
  hokeemons.makeMenu('1', pig.getName());
  hokeemons.makeMenu('2', rabbit.getName());
  hokeemons.makeMenu('C', "ombine: combine hokeemon to make a child");

  // Basic instructions
  cout << "☆ Enter a # of a hokeemon or combine hokeemons, followed by what action you want to do (L,P,F,Q) ☆" << endl;
  
  do{
    cout << endl;
    cout << "☆ - ☆ - ☆ - ☆ - ☆" << endl;
    cout << "Current Hokeemons(Name): " << endl;
    cout << "☆ - ☆ - ☆ - ☆ - ☆" << endl;
    hokeemons.prtMenu();
    cout << "☆ - ☆ - ☆ - ☆ - ☆" << endl;
    
    // Input
    do {
      cout << endl;
      cout << "Select a hokeemon by # or combine[C]: ";
      cin >> hChoice;

      // check if the entered character was to C or combine
      if (hChoice == 'C' || hChoice == 'c') {
        hChoice = 'C'; // Convert to uppercase
        break;
      }

      // turn into int
      hChoice = hChoice - '0';

      // check if the choice of hokeemon the user entered exists
      if (hChoice < min || hChoice > max || cin.fail()) {
        cout << "Invalid choice. Please try again." << endl;
        cout << endl;
        exit = false;
      } else {
        exit = true;
        break; // valid input, exit the loop
      }
      
    } while (!exit);

    // Interaction Options
    // Enter actions for a hokeemon based off of what the user entered previously
    if (hChoice == 1) {
      cin >> pig;
    } else if (hChoice == 2) {
      cin >> rabbit;
    } else if (hChoice == 'C') {
      // combine the hokeemon to create a child from the existing hokeemons and remove the option to combine more hokeemon
      hokeemons.removeCombineOption();
      dwaekki = pig + rabbit;
      hokeemons.makeMenu('3', dwaekki.getName());  // add interaction ability to menu
      max = 3;
    } else if (hChoice == 3 && max == 3) {
      cin >> dwaekki;
    }

    // Reprint menu with headder
    cout << "☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆" << endl;
    menu.header();
    menu.prtMenu();
  } while (true);

  return 0;
}

// Create object and test the component test method in each class accordingly
void UnitTest(){
  // Create the objects
  Date date;
  Hokeemon hmon;
  Ditto ditto;

  cout << "ComponentTest for each class" << endl;
  cout << "============================" << endl;

  // Call ComponentTest() for each class
  date.ComponentTest();
  hmon.ComponentTest();
  ditto.ComponentTest();

  // Specification A4 - Write a Lambda
  // Lambda function to display message
  auto displayMessage = []() {
    cout << "Lambda function used in UnitTest" << endl;
  };

  cout << endl;
  cout << "LAMBDA FUNCTION" << endl;
  cout << "- - - - - - - -" << endl;
  displayMessage();

  for(int i = 0;i < 20; i++){
    cout << "=";
  }
  cout << endl;
}

// Program Greeting
void ProgramGreeting(){
  Date sstmDte;
  // Display the welcome to the program, name, and the due date of the assignment.
  cout << endl;
  cout << "= = = = = = = = = = = = = = = = " << endl;
  cout << "Welcome to the HOKEEMON Program." << endl;
  cout << "Made by: Samantha Siew" << endl;
  cout << "Date:" << sstmDte.getDate() << endl;
  cout << "= = = = = = = = = = = = = = = = " << endl;
  cout << endl;
}