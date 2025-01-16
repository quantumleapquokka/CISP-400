//InvInq.cpp
//Samantha Siew, CISP 400
//10/22/23

#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <ctime>
#include <random>
#include <cstdlib>
using namespace std;

// Specification B1 - Date class
// Get the system date and return it in a mm/dd/yyyy format
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
    if(testing.getYr() == 2023) {
      cout << "GetDate works!" << endl;
    } else {
      cout << "Test failed!" << endl;
    }
  }
};

//Specification B2 -RandNo Class
// Return a random number used for the wholesale cost
class RandNo {
private:
  // Specification B3 - Singleton Design Pattern
  // Time set seed code will only run once
  static RandNo* instance;
  RandNo(){
    double seed = time(NULL);
    srand(seed);
  }
public:
  static RandNo* getInstance(){
    if(!instance){
      instance = new RandNo; 
    }
    return instance;
  }

  // Return a random nubmer within the minimum and maximum bounds passed in
  double randNum(double dmin, double dmax){
    double randNum = (double)rand()/RAND_MAX;
    return dmin + randNum * (dmax - dmin);
  }

  // Specification A3 - Overload operator»
  // Overload the stream insertion operator to allow the user to enter a specified seed number
  friend istream & operator >> (istream &in, RandNo&r){
    int seedEnt;
    while (true) {
      cout << endl;
      cout << "=======Inventory Inquisitor=======" << endl;
      cout << "Enter seed number for random number: ";
      in >> seedEnt;    // User specified seed input

      // Validate that an integer was entered
      if (in.fail()) {
        in.clear();
        in.ignore(100, '\n'); 
        cout << "Invalid input. Please enter an integer." << endl;
      } else {
        break;
      }
    }

    srand(seedEnt);    
    return in;
  }

  void ComponentTest() {
    RandNo* r = RandNo::getInstance();
    cout << "RandNo ComponentTest" << endl;
    cout << "Random number(10-100): " << r->randNum(10, 100) << endl;
  }
};

// Initialize to 0
RandNo* RandNo::instance = 0;

// Inventory class to contain all the necessary information about each item within the inventory
class Inventory {
private:
  RandNo* randGen;
  int itemID;
  int quantity;
  double wholesaleCost;
  double retailCost;
  Date dateAdded;
public:
  Inventory(){
    randGen = RandNo::getInstance();
    itemID = 0;
    quantity = 0;
    wholesaleCost = randGen->randNum(0.0, 1000.0);  // Initialize wholesale to a random number between 0 and 1000
    retailCost = wholesaleCost * (1 + randGen->randNum(.1, .5));    // Calculate the random 10%-50% increase for retail 
  }

  // Specification A2 - Overload operator«
  // Overload the stream extraction operator to display the inventory item
  friend ostream & operator << (ostream &output, Inventory &inv) {
    output << inv.getItemID() << setw(12);
    output << inv.getQuantity() << setw(21);
    output << fixed << setprecision(2) << inv.getWholesaleCost() << setw(22);
    output << fixed << setprecision(2) << inv.getRetailCost() << setw(22);
    output << inv.date().getDate() << endl;
    return output;
  }

  // Set the item ID
  void SetID(int id){
    itemID = id;
  }

  // Set the quantity
  void SetQuantity(int q){
    quantity = q;
  }

  // Get the item ID
  int getItemID() {
    return itemID;
  }

  // Get the quantity
  int getQuantity() {
    return quantity;
  }

  // Return the wholesale cost
  double getWholesaleCost() {
    return wholesaleCost;
  }

  // Return the retail cost
  double getRetailCost() {
    return retailCost;
  }

  // Return the date added
  Date date() {
    return dateAdded;
  }

  // Component test to test each module within the class
  void ComponentTest(){
    Inventory inventory;
    int testID = 12345;
    int testQ = 5;

    // Set id and quantity
    inventory.SetID(testID);
    inventory.SetQuantity(testQ);

    // Verify the things are set correctly
    int getQ = inventory.getQuantity();
    int getID = inventory.getItemID();
    int getWSC = inventory.getWholesaleCost();
    int getRTC = inventory.getRetailCost();
    if (getQ == testQ && getID == testID) {
      cout << "ID test: PASSED" << endl;
      cout << "Quantity test: PASSED" << endl;
    } else {
      cout << "ID test: PASSED" << endl;
      cout << "Quantity test: FAILED" << endl;
    }
    // Return the wholesale and retail costs
    cout << "Get Wholesale and Retail Costs test:" << endl;
    cout << "Wholesale Cost: " << getWSC << endl;
    cout << "Retail Cost:" << getRTC << endl;
  }
};

// Specification B4 - Inventory Entry Input Validation
// Validate the input for the first three validation rules for itemID, quantity, and wholesale cost
class InVal {
private:
public:
  InVal(){}
  static bool validateID(int& inventoryID, Inventory**& inventoryArr, int& arrSize){
    bool valid = false;
    int length;

    // Get intput for ID until it is 5 digits long and also make sure it is an integer
    do {
      length = 0;

      cin.clear();
      cin.ignore(100, '\n');

      // Get the id input
      cout << "Enter item ID: ";
      cin >> inventoryID;

      // Get the length of the id that was inputed
      string idStr = to_string(inventoryID);
      length = idStr.length();

      // Make sure an integer was actually inputed, then make sure it is 5 digits and then make sure it is not a duplicate
      if (cin.fail()) {
        cout << "Enter a 5-digit integer!" << endl;
      } else if (length != 5) {
        cout << "Invalid ID. Must be 5 digits." << endl;
      } else if (inventoryID < 0) {
        cout << "Invalid ID!" << endl;
      } else {
        valid = true;
        // Check for a duplicate item id
        for (int i = 0; i < arrSize; i++) {
          if (inventoryArr[i] != nullptr && inventoryID == inventoryArr[i]->getItemID()) {
            cout << "Item ID already exists." << endl;
            valid = false;
            break;
          }
        }
      }
    } while (!valid);

    return true;
  }

  static bool validEditID(int& idEdit, Inventory**& inventoryArr, int& arrSize){
    bool valid = false;
    int length;

    do {
      length = 0;

      cin.clear();
      cin.ignore(100, '\n');

      // Input
      cout << "Enter item ID: ";
      cin >> idEdit;

      // Get the length of the ID entered
      string idStr = to_string(idEdit);
      length = idStr.length();

      // Make sure integers were inputed then check if a 5 digit id was entered, then check that it is an item in the inventory that can be edited
      if (cin.fail()) {
        cout << "Enter an ID!" << endl;
      } else if (length != 5) {
        cout << "Invalid ID. Must be 5 digits." << endl;
      } else if (idEdit < 0) {
        cout << "Invalid ID!" << endl;
      } else {
        //check if the item is in the inventory
        for (int i = 0; i < arrSize; i++) {
          if (idEdit == inventoryArr[i]->getItemID()) {
            valid = true;
            break;
          } 
        }
        if (!valid) {
          cout << "Item ID does not exist." << endl;
        }
      }
    } while (!valid);

    return true;
  }

  static bool validQ(int& quantity, int& arrSize){
    bool valid = false;
    do{
      cin.clear();
      cin.ignore(100, '\n');
      cout << "Enter Quantity: ";

      if (cin >> quantity) {
        if (quantity >= 0 && cin.peek() == '\n') {
          valid = true;
        } else {
          cout << "Invalid Quantity!" << endl;
        }
      } else {
        cout << "Invalid Input!" << endl;
      }
    } while (!valid);

    return true;
  }
};


// Function Prototypes
void ProgramGreeting();
void alphaMenu();
char inputValid();
void addInventory(Inventory **&inventoryArr, int& arrSize);
void dlteInventory(Inventory**& inventoryArr, int& arrSize);
void edtInventory(Inventory**& inventoryArr, int arrSize);
void prntIvtry(Inventory **inventoryArr, int arrSize);
void UnitTest();


int main() {
  // Program Greeting
  ProgramGreeting();

  // Specification A4 - UnitTest() function in main()
  // Test all the component tests inside each class
  UnitTest();

  // Allow the user to enter a random seed number
  RandNo* randGen = nullptr;
  cin >> *randGen;
  
  // Variables
  char alphaInput;
  bool stop = false;

  // Print out menu
  alphaMenu();

  // Specification C2 - Dynamic Array
  // Dynamic Array of the inventory
  int arrSize = 1;
  Inventory** inventoryArr = new Inventory*[arrSize];
  inventoryArr[0] = nullptr;
  
  // Keep running the program until the user quits
  do{
    // Ask for user input for menu and validate it
    alphaInput = inputValid();

    // Switch statement for modifying the inventory based on the input for the menu
    switch(alphaInput){
      // Add an item to the inventory with its 5 components
      case 'A': 
        cout << endl;
        cout << "- - - - - - " << endl;
        addInventory(*&inventoryArr, arrSize);
        cout << "- - - - - - " << endl;
        cout << endl;
        break;
      // Delete the latest entered item that was in the inventory a.k.a the last item
      case 'D':
        dlteInventory(*&inventoryArr, arrSize);
        if (arrSize == 0){
          arrSize = 1;
          inventoryArr = new Inventory*[arrSize];
          inventoryArr[0] = nullptr;
        }
        break;
      // Edit the quantity of the item that the user has chosen to edit
      case 'E':
        edtInventory(*&inventoryArr, arrSize);
        cout << endl;
        break;
      // Print the entire inventory
      case 'P':
        prntIvtry(*&inventoryArr, arrSize);
        break;
      default: 
        // Display goodbye message when the user enters 'a' (quiting the program)
        cout << endl;
        cout << "▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵▵" << endl;
        cout << "Exited Inventory Inquisitor Program." << endl;
        cout << "▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿▿" << endl;
        stop = true;
        break;
    }
  }while(!stop);
  
  return 0;
}

// UnitTest() function for testing each component test function in each class
void UnitTest(){
  Date date;
  RandNo* randN = nullptr;
  Inventory inventory;

  cout << "ComponentTest for each class" << endl;
  cout << "v---v---v---v---v---v---v---v" << endl;

  // Call ComponentTest() for each class
  date.ComponentTest();
  randN->ComponentTest();
  inventory.ComponentTest();

  cout << "v---v---v---v---v---v---v---v" << endl;
  cout << endl;
}

// Add the item id and quantity to the inventory and the wholesale and retail cost are randomized
void addInventory(Inventory**& inventoryArr, int& arrSize){
  int inventoryID;
  int quantity;
  
  if (InVal::validateID(inventoryID, *&inventoryArr, arrSize) && InVal::validQ(quantity, arrSize)){
    // Create new inventory and set the entered values to the according spot in the array
    Inventory *item = new Inventory();
    item->SetID(inventoryID);
    item->SetQuantity(quantity);
    item->date();

    if (inventoryArr[0] == nullptr)
    {
      inventoryArr[0] = item;
    }
    else {
      // Specification C3 - Resize Array
      // Resize the array by adding a spot for the item that the user has just entered
      Inventory** pTmp = new Inventory*[arrSize + 1];

      for (int i = 0; i < arrSize; i++) {
        pTmp[i] = inventoryArr[i];
      }

      pTmp[arrSize] = item;
      arrSize++;

      // Deallocate memory for the old array and update the pointer
      delete[] inventoryArr;
      inventoryArr = pTmp;
      pTmp = nullptr;
    }
  }
  
}

// Delete the latest item in the inventory
void dlteInventory(Inventory**& inventoryArr, int& arrSize){
  arrSize--;
  
  Inventory** pTmp = new Inventory*[arrSize];
  
  for (int i = 0; i < arrSize; i++) {
    pTmp[i] = inventoryArr[i];
  }
  
  // Deallocate memory for the old array and update the pointer
  delete[] inventoryArr;
  inventoryArr = pTmp;
  pTmp = nullptr;
}

// Specification A1 - Edit Inventory
// Edit the inventory by entering an ID and then changing the quantity
void edtInventory(Inventory**& inventoryArr, int arrSize){
  int idEdit;
  int qChange;

  cout << endl;
  cout << "Enter the item ID of the item you want to edit, followed by the new quantity." << endl;
  
  if (InVal::validEditID(idEdit, inventoryArr, arrSize) && InVal::validQ(qChange, arrSize)) {
    // Loop through the array to find the item with the matching ID and reset the quantity to whatever the user has entered
    for (int i = 0; i < arrSize; i++) {
      if(inventoryArr[i]->getItemID() == idEdit){
        inventoryArr[i]->SetQuantity(qChange);
      }
    }
  }
  
}

// Print out the inventory in a formatted chart
void prntIvtry(Inventory **inventoryArr, int arrSize){
  cout << endl;
  cout << setw(50) << "===============" << endl;
  cout << setw(50) << "|  INVENTORY  |" << endl;
  cout << setw(50) << "===============" << endl;
  cout << "Item ID:" << setw(15) << "Quantity:" << setw(20) << "Wholesale Cost:" << setw(20) << "Retail Cost:" << setw(20) <<"Date Added:" << endl;
  cout << "--------" << setw(15) << "---------" << setw(20) << "---------------" << setw(20) << "------------" << setw(20) << "-----------" << endl;

  // If there is nothing in the inventory, print out the titles but display that it is empty, otherwise display the inventories
  if(inventoryArr[0] != nullptr){
    for (int i = 0; i < arrSize; i++) {
      cout << *inventoryArr[i] << endl;
    }
  } else {
    cout << endl;
    cout << "There is nothing in your inventory." << endl;
    cout << endl;
  }
  
}

// Input Validation for menu choice
char inputValid(){
  // Specification C4 - Menu Input Validation
  // Prompt, accept input, and validate the menu input of the user to be character within the menu choices
  char alphaSelect;
  bool validChoice = false;

  do {
    // Prompt for the user's menu choice and
    cout << "Enter menu choice: ";
    cin >> alphaSelect;
    alphaSelect = toupper(alphaSelect);

    // Validate the input is within the 1 through 4 limits of the menu and is an integer
    if ( cin.fail() || (alphaSelect != 'A' && alphaSelect != 'D' && alphaSelect != 'E' && alphaSelect != 'P' && alphaSelect != 'Q')){
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

// Menu
void alphaMenu() {
  // Specification C1 - Alpha Menu
  // Menu with 5 choices to modify the inventory or quit the program
  cout << "=====================" << endl;
  cout << "|     Main Menu     |" << endl;
  cout << "=====================" << endl;
  cout << " • <A>dd Inventory" << endl;
  cout << " • <D>elete Inventory" << endl;
  cout << " • <E>dit Inventory" << endl;
  cout << " • <P>rint Inventory" << endl;
  cout << " • <Q>uit Program" << endl;
  cout << "=====================" << endl;
  cout << endl;
}

// Program Greeting
void ProgramGreeting(){
  // Display the welcome to the program, name, and the due date of the assignment.
  cout << endl;
  cout << "= = = = = = = = = = = = = = =" << endl;
  cout << "Welcome to the Inventory Inquisitor Program." << endl;
  cout << "Made by: Samantha Siew" << endl;
  cout << "Due: October 22, 2023" << endl;
  cout << "= = = = = = = = = = = = = = =" << endl;
  cout << endl;
}