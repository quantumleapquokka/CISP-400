//TodoList.cpp
//Samantha Siew, CISP 400
//11/5/2023

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

// Date Class to return system date but formatted
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

  //for m/d/yyyy string to Date
  Date(string s) {
    int pos = 0;
    string m, d, y;
    pos = s.find("/");
    m = s.substr(0, pos);
    s.erase(0, pos + 1);
    pos = s.find("/");
    d = s.substr(0, pos);
    s.erase(0, pos + 1);
    y = s;
    mon = stoi(m);
    day = stoi(d);
    yr = stoi(y);
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
        cout << "➡ [" << items[i].letter << "] " << items[i].description << endl;
      }
    }

    // Method for menu header
    void header() {
      cout << endl;
      cout << "~ M E N U ~" << endl;
      cout << "- - - - - -" << endl;
    }
};

// Class for the todo list
class TodoLzt{
  friend ostream& operator<<(ostream&, const TodoLzt&);
  friend istream& operator>>(istream&, TodoLzt&);
private:
  string toDoThng;
  int todoIDNum;
  Date dteAdd;
  string enterInpt;
  bool isRemoved;

public:
  //Specification C4 - TODO array
  int arrSze = 0;
  TodoLzt** toDo = new TodoLzt*[arrSze];
  int highestID = 0;

  TodoLzt(){
    toDo = nullptr;
    toDoThng = "";
    todoIDNum = 0;
    // Specification A3 - System Date
    dteAdd = Date();
    isRemoved = false;
  }

  // Specification A4 - Overload Constructor
  TodoLzt(string descr) {
    TodoLzt();
    toDoThng = descr;
    todoIDNum = 0;
  }

  // Specification A1 - Overload Copy Constructor
  TodoLzt(const TodoLzt& todo) {
    this->toDoThng = todo.toDoThng;
    this->todoIDNum = todo.todoIDNum;
    this->dteAdd = todo.dteAdd;
    this->isRemoved = todo.isRemoved;
  }

  // Specification A2 - Overload Assignment Operator
  TodoLzt& operator=(const TodoLzt& todo) {
    this->toDoThng = todo.toDoThng;
    this->todoIDNum = todo.todoIDNum;
    this->dteAdd = todo.dteAdd;
    this->isRemoved = todo.isRemoved;
    return *this;
  }


  void formatting(int ammt);
  Date date() const;
  void ComponentTest();
  void printToDo();
  void addToDo();
  void dlteToDo(int id);
  void saveToDo(const string& fileN);
  void loadToDo(const string& fileN);
};

// Formatting
void TodoLzt::formatting(int ammt) {
  for (int i = 0; i < ammt; i++) {
    cout << "- ";
  }
  cout << endl;
}


// Return the system date
Date TodoLzt::date() const {
  return dteAdd;
}

// Specification C3 - Test TODO’s
// Generate 5 todo's and print them out to make sure it's correct
void TodoLzt::ComponentTest(){
  TodoLzt todoTST[5]; // Create an array of 5 TodoLzt objects

  cout << "TODO LIST CLASS" << endl;
  formatting(9);
  cout << "|  Date Added  |  To-Do ID  |  To-Do Description" << endl;
  cout << "|  ==========  |  ========  |  =================  " << endl;
  
  // Set TODO for each description and id
  todoTST[0].toDoThng = "Task 1";
  todoTST[0].todoIDNum = 1;

  todoTST[1].toDoThng = "Task 2";
  todoTST[1].todoIDNum = 2;

  todoTST[2].toDoThng = "Task 3";
  todoTST[2].todoIDNum = 3;

  todoTST[3].toDoThng = "Task 4";
  todoTST[3].todoIDNum = 4;

  todoTST[4].toDoThng = "Task 5";
  todoTST[4].todoIDNum = 5;

  // Display the TODO items
  for (int i = 0; i < 5; i++) {
    cout << todoTST[i];
  }

  // Test the assignment operator and the copy constructor
  TodoLzt todo1;
  TodoLzt todo3;
  TodoLzt todo4;
  
  todo1.toDoThng = "copy constructor test";
  todo1.todoIDNum = 6;
  cout << todo1;
  
  TodoLzt todo2(todo1); // copy constructor
  cout << todo2;

  todo4.toDoThng = "assignment operator test";
  todo4.todoIDNum = 7;
  cout << todo4;
  
  todo3 = todo4; // assignment operator
  cout << todo3;
}

// Specification C1 - Overload «
// Output the whole todo list formatted into a table
ostream& operator<<(ostream& out, const TodoLzt& todo){
  int space = 12;
  if (todo.todoIDNum >= 10) {
    space = 11;
  }
  out << "|  " << todo.date().getDate() << setw(6) << "  |  " << todo.todoIDNum << setw(space) << " |  " << todo.toDoThng << endl;
    out << "|              |            |    " << endl;
  return out;
}
  
//Specification C2 - Overload »
// Allow for input and based on the command that the user enters, add to, delete from, or display the todo list
istream& operator>>(istream& in, TodoLzt& todo){
  // Variables
  int id = 0;
  static int defaultID = 0;
  static int idNum = todo.highestID;
  string command;
  string description;

  in.clear();
  in.ignore(100, '\n');  
  cout << endl;
  cout << "ENTER COMMAND: ";
  in >> command;

  if (command == "+") {
    in.ignore();  
    getline(in, description);

    TodoLzt newTodo;

    if (!description.empty()) {
      // Modify the temporary object's `toDoThng` with the client-supplied description
      newTodo.toDoThng = description;
      todo.dlteToDo(defaultID);
      idNum++;
      newTodo.todoIDNum = idNum;
    } else {
      newTodo = TodoLzt("According to all known laws of aviation, there is no way a bee should be able to fly");
    }

    

    // Use the assignment operator to copy the data from newTodo to todo
    todo = newTodo;
    todo.addToDo();
    
    // idNum++;
    // todo.todoIDNum = idNum;
    // todo.addToDo();
  } else if (command == "?") {
    todo.printToDo();
  } else if (command == "-") {
    if (todo.arrSze == 0) {
      todo.formatting(19);
      cout << "No tasks found. Deletion not possible." << endl;
      todo.formatting(19);
    } else {
      in.ignore(); 
      in >> id;
      todo.dlteToDo(id);
    }
  } else if (command == "X" || command == "x") { 
    todo.saveToDo("todo.txt");
    cout << endl;
    cout << "☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆" << endl;
    cout << "Exited Program..." << endl;
    cout << "☆ ~ ☆ ~ ☆ ~ ☆ ~ ☆" << endl;
    exit(0);
  } else {
    cout << "Invalid Command, Try again!" << endl;
  }
  
  return in;
}

// Specification B2 - ? Symbol
// Print out all the tasks with the date added, id, and task description
void TodoLzt::printToDo(){
  bool extTask = false;
  
  cout << endl;
  cout << "            " << "☆ ☆ ☆ ☆ ☆ ☆ ☆ ☆ ☆ ☆ ☆" << endl;
  cout << "            " << "☆ T O-D O   L I S T ☆" << endl;
  cout << "            " << "☆ ☆ ☆ ☆ ☆ ☆ ☆ ☆ ☆ ☆ ☆" << endl;
  cout << "|  Date Added  |  To-Do ID  |  To-Do Description" << endl;
  cout << "|  ==========  |  ========  |  =================  " << endl;
  
  for (int i = 0; i < arrSze; i++) {
    if (toDo[i] != nullptr && !toDo[i]->isRemoved) {
      cout << *toDo[i];
      extTask = true;
    } 
  }
  
  if (!extTask) {
      cout << "   ☆  There are no tasks added at this time  ☆" << endl;
  }
}

// Specification B1 - + Symbol
// Add the to-do and the id number to the array
void TodoLzt::addToDo(){
  // Create new item and set the entered values to the according property
  TodoLzt *item = new TodoLzt();
  item->toDoThng = toDoThng;
  item->todoIDNum = todoIDNum; 

  TodoLzt** pTmp = new TodoLzt*[arrSze + 1];

  for (int i = 0; i < arrSze; i++) {
    pTmp[i] = toDo[i];
  }

  pTmp[arrSze] = item;
  arrSze++;

  // Deallocate memory for the old array and update the pointer
  delete[] toDo;
  toDo = pTmp;
  pTmp = nullptr;
}

// Specification B3 - - symbol
// Delete the to-do and the id number from the array
void TodoLzt::dlteToDo(int id){
  bool exists = false;
  
  for (int i = 0; i < arrSze; i++) {
    if (toDo[i] != nullptr && toDo[i]->todoIDNum == id && toDo[i]->isRemoved == false) {
      toDo[i]->isRemoved = true;
      exists = true;
      
      break;
    }
  }
  if (exists){
    if(id != 0){
      formatting(15);
      cout << "To-Do ID " << id << " has been deleted." << endl;
      formatting(15);
    }
  } else {
    formatting(9);
    cout << "Invalid Task ID!" << endl;
    formatting(9);
  }
}

// Specification B4 - Persistence
// Save the to-do list to a file and load it when the program is run if the file exists
void TodoLzt::saveToDo(const string& fileN){
  ofstream fileO;
  fileO.open(fileN, ios::trunc);
  for (int i = 0; i < arrSze; i++) {
    if (toDo[i] != nullptr && !toDo[i]->isRemoved) {
      fileO << date().getDate() << "|" << toDo[i]->todoIDNum << "|" << toDo[i]->toDoThng << endl;
    }
  }
  fileO.close();
}

void TodoLzt::loadToDo(const string& fileN) {
  // Variables
  int id;

  ifstream fileI(fileN);
  if (fileI.is_open()) {
    TodoLzt* newLzt;
    string aline;
    int pos = 0;
    arrSze = 0;

    TodoLzt** newToDoArray;
    while (!fileI.eof() && getline(fileI, aline)) {
      newLzt = new TodoLzt();  
      pos = aline.find('|');
      newLzt->dteAdd = Date(aline.substr(0, pos));
      aline.erase(0, pos + 1);
      
      pos = aline.find('|');
      id = stoi(aline.substr(0, pos));
      newLzt->todoIDNum = id;
      aline.erase(0, pos + 1);

      newLzt->toDoThng = aline;
      highestID = newLzt->todoIDNum;
      
      newToDoArray = new TodoLzt*[arrSze + 1];
      for (int i = 0; i < arrSze; i++) {
        newToDoArray[i] = toDo[i];
      }
      newToDoArray[arrSze] = newLzt;
      delete[] toDo;
      toDo = newToDoArray;
      arrSze++;
    }

    fileI.close();
  }

}


// Functs Prototypes
void ProgramGreeting();
void UnitTest();


int main() {
  ProgramGreeting();
  UnitTest();
  
  // Variables
  TodoLzt toDoM;
  bool end = false;
  
  cout << "ONTO THE PROGRAM..." << endl;
  cout << "= = = = = = = = = = = = \n" << endl;

  // Menu
  Menu menu;
  cout << "☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆" << endl;
  cout << " T O-D O ~ L I S T ~ P R O G R A M ~ M E N U" << endl;
  cout << "☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆" << endl;
  menu.makeMenu('+', "Add To-Do (Put a space after the + before entering task)");
  menu.makeMenu('?', "Display To-Do List (- 'id number')");
  menu.makeMenu('-', "Remove To-Do");
  menu.makeMenu('X', "Quit Program");
  menu.prtMenu();
  cout << "☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆ - ☆" << endl;
  cout << endl;

  toDoM.loadToDo("todo.txt");
  // Keep the program running until the user quits (is done adding tasks)
  do{
    cin >> toDoM;

    menu.header();
    menu.prtMenu();
  } while (!end);
  
  return 0;
}


void UnitTest(){
  // Create the objects
  Date date;
  TodoLzt todo;

  cout << "ComponentTest for each class" << endl;
  cout << "============================" << endl;

  // Call ComponentTest() for each class
  date.ComponentTest();
  todo.ComponentTest();

  for(int i = 0;i < 50; i++){
    cout << "=";
  }
  cout << endl;
}

// Program Greeting
void ProgramGreeting(){
  Date sstmDte;
  // Display the welcome to the program, name, and the due date of the assignment.
  cout << endl;
  cout << "= = = = = = = = = = = = = = =" << endl;
  cout << "Welcome to the TODO LIST Program." << endl;
  cout << "Made by: Samantha Siew" << endl;
  cout << "Date:" << sstmDte.getDate() << endl;
  cout << "= = = = = = = = = = = = = = =" << endl;
  cout << endl;
}