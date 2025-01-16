//Robots.cpp
//Samantha Siew, CISP 400
//12/14/2023

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib> 
#include <random>
#include <ctime>
using namespace std;

// Create a 10 x 10 map and generate batteries in it so that 40% of the squares are batteries
class Map {
public:
  int map[10][10];  // Create a 10 x 10 map

  // Constructor to initialize the map with batteries
  Map(){
    initializemap(map);
    genBats(map);
  }

  // Function to initialize the map with 0s
  void initializemap(int map[10][10]){
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        map[i][j] = 0;
      }
    }
  }

  // Genenerate 40 batteries in the map
  void genBats(int (&map)[10][10]) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);

    int batCount = 0;
    while (batCount < 40) {
      int batX = dis(gen);
      int batY = dis(gen);

      if (map[batX][batY] == 0) {
        map[batX][batY] = 5;
        batCount++;
      }
    }
  }

  // Display the map (Only for testing purposes)
  void displayMap(){
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        cout << "+---";
      }
      cout << "+" << endl;
      cout << "| ";
      for (int j = 0; j < 10; j++) {
        cout << map[i][j] << " | ";
      }
      cout << endl;
    }
  }
};

// Randomly generate a robot in the map
class Robot {
public:
  int x, y;  // Current position of the robot
  int energy;  // Energy level of the robot
  int turns;  // Number of moves made by the robot
  int batCount;  // Number of batteries collected by the robot
  int genes[16][5];  // Genes of the robot
  int sensor[4];  // Sensors of the robot
  Map map;  // Map of the environment


  // Constructor to initialize the robot
  Robot(){
    // Set the robot on somewhere in the map while avoiding starting on a battery
    do {
      x = rand() % 10;
      y = rand() % 10;
    } while (map.map[x][y] == 5); 

    energy = 5;  // Set starting energy to 5
    turns = 0;    // Keep track of turns
    batCount = 0;  // Keep track of batteries collected
    setGenes(genes);  // Set the 16 genes randomly
    updateSensor(map.map, x, y);    // Initialize sensor array
  }

  // Function to set the genes of the robot to be random
  void setGenes(int (&genes)[16][5]){
    int randDir;
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 4; j++) {
        // Set genes 1-4 to be either 0 or 1
        genes[i][j] = rand() % 2;
      }

      // Set the 5th gene to be either N, E, S, or W corresponding with 1, 2, 3, or 4 or 0 for random
      randDir = rand() % 5; // 0, 1, 2, 3, 4
      genes[i][4] = randDir;

      // Set genes 9 to be -1 -1 -1 -1 0
      if (i == 9 && randDir > 2) {
          genes[i][0] = -1;
          genes[i][1] = -1;
          genes[i][2] = -1;
          genes[i][3] = -1;
          genes[i][4] = 2;
      }
    }
  }

  // Update the sensor data of the robot
  void updateSensor(int (&map)[10][10], int x, int y){
    // Set the sensor data to be 1 if there is a wall, 5 if there is a battery, 0 for nothing, and -1 for don't care if anything is there
    // Sensor to the north
    this->sensor[0] = (x - 1 >= 0) ? ((map[x - 1][y] == 5) ? 5 : (map[x - 1][y] == 0) ? 0 : -1) : 1;
    // Sensor to the east
    this->sensor[1] = (y + 1 < 10) ? ((map[x][y + 1] == 5) ? 5 : (map[x][y + 1] == 0) ? 0 : -1) : 1;
    // Sensor to the south
    this->sensor[2] = (x + 1 < 10) ? ((map[x + 1][y] == 5) ? 5 : (map[x + 1][y] == 0) ? 0 : -1) : 1;
    // Sensor to the west
    this->sensor[3] = (y - 1 >= 0) ? ((map[x][y - 1] == 5) ? 5 : (map[x][y - 1] == 0) ? 0 : -1) : 1;
  }

  // If the robot goes on a square with a battery it adds 5 to its energy
  void collectBattery() {
    energy += 5;
    batCount++;
  }

  // If the robot runs out of energy, it dies
  bool isDead() {
    bool dead = false;
    if (energy == 0) {
      dead = true;
    }
    return dead;
  }

  // Decrease energy each turn and keep track of turns
  void decEnergyAndCntTurns() {
    if (energy > 0) {
      energy--;

      turns++;
    }
  }

  // Function to move the robot in a specific direction
  void moveDir(int direction) {
    // Should the direction be 0, randomize the direction
    if (direction == 0) {
      direction = rand() % 4 + 1;
    }

    // Regular directions
    switch (direction) {
      case 1:  // Move North
        if (x - 1 >= 0) {
          this->x--;
          updateSensor(map.map, this->x, this->y);
        }
        break;
      case 2:  // Move East
        if (y + 1 < 10) {
          this->y++;
          updateSensor(map.map, this->x, this->y);
        }
        break;
      case 3:  // Move South
        if (x + 1 < 10) {
          this->x++;
          updateSensor(map.map, this->x, this->y);
        }
        break;
      case 4:  // Move West
        if (y - 1 >= 0) {
          this->y--;
          updateSensor(map.map, this->x, this->y);
        }
        break;
      default:
        // Handle the case where the direction is not valid
        break;
    }
  }

  // Function to find the gene that matches the sensor data
  bool findMatchingGene() {
    // When scanning through genes a battery[5] and nothing[0] mean the same thing 
    for (int i = 0; i < 4; i++) {
      if (sensor[i] == 5) {
        sensor[i] = 0;
      }
    }

    for (int i = 0; i < 16; i++) {
      // Check if the gene matches the sensor data
      if ((genes[i][0] == sensor[0]) && (genes[i][1] == sensor[1]) && (genes[i][2] == sensor[2]) && (genes[i][3] == sensor[3])) {
        moveDir(genes[i][4]);

        return true;
      }
    }
    return false;
  }

  // Move the robot in the specified direction based on the gene and handle all the energy, battery, etc.
  void moveRobot() {
    // Get the current position of the robot
    int x = this->x;
    int y = this->y;

    // Get the current sensor data of the robot
    updateSensor(map.map, this->x, this->y);

    // Find the gene that matches the sensor data
    if (!findMatchingGene()) {
      // If no match is found, use the default from the 16th gene
      moveDir(genes[15][4]);
    }

    // Check if the robot is on a square with a battery
    if (map.map[x][y] == 5) {
      // Collect the battery
      collectBattery();
      // Remove the battery from the square
      map.map[x][y] = 0;
    }

    // Decrease energy and count turns
    decEnergyAndCntTurns();

  }

  // Function to display the map where the robot is located (Testing purposes only)
  void displayMapWR() {
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        if (i == x && j == y) {
          cout << "| R ";
        } else if (map.map[i][j] == 5) {
          cout << "| 5 ";
        } else {
          cout << "| " << map.map[i][j] << " ";
        }
      }
      cout << "|" << endl;

      for (int j = 0; j < 10; j++) {
        cout << "+---";
      }
      cout << "+" << endl;
    }
  }

  // Function to display the genes of the robot (Testing purposes only)
  void displayGenes(){
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 5; j++) {
        cout << genes[i][j] << " ";
      }
      cout << endl;
    }
    cout << "Energy level: " << energy << endl;
  }  

  // Function to display the robot's stats (Testing purposes only)
  void displayStats(){
    cout << "Turns: " << turns << endl;
    cout << "Batteries collected: " << batCount << endl;
    cout << "Energy level: " << energy << endl;
    cout << "Location: " << x << ", " << y << endl;
  }
};

// Populate 200 robots and manage the offspring and the overal evolution
class Population {
private:
  Robot robts[200];    // Store and generate 200 robots
  int generation;     // Track the current generation number  
  int totalBat;      // Total energy collected by all robots in generation

public:
  Population() {
    generation = 1;

    // Populate the first generation of robots
    for (int i = 0; i < 200; i++) {
        Robot robot;
        robts[i] = robot;
    }
  }

  // Run all robots for one generation
  void runRobots() {
    for (int i = 0; i < 200; i++) {
      do {
        robts[i].moveRobot();
      } while (!robts[i].isDead());
    }
  }

  // Sort robots based on the number of batteries collected
  void sortRobots() {
    totalBat = 0; // Reset total batteries

    // Sort robots by energy level
    for (int i = 0; i < 200; i++) {
      for (int j = i + 1; j < 200; j++) {
        if (robts[i].batCount < robts[j].batCount) {
          Robot temp = robts[i];
          robts[i] = robts[j];
          robts[j] = temp;
        }
      }
    }

    for (int i = 0; i < 200; i++) {
      totalBat += robts[i].batCount;
    }

    cout << "Total collected batteries: " << totalBat << endl;
  }

  // Breed robot children and manage the population
  void managePopulation() {
    sortRobots();

    // Create children and replace the lowest 50%
    for (int i = 0; i < 100; i += 2) {
      // Pair the top 2 robots and produce 2 children
      Robot child1 = createChild(robts[i], robts[i + 1]);
      Robot child2 = createChild(robts[i], robts[i + 1]);

      // Replace the lowest 2 robots with the new children
      robts[200 - 1 - i] = child1;
      robts[200 - 2 - i] = child2;
    }
  }

  // Evolve the population
  void evolve(int amtGen) {
    double avgFitness[amtGen];

    for (int gen = 0; gen < amtGen; gen++) {
      runRobots();
      managePopulation();

      // Calculate average fitness for the current generation
      avgFitness[gen] = (double)totalBat / 200;

      double perc = (avgFitness[gen]/40)*100.00;

      // Print or store information about the current generation (TESTING PURPOSES ONLY)
      cout << "Generation " << gen + 1 << " Average Fitness: " << avgFitness[gen] << "/40  ("<< perc << "%)" << endl;
      cout << endl;
    }

    cout << endl;

    //Print everything out
    cout << "☆--------------------------☆--------------------------☆" << endl;
    cout << "| Generations Evolved: " << amtGen << " |                          |" << endl;
    cout << "|--------------------------|--------------------------|" << endl;
    cout << "| Data:                    |                          |" << endl;

    // Display the first 3 generations
    for (int i = 0; i < min(3, amtGen); ++i) {
      cout << "|  Generation " << i + 1 << setw(33 - to_string(i + 1).length());
      cout << "|  Average Fitness: " << avgFitness[i] << setw(15 - to_string(avgFitness[i]).length()) << "|" << endl;
    }

    // Display the "..." row
    cout << "|           ...            |            ...           |" << endl;

    // Display the last 3 generations
    for (int i = max(0, amtGen - 3); i < amtGen; ++i) {
      cout << "|  Generation " << i + 1 << setw(33 - to_string(i + 1).length());
      cout << "|  Average Fitness: " << avgFitness[i] << setw(12 - to_string(avgFitness[i]).length()) << "|" << endl;
    }
    cout << "☆--------------------------☆--------------------------☆" << endl;
  }

  // Create a child robot by combining genes from two parents split 50/50
  Robot createChild(const Robot& parent1, const Robot& parent2) {
    Robot child;

    // Combine genes from both parents
    // First 8 genes from parent1 and the other 8 from parent2
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 5; j++) {
        child.genes[i][j] = parent1.genes[i][j];
      }
    }

    for (int i = 8; i < 16; i++) {
      for (int j = 0; j < 5; j++) {
        child.genes[i][j] = parent2.genes[i][j];
      }
    }

    // 5% chance of mutation with a random change in one of the genes
    if (rand() % 100 < 5) {
      // Mutation: Randomly change one gene
      int geneIndex = rand() % 16;
      int mutation = rand() % 5;
      child.genes[geneIndex][mutation] = rand() % 5;
    }

    return child;
  }

  // Function to get the current generation number
  int getGeneration() const {
    return generation;
  }

  // Function to access the robots in the population
  const Robot* getRobots() const {
    return robts;
  }
};

// Functs Prototypes
void ProgramGreeting();

int main() {
  // Functions
  ProgramGreeting();

  // Initialize robot population
  Population robotGen;  
  const int numGenerations = 100;    

  // Evolve the population for the specified number of generations and return the data
  robotGen.evolve(numGenerations);

  // TESTING PURPOSES ONLY
  // Robot robots;
  // while (robots.energy != 0) {
  //   cout << endl;
  //   cout << "--------------------" << endl;
  //   robots.displayMapWR();
  //   robots.moveRobot();
  //   robots.displayStats();
  //   cout << "--------------------" << endl;
  // }
  // TESTING PURPOSES ONLY

  return 0;
}


// Program Greeting
void ProgramGreeting(){
  // Display the welcome to the program, name, and the due date of the assignment.
  cout << endl;
  cout << "= = = = = = = = = = = = = = =" << endl;
  cout << "Welcome to ROBOT POPULATION." << endl;
  cout << "Made by: Samantha Siew" << endl;
  cout << "Date:" << "12/14/2023" << endl;
  cout << "= = = = = = = = = = = = = = =" << endl;
  cout << endl;
} 