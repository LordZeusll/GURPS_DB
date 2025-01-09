#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "pcg_random.hpp"
#include "tables.hpp"

using namespace std;

pcg_extras::seed_seq_from<std::random_device> gen;
pcg32 rng(gen);

struct Formulas {};

class Planet {};

class Star {
 public:
  string name;
  double mass;
  double luminosity;
  int temperature;
  string spectralType;
  string stage;
};

class System {
 public:
  int numberOfStars;
  vector<Star> stars;
  double age;
};

double findMass(int firstRoll, int secondRoll) {
  auto it = stellarMassTable.find(firstRoll);
  if (it != stellarMassTable.end()) {
    for (const auto &range : it->second) {
      if (secondRoll >= range.start && secondRoll <= range.end) {
        return range.mass;
      }
    }
  }
}

bool fileExists(string fileName) {
  ifstream file(fileName);
  file.close();
  return file.good();
}

int rollDice(int rolls = 1) {
  int total{};
  uniform_int_distribution<int> dis(1, 6);
  for (int i{}; i < rolls; ++i) {
    total += dis(gen);
  }
  return total;
}

void starMasses(System &system) {
  int firstRoll, secondRoll, thirdRoll;
  firstRoll = rollDice(3);
  secondRoll = rollDice(3);
  system.stars.push_back({"", findMass(firstRoll, secondRoll)});

  if (system.numberOfStars != 1) {
    for (int i{system.numberOfStars - 1}; i > 0; --i) {
      thirdRoll = rollDice() - 1;
      int firstRollTemp{firstRoll};
      if (thirdRoll == 0) {
        system.stars.push_back({"", system.stars[0].mass});
      } else {
        if (firstRoll < 6) {
          double highMass{system.stars[0].mass - 1.5};
          int positions{rollDice(thirdRoll + 1)};
          if (positions <= highMass * 10) {
            system.stars.push_back(
                {"", system.stars[0].mass - (positions * 0.1)});
          } else {
            double temp{system.stars[0].mass - highMass};
            positions -= static_cast<int>(highMass * 10);
            system.stars.push_back(
                {"", system.stars[0].mass - (temp + (positions * 0.05))});
          }
        } else {
          system.stars.push_back(
              {"", system.stars[0].mass - (rollDice(thirdRoll + 1) * 0.05)});
        }
      }
    }
  }
}

void systemAge(System &system) {
  int firstRoll, secondRoll, thirdRoll;
  firstRoll = rollDice(3);
  secondRoll = rollDice() - 1;
  thirdRoll = rollDice() - 1;
  auto it = stellarAgeTable[firstRoll];
  system.age = it.baseAge + (it.stepA * secondRoll) + (it.stepB * thirdRoll);
}

void stellarCharacteristics(System &system) {
  for (auto &star : system.stars) {
    auto evolutionEntry = evolutionTable[star.mass];
    star.spectralType = evolutionEntry.type;
    star.temperature = evolutionEntry.temp;
    if (star.mass < 0.45) {
      star.luminosity = evolutionEntry.lMin;
      star.stage = "main";
    } else if (star.mass <= evolutionEntry.mSpan) {
      star.luminosity = evolutionEntry.lMin +
                        abs((system.age / evolutionEntry.mSpan) *
                            (evolutionEntry.lMax - evolutionEntry.lMin));
      star.stage = "main";
    } else if (star.mass <= evolutionEntry.mSpan + evolutionEntry.sSpan) {
      star.luminosity = evolutionEntry.lMax;
      star.temperature =
          star.temperature -
          abs((system.age / evolutionEntry.sSpan) * (star.temperature - 4800));
      star.stage = "subgiant";
      // !WRITE AN ALGORITHM FOR FINDING APPROPRIATE SPECTRAL TYPE!
    } else if (star.mass <= evolutionEntry.mSpan + evolutionEntry.sSpan +
                                evolutionEntry.gSpan) {
      star.temperature = (rollDice(2) - 2) * 200 + 3000;
      star.luminosity = evolutionEntry.lMax * 25;
      star.stage = "giant";
    } else {
      star.mass = (rollDice(2) - 2) * 0.05 + 0.9;
      star.luminosity = 0.0005;
      star.stage = "dwarf";
    }
  }
}

void createSystem() {
  System system;
  int firstRoll;

  firstRoll = rollDice(3);
  system.numberOfStars = firstRoll < 11 ? 1 : firstRoll < 16 ? 2 : 3;

  starMasses(system);
  systemAge(system);
  stellarCharacteristics(system);

  // Step 17: Stellar Characteristics
}

void openDatabase(string dbName) {
  if (fileExists(dbName)) {
    cout << "Database found\n";
  } else {
    int choice{};
    while (choice != 1 && choice != 2) {
      cout << "Database was not found. Would you like to create a new "
              "one?\n1. Yes\n2. No\n";
      cin >> choice;
      if (choice != 1 && choice != 2) {
        cout << "Invalid choice\n";
      }
    }

    if (choice == 1) {
      ofstream file(dbName);
      file.close();
      cout << "Creating new database\n";
    } else {
      cout << "Exiting program\n";
      exit(0);
    }
  }
}

int main() {
  string dbName{"galaxy.db"};
  vector<System> systems;

  openDatabase(dbName);
}
