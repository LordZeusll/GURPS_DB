#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <conio.h>
#include <cmath>
#include <string>
#include <algorithm>
#include <random>
#include "tables.h"

using namespace std;
using json = nlohmann::json;

struct Formulas {
	
};

class Planet {

};

class Star {
public:
	string name;
	double mass;
	double age;
};

class System {
public:
	int numberOfStars;
	vector<Star> stars;
};

double findMass(int firstRoll, int secondRoll) {
	auto it = stellarMassTable.find(firstRoll);
	if (it != stellarMassTable.end()) {
		for (const auto& range : it->second) {
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

int rollDice() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 6);
	return dis(gen);
}

int rollDice(int rolls) {
	int total{};
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 6);
	for (int i{}; i < rolls; ++i) {
		total += dis(gen);
	}
	return total;
}

void starMasses(System& system) {
	int firstRoll, secondRoll, thirdRoll;
	firstRoll = rollDice(3);
	secondRoll = rollDice(3);
	system.stars.push_back({ "", findMass(firstRoll, secondRoll) });

	if (system.numberOfStars != 1) {
		for (int i{ system.numberOfStars - 1 }; i > 0; --i) {
			thirdRoll = rollDice() - 1;
			int firstRollTemp{ firstRoll };
			if (thirdRoll == 0) {
				system.stars.push_back({ "", system.stars[0].mass });
			}
			else {
				if (firstRoll < 6) {
					double highMass{ system.stars[0].mass - 1.5 };
					int positions{ rollDice(thirdRoll + 1) };
					if (positions <= highMass * 10) {
						system.stars.push_back({ "",system.stars[0].mass - (positions * 0.1) });
					}
					else {
						double temp{ system.stars[0].mass - highMass };
						positions -= static_cast<int>(highMass * 10);
						system.stars.push_back({ "",system.stars[0].mass - (temp + (positions * 0.05)) });
					}
				}
				else {
					system.stars.push_back({ "",system.stars[0].mass - (rollDice(thirdRoll + 1) * 0.05) });
				}
			}
		}
	}
}

void starAge(System& system) {
	int firstRoll, secondRoll, thirdRoll;
	for (int i{}; i < system.numberOfStars; ++i) {
		firstRoll = rollDice(3);
		secondRoll = rollDice() - 1;
		thirdRoll = rollDice() - 1;
		auto it = stellarAgeTable[firstRoll];
		system.stars[i].age = it.baseAge + (it.stepA * secondRoll) + (it.stepB * thirdRoll);
	}
	
}

void createSystem() {
	System system;
	int firstRoll;

	firstRoll = rollDice(3);
	system.numberOfStars = firstRoll < 11 ? 1 : firstRoll < 16 ? 2 : 3;

	starMasses(system);
	starAge(system);

	// Step 17: Stellar Characteristics
}

void openDatabase(string dbName) {
	if (fileExists(dbName)) {
		cout << "Database found\n";

	}
	else {
		int choice{};
		while (choice != 1 && choice != 2) {
			cout << "Database was not found. Would you like to create a new one?\n1. Yes\n2. No\n";
			choice = _getch();
			choice -= '0';
			if (choice != 1 && choice != 2) {
				cout << "Invalid choice\n";
			}
		}

		if (choice == 1) {
			ofstream file(dbName);
			file.close();
			cout << "Creating new database\n";
		}
		else {
			cout << "Exiting program\n";
			exit(0);
		}
	}
}

int main() {
	string dbName { "galaxy.json" };
	vector<System> systems;

	openDatabase(dbName);


	
}
