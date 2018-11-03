//"Schwuring Machine Simulator" project by Alexander Hansen
//https://github.com/adhansen/Schwuring-Machine-Simulator

#include <iostream>
#include <climits>
#include <string>
#include <getopt.h>
#include "simulator.h"

using namespace std;

static option long_options[] = {
	// LONG         ARGUMENT USED? (ignore) SHORT
	{ "help",		no_argument,   nullptr, 'h' },
	{ "quiet",		no_argument,   nullptr, 'q' },
	//End command line arguments
	{ nullptr,		0,             nullptr,  0 }
};

int main(int argc, char* argv[]) {
	//No mixing of printf() and cout in this program - speed up writing output
	std::ios::sync_with_stdio(false);
	bool quietMode = false;
	int choice = 0;
	while ((choice = getopt_long(argc, argv, "hq", long_options,
		nullptr)) != -1) {
		switch (choice) {
		case 'h':
			cout << "This is a program to simulate the behavior of user-defined Turing Machines\n"
					"For complete documentation and usage go to: \n"
					"https://github.com/adhansen/Schwuring-Machine-Simulator";
			return 0;
			break;
		case 'q':
			quietMode = true;
			break;
		}
	}	//While
	Simulator sim(quietMode);
	string command, machine;
	int numStates, numSymbols, size;
	while (true) {
		try {
			cout << "% ";
			cin >> command;
			//All commands start with a unique character
			if (command[0] == 'C') {
				cin >> machine >> numStates >> numSymbols;
				sim.addMachine(machine, numStates, numSymbols);
			}
			else if (command[0] == 'D') {
				cin >> machine;
				sim.defineMachine(machine);
			}
			else if (command[0] == 'P') {
				cin >> machine;
				//sim.printMachine(machine);
			}
			else if (command[0] == 'R') {
				cin >> machine >> size;
				sim.runMachine(machine, size);
			}
			else if (command[0] == 'Q') {
				cout << "Thanks for using the simulator!\n";
				return 0;
			}
			else if (command[0] == 'X') {
				cin.ignore(INT_MAX, '\n');
			}
			else if (command[0] == 'S') {
				cin >> machine;
				//sim.shredMachine(machine);
			}
			else {
				throw "Unrecognized command: " + command;
			}
		}
		catch(string error){
			cin.ignore(INT_MAX, '\n');
			cout << error << '\n';
		}
	}
	return 0;
}