#include <climits>
#include <iostream>
#include <algorithm>
#include "simulator.h"

using namespace std;

//Schwuring struct is responsible for cleaning up gamma and transitionFunction
Simulator::Schwuring::~Schwuring() {
	delete[] gamma;
	delete[] transitionFunction;
}

//Default ctor initializes tape to all blank symbols and inputSize to 0
Simulator::Simulator(bool is_quiet) : inputSize{ 0 }, quiet{ is_quiet } {
	for (int i = 0; i < MAX_INPUT_LENGTH; ++i) {
		tape[i] = '#';
	}
}

//Adds a new machine to the simulator, reads num_symbols symbols from cin
void Simulator::addMachine(string name_in, int num_states, int num_symbols) {
	unordered_map<string, Schwuring>::iterator currentMachine = machines.find(name_in);
	if (currentMachine != machines.end()) {
		throw name_in + " already exists!";
	}
	if ((num_states > MAX_STATES) || (num_states < 0)) {
		throw "Number of states out of range";
	}
	if ((num_symbols > MAX_SYMBOLS) || (num_symbols < 0)) {
		throw "Number of symbols out of range";
	}
	//Initialize SM members(not transition function), save iterator to avoid redundant hashing
	currentMachine = machines.emplace(name_in, Schwuring()).first;
	currentMachine->second.numStates = num_states + 3;
	currentMachine->second.gammaSize = num_symbols + 1;
	//Reference to gamma for readability and less dereferencing of currentMachine iterator
	char *gammaRef = currentMachine->second.gamma;
	gammaRef = new char[num_symbols + 1];
	gammaRef[0] = '#';
	for (int i = 1; i < num_symbols + 1; ++i) {
		char next;
		cin >> next;
		if (next == '#') {
			throw "The blank symbol \'#\' cannot be a member of the input alphabet";
			machines.erase(currentMachine);
			return;
		}
		else {
			gammaRef[i] = next;
		}
	}
	//Sort gamma->faster symbol lookup when error-checking RUN and ensuring no dupes in this command
	std::sort(gammaRef, gammaRef + num_symbols + 1);
	for (int i = 0; i < num_symbols; ++i) {
		if (gammaRef[i] == gammaRef[i + 1]) {
			throw "Duplicate symbols are not allowed in the input alphabet";
			machines.erase(currentMachine);
			return;
		}
	}
	return;
}

//Defines machine given by name_in according to input from cin
void Simulator::defineMachine(std::string name_in) {
	unordered_map<string, Schwuring>::iterator currentMachine = machines.find(name_in);
	if (currentMachine == machines.end()) {
		throw name_in + " does not name a machine in the simulator";
	}
	for (int i = 0; i < currentMachine->second.numStates; ++i) {
		for (int j = 0; j < currentMachine->second.gammaSize; ++j) {

		}
	}
}