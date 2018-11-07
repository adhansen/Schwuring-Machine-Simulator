#include <climits>
#include <iostream>
#include <algorithm>
#include <numeric>
#include "simulator.h"

using namespace std;

const int Q_ACCEPT = -1;
const int Q_REJECT = -2;

//Schwuring struct is responsible for cleaning up gamma and transitionFunction
Simulator::Schwuring::~Schwuring() {
	delete[] gamma;
	gamma = nullptr;
	delete[] transitionFunction;
	transitionFunction = nullptr;
}

//Binary search on gamma for query (error checking DEFINE, RUN) returns index of query or -1 
int Simulator::Schwuring::findSymbol(char query) const{
	char *container = gamma;
	int size = gammaSize;
	while ((container + size) >= container) {
		int mid = size / 2;
		if (container[mid] == query) {
			return container + mid - gamma;
		}
		else if (container[mid] > query) {
			size = mid - 1;
		}
		else {
			container += mid + 1;
			size = mid;
		}
	}
	return -1;
}

//Default ctor initializes tape to all blank symbols and inputSize to 0
Simulator::Simulator(bool is_quiet) : quiet{ is_quiet }, inputSize{ 0 } {
	for (int i = 0; i < MAX_INPUT_LENGTH; ++i) {
		tape[i] = '#';
	}
}

//Adds a new machine to the simulator, reads num_symbols symbols from cin
//Responsible for sorting gamma to allow binary search for symbols with RUN command
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
	currentMachine->second.numStates = num_states + 1;
	currentMachine->second.gammaSize = num_symbols + 1;
	currentMachine->second.gamma = new char[num_symbols + 1];
	currentMachine->second.gamma[0] = '#';
	for (int i = 1; i < num_symbols + 1; ++i) {
		char next;
		cin >> next;
		if (next == '#') {
			machines.erase(currentMachine);
			throw "The blank symbol \'#\' cannot be a member of the input alphabet";
		}
		else {
			currentMachine->second.gamma[i] = next;
		}
	}
	//Sort gamma, faster symbol lookup when error-checking RUN and ensuring no dupes in this command
	std::sort(currentMachine->second.gamma, currentMachine->second.gamma + num_symbols + 1);
	for (int i = 0; i < num_symbols; ++i) {
		if (currentMachine->second.gamma[i] == currentMachine->second.gamma[i + 1]) {
			machines.erase(currentMachine);
			throw "Duplicate symbols are not allowed in the input alphabet";
		}
	}
	cout << "Created " << name_in << '\n';
}

//Defines machine given by name_in according to input from cin
void Simulator::defineMachine(string name_in) {
	char symbolWrite, direction, nextStateChar;
	int nextStateInt;
	unordered_map<string, Schwuring>::iterator currentMachine = machines.find(name_in);
	if (currentMachine == machines.end()) {
		throw '\"' + name_in + "\" does not name a machine in the simulator";
	}
	//allocate space for transition function & read from user input
	currentMachine->second.transitionFunction =
	new Simulator::Schwuring::Transition[currentMachine->second.numStates * currentMachine->second.gammaSize];
	//Init Qstart separately for less extra work for every other state
	for (int i = 0; i < currentMachine->second.gammaSize; ++i) {
		cout << "% S " << currentMachine->second.gamma[i] << " -> ";
		cin >> nextStateInt;
		//next state was in {S, A, R}, make nextStateInt usable
		if (!cin.good()) {
			cin.clear();
			cin >> nextStateChar;
			if (nextStateChar == 'S') {
				nextStateInt = 0;
			}
			else if (nextStateChar == 'A') {
				nextStateInt = Q_ACCEPT;
			}
			else if (nextStateChar == 'R') {
				nextStateInt = Q_REJECT;
			}
			else {
				cin.ignore(INT_MAX, '\n');
				cout << "Invalid state\n";
				--i;
				continue;
			}
		}
		else if ((nextStateInt < 0) || (nextStateInt >= currentMachine->second.numStates)) {
			cin.ignore(INT_MAX, '\n');
			cout << "Invalid state\n";
			--i;
			continue;
		}
		cin >> symbolWrite >> direction;
		if (!((direction == 'L') || (direction == 'R'))) {
			cin.ignore(INT_MAX, '\n');
			cout << "SM can only move left or right\n";
			--i;
			continue;
		}
		if (currentMachine->second.findSymbol(symbolWrite) == -1) {
			cin.ignore(INT_MAX, '\n');
			cout << "Symbol not in tape alphabet\n";
			--i;
			continue;
		}
		//Modify transitionfunction
		currentMachine->second.transitionFunction[i].stateOut = nextStateInt;
		currentMachine->second.transitionFunction[i].symbolOut = symbolWrite;
		currentMachine->second.transitionFunction[i].moveRight = (direction == 'R');
	}
	//Other states
	for (int i = 1; i < currentMachine->second.numStates; ++i) {		
		for (int j = 0; j < currentMachine->second.gammaSize; ++j) {
			cout << "% " << i << ' ' << currentMachine->second.gamma[j] << " -> ";
			cin >> nextStateInt;
			//next state was in {S, A, R}, make nextStateInt usable
			if (!cin.good()) {
				cin.clear();
				cin >> nextStateChar;
				if (nextStateChar == 'S') {
					nextStateInt = 0;
				}
				else if (nextStateChar == 'A') {
					nextStateInt = Q_ACCEPT;
				}
				else if (nextStateChar == 'R') {
					nextStateInt = Q_REJECT;
				}
				else {
					cin.ignore(INT_MAX, '\n');
					cout << "Invalid state\n";
					--j;
					continue;
				}
			}
			else if ((nextStateInt < 0) || (nextStateInt >= currentMachine->second.numStates)) {
				cin.ignore(INT_MAX, '\n');
				cout << "Invalid state\n";
				--j;
				continue;
			}
			cin >> symbolWrite >> direction;
			if (!((direction == 'L') || (direction == 'R'))) {
				cout << "SM can only move left or right\n";
				--j;
				continue;
			}
			if (currentMachine->second.findSymbol(symbolWrite) == -1) {
				cin.ignore(INT_MAX, '\n');
				cout << "Symbol not in tape alphabet\n";
				--j;
				continue;
			}
			//Modify transitionfunction
			Simulator::Schwuring::Transition &currTrans = 
			currentMachine->second.transitionFunction[(i * currentMachine->second.gammaSize) + j];
			currTrans.stateOut = nextStateInt;
			currTrans.symbolOut = symbolWrite;
			currTrans.moveRight = (direction == 'R');
		}
	}
}

//Prints information about machine given by name_in
void Simulator::printMachine(string name_in) const {
	unordered_map<string, Schwuring>::const_iterator currentMachine = machines.find(name_in);
	if (currentMachine == machines.end()) {
		throw '\"' + name_in + "\" does not name a machine in the simulator";
	}
	cout << "Name: " << currentMachine->first << '\n';
	cout << "States: S";
	if (currentMachine->second.numStates > 1) {
		cout << ", 1-" << currentMachine->second.numStates - 1;
	}
	cout << ", A, R\n";
	cout << "Tape alphabet: ";
	for (int i = 0; i < currentMachine->second.gammaSize; ++i) {
		cout << currentMachine->second.gamma[i] << ' ';
	}
	cout << "\nTransition function: \n";
	if (!currentMachine->second.transitionFunction) {
		cout << "undefined\n";
	}
	else {
		for (int i = 0; i < currentMachine->second.numStates; ++i) {
			for (int j = 0; j < currentMachine->second.gammaSize; ++j) {
				if (i == 0) {
					cout << "S(" << currentMachine->second.gamma[j] << ") = {";
				}
				else {
					cout << i << '(' << currentMachine->second.gamma[j] << ") = {";
				}
				if (currentMachine->second.transitionFunction[(i * currentMachine->second.gammaSize) + j].stateOut == 0) {
					cout << "S, ";
				}
				else if (currentMachine->second.transitionFunction[(i * currentMachine->second.gammaSize) + j].stateOut == Q_ACCEPT) {
					cout << "A, ";
				}
				else if (currentMachine->second.transitionFunction[(i * currentMachine->second.gammaSize) + j].stateOut == Q_REJECT) {
					cout << "R, ";
				}
				else {
					cout << currentMachine->second.transitionFunction[(i * currentMachine->second.gammaSize) + j].stateOut << ", ";
				}
				cout << currentMachine->second.transitionFunction[(i * currentMachine->second.gammaSize) + j].symbolOut << ", ";
				if (currentMachine->second.transitionFunction[(i * currentMachine->second.gammaSize) + j].moveRight) {
					cout << "R}\n";
				}
				else {
					cout << "L}\n";
				}
			}
		}
	}
}

//Runs machine given by name_in, reads input string from cin
void Simulator::runMachine(string name_in, int input_size) {
	unordered_map<string, Schwuring>::iterator currentMachine = machines.find(name_in);
	if (currentMachine == machines.end()) {
		throw '\"' + name_in + "\" does not name a machine in the simulator";
	}
	if (!currentMachine->second.transitionFunction) {
		throw '\"' + name_in + "\" has not been defined with DEFINE command";
	}
	if (input_size > MAX_INPUT_LENGTH) {
		throw "A Schwuring Machine cannot accept input > " + to_string(MAX_INPUT_LENGTH);
	}
	//Write input to tape and overwrite any symbols from prior input with '#'
	char tapeSym;
	int tapeSymIndex;
	for (int i = 0; i < input_size; ++i) {
		cin >> tapeSym;
		tapeSymIndex = currentMachine->second.findSymbol(tapeSym);
		if (tapeSymIndex == -1) {
			throw "Unrecognized symbol at position " + to_string(i);
		}
		else {
			tape[i] = tapeSym;
		}
	}
	for (int i = input_size; i < inputSize; ++i) {
		tape[i] = '#';
	}
	inputSize = input_size;
	//Run SM on tape
	int executed = 0;
	int tapeHead = 0;
	int currState = 0;
	while (executed <= MAX_INSTRUCTIONS) {
		//Check for tape head off the end of tape
		if ((tapeHead < 0) || (tapeHead > MAX_INPUT_LENGTH)) {
			throw "Tape head out of range on step " + to_string(executed);
		}
		//print info if not in quiet mode
		if (!quiet) {
			cout << "Instructions executed = " << executed << ", state = ";
			if (currState == 0) {
				cout << "Start";
			}
			else if (currState == Q_ACCEPT) {
				cout << "Accept";
			}
			else if (currState == Q_REJECT) {
				cout << "Reject";
			}
			else {
				cout << currState;
			}
			cout << ", tape head at position: " << tapeHead << ", symbol under tape head: "
				<< tape[tapeHead] << '\n';

		}
		//Check if in end state, else continue with execution
		if (currState < 0) {
			break;
		}
		//Execute Transition function
		tapeSymIndex = currentMachine->second.findSymbol(tape[tapeHead]);
		tape[tapeHead] = currentMachine->second.transitionFunction[currState * currentMachine->second.gammaSize + tapeSymIndex].symbolOut;
		if (currentMachine->second.transitionFunction[currState * currentMachine->second.gammaSize + tapeSymIndex].moveRight) {
			tapeHead++;
		}
		else {
			tapeHead--;
		}
		currState = currentMachine->second.transitionFunction[currState * currentMachine->second.gammaSize + tapeSymIndex].stateOut;
		++executed;
	}
	//Check if last step set state to accept or reject, else timeout
	if (currState == Q_ACCEPT) {
		cout << currentMachine->first << " accepted input after " << executed << " steps\n";
	}
	else if (currState == Q_REJECT) {
		cout << currentMachine->first << " rejected input after " << executed << " steps\n";
	}
	else {
		cout << currentMachine->first << "timed out!\n";
	}
}

//Removes the machine given by name_in from the simulator
void Simulator::shredMachine(string name_in) {
	unordered_map<string, Schwuring>::iterator currentMachine = machines.find(name_in);
	if (currentMachine == machines.end()) {
		throw '\"' + name_in + "\" does not name a machine in the simulator";
	}
	machines.erase(currentMachine);
	cout << "Erased " << name_in << " from simulator\n";
}