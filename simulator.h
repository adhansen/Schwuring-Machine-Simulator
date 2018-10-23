#ifndef SIMULATOR
#define SIMULATOR

#include <string>
#include <unordered_map>
//use a sorted array for the input alphabet - efficient

//Simulator class to simulate SMs - member functions are all responsible for error-checking
class Simulator {
private:
	//Arbitrary constraints that limit the Schwuring Machine, feel free to adjust to your needs
	static const int MAX_STATES = 50;
	static const int MAX_ALPHABET = 26;
	static const int MAX_INPUT_LENGTH = 100;
	static const int MAX_INSTRUCTIONS = 1000000;
	
	char tape[MAX_INPUT_LENGTH];
	int inputSize;
	//Representation of a SM - gamma and transitionFunction allocated based on user input
	struct Schwuring {
		int numStates;
		char *gamma;
		struct Transition {
			int stateOut;
			char symbolOut;
			bool moveLeft;
		};
		Transition *transitionFunction;
	};
	//For constant-time lookup of Schwuring machines based on the machine's name
	std::unordered_map<std::string, Schwuring> machines;
public:
	//Default ctor initializes tape to all blank symbols and inputSize to 0
	Simulator();

	//Adds a new machine to the simulator, reads num_symbols symbols from cin
	void addMachine(string name_in, int num_states, int num_symbols);

	//Defines machine given by name_in according to input from cin
	void defineMachine(string name_in);

	//Prints information about machine given by name_in
	void printMachine(string name_in) const;

	//Runs machine given my name_in on input read from cin
	void runMachine(string name_in, int input_size);

	//Destructor cleans up gamma and transitionFunction
	~Simulator();
};
#endif
