#ifndef SIMULATOR
#define SIMULATOR

#include <string>
#include <unordered_map>

//Simulator class to simulate SMs - member functions are all responsible for error-checking
class Simulator {
private:
	//Arbitrary constraints that limit the Schwuring Machine, feel free to adjust to your needs
	static const int MAX_STATES = 50;
	static const int MAX_SYMBOLS = 26;
	static const int MAX_INPUT_LENGTH = 100;
	static const int MAX_INSTRUCTIONS = 1000000;
	
	bool quiet;
	char tape[MAX_INPUT_LENGTH];
	int inputSize;

	//Representation of a SM - gamma and transitionFunction allocated based on user input
	//NumStates is inclusive of S, A, R; gammaSize includes blank symbol '#'
	struct Schwuring {
		int numStates;
		int gammaSize;
		char *gamma = nullptr;
		struct Transition {
			int stateOut;
			char symbolOut;
			bool moveRight;
		};
		Transition *transitionFunction = nullptr;
		//Schwuring struct is responsible for cleaning up gamma and transitionFunction
		~Schwuring();
		//Binary search on gamma for query (error checking DEFINE, RUN) returns index of query or -1
		int findSymbol(char query) const;
	};
	//For constant-time lookup of Schwuring machines based on the machine's name
	std::unordered_map<std::string, Schwuring> machines;

public:
	//Nondefault ctor initializes tape to all blank symbols and inputSize to 0, specifies quietmode
	Simulator(bool is_quiet);

	//Adds a new machine to the simulator, reads num_symbols symbols from cin
	//Responsible for sorting gamma to allow binary search for symbols with RUN command
	void addMachine(std::string name_in, int num_states, int num_symbols);

	//Defines machine given by name_in according to input from cin
	void defineMachine(std::string name_in);

	//Prints information about machine given by name_in
	//void printMachine(std::string name_in) const;

	//Runs machine given by name_in, reads input string from cin
	void runMachine(std::string name_in, int input_size);

	//Removes the machine given by name_in from the simulator
	//void printMachine(std::string name_in) const;

};
#endif
