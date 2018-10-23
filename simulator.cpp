#include <climits>
#include <iostream>
#include "simulator.h"

using namespace std;

//Default ctor initializes tape to all blank symbols and inputSize to 0
Simulator::Simulator() : inputSize{ 0 } {
	for (int i = 0; i < MAX_INPUT_LENGTH; ++i)
		tape[i] = '#';
}