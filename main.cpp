//Schwuring Machine Simulator project by Alexander Hansen

#include <iostream>
#include <unordered_map>
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

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////