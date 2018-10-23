# Schwuring-Machine-Simulator
## Overview
This is a program to simulate the behavior of user-define Schwuring Machines. A Schwuring Machine behaves very much like a Turing Machine but with some constraints due to various pesky undecidable problems and the finite resources of our devices. First and foremost, a Schwuring Machine is not allowed to loop on any input, else the simulator would loop. Therefore a Schwuring Machine will time out after executing the millionth instruction on an input. Furthermore, the input tape to a Schwuring Machine is restricted to be less than or equal to one hundred symbols in length. Note that these two constraints effectively limit the time complexity of an input Schwuring Machine to strictly O(n<sup>3</sup>) for large input strings. Finally, a Schwuring Machine is restricted to at most fifty states(not including Q<sub>Start</sub>, Q<sub>Accept</sub>, and Q<sub>Reject</sub>) in the set of states and twenty-six symbols in the input alphabet. These constraints are defined as constants at the top of the simulator class in ```simulator.h``` and the simulator should agree with any modifications to them so long as they aren't done with the intent of breaking the program(such as setting the max input length to a negative number).

## Learning Goals

* Gain experience defining and implementing an original project from scratch.

* Understand how Turing Machines work by implementing Schwuring Machines in C++.

* Create an interesting, complex program and effectively communicate to other programmers how to use it properly.

## Project Specification
### Command Line Usage
The simulator accepts the following optional command line arguments:

1.```-h, --help```

- This causes the program to print a helpful message and then immediately terminate.

2.```-q, --quiet```

- This causes the program to run in quiet mode. In quiet mode, the program will run very similar to normal. However, the simulator will not print out information at each step of the Schwuring Machine's execution. Quiet mode exists so that we may simulate long programs with our Schwuring machines without producing an inordinate amount of output.

### User Commands
On startup and following the completion of every command the program will print ```% ``` as a prompt for the next command. Commands are specified by a command keyword followed by a required space and the command-specific arguments where applicable.

### CREATE - add a new Schwuring Machine to the simulator
Syntax: ```CREATE <machinename> <M> <N> <N[0]> <N[1]> ... <N[N - 1]>```

Creates a new Schwuring Machine named ```machinename``` with ```M``` states in addition to Q<sub>Start</sub>, Q<sub>Accept</sub>, and Q<<sub>Reject</sub> and ```N``` symbols in the input alphabet. Each N[i] specifies a single symbol in the input alphabet. Important caveat: ```#``` is a reserved symbol to represent the blank symbol in the tape alphabet and is therefore not allowed to be an element of the input alphabet.

Possible usage errors that the simulator will catch:

1. ```machinename``` names a Schwuring Machine that already exists in the simulator.
2. Duplicate defined symbols in the input alphabet.
3. Either N or M is negative.
4. ```N > MAX_STATES``` or ```M > MAX_ALPHABET```
5. ```#``` is an element of the input alphabet.

If the simulator catches any of the above errors, a helpful message will be printed and the command will be ignored.

### DEFINE - define the transition function for a Schwuring Machine in the simulator
Syntax: ```DEFINE <machinename>```

Defines or redefines the transition function for the Schwuring Machine named by ```machinenname```. First the simulator will print the set of states followed by the tape alphabet. Then for each combination of state number ```S``` and tape alphabet symbol ```C``` the simulator will print ```% S C ->``` and prompt a triple from the user of the form : ```<Q> <B> <D>``` to define the transition function on ```{S,C}``` where ```Q``` is the next state from the set of states [0,N-1] or {S, A, R} for Q<sub>Start</sub>, Q<sub>Accept</sub>, or Q<sub>Reject</sub> respectively. ```B``` is the symbol to write at the tape head from the tape alphabet, and ```D``` is the direction to move from the set {L,R}. If the given Schwuring Machine's transition function has already been defined, the simulator will warn the user before prompting redefinition.

Possible usage errors that the simulator will catch:

1. ```machinename``` does not name a Schwuring Machine in the simulator.
2. ```Q``` is outside the range of the set of states.
3. ```B``` is not a symbol in the tape alphabet.
4. ```D``` is not one of {L,R}.

If the simulator catches error 1, a helpful error message will be printed and the command will be ignored. If the simulator catches errors 2 through 4 a helpful error message will be printed and the simulator will re-prompt the triple.

### PRINT - print the definition for a Schwuring Machine in the simulator
Syntax: ```PRINT <machinename>```

Prints the input alphabet, states, and transition function definition of the Schwuring Machine named by ```machinename``` (will print 'undefined' if the machine's transition function has not been defined yet).

Possible usage errors that the simulator will catch:

1. ```machinename``` does not name a Schwuring Machine in the simulator.

If the simulator catches the above error, a helpful error message will be printed and the command will be ignored.

### RUN - run a Schwuring Machine on an input string
Syntax: ```RUN <machinename> <X> <Y>```

Simulates the behavior of the Schwuring Machine named by ```machinename``` on an input string where ```X``` is the number of symbols in the input string ```Y```. Any symbol past the X<sup>th</sup> symbol in ```Y``` will NOT be included in the input string and the input string will contain blank symbols from the end of the input to the end of the tape. If quiet mode is not enabled, the simulator will print the Schwuring Machine's state, tape head position, and what symbol is under the tape head for each step of its execution. Regardless of quietmode, if the Schwuring Machine halts the simulator will print whether it accepted or rejected ```Y```, the number of steps in the execution, and the position of the tape head and symbol under it when the Schwuring Machine halted. The simulator will then print the entire tape on a new line. If the simulator times out or goes off of either end of the tape during execution of ```machinename```, a message will be printed and execution of ```machinename``` on ```Y``` will be terminated.

Possible usage errors that the simulator will catch:

1. ```machinename``` does not name a Schwuring Machine in the simulator.
2. ```machinename``` names a Schwuring Machine that has not been defined.
3. Any symbol in ```Y``` is not in the tape alphabet of ```machinename```.
4. ```X``` is greater than the maximum input length allowed.

If the simulator catches any of the above errors, a helpful message will be printed and the command will be ignored.


### QUIT - quit the program
Syntax: ```QUIT```

Cleans up all internal data, prints a goodbye message and quits the program.

Possible usage errors that the simulator will catch: none

### XX - comment(no operation) useful for adding comments to command files
Syntax: ```XX ignore this entire line```

Any line beginning with an X is ignored. This command produces no output.

Possible usage errors that the simulator will catch: none

## Compiling and running the simulator
On Linux, download or clone the repository using ```git clone https://github.com/adhansen/Schwuring-Machine-Simulator.git```. The provided makefile can be used to create an executable named 'schwuring' which runs the simulator.