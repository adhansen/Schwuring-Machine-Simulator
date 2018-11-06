OBJS    = main.o simulator.o
SOURCE    = main.cpp simulator.cpp
HEADER    = 
OUT    = schwuring
CC     = g++
FLAGS     = -g -c -Wall -std=c++11
LFLAGS     = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

simulator.o: simulator.cpp
	$(CC) $(FLAGS) simulator.cpp 


clean:
	rm -f $(OBJS) $(OUT)