# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: cTrace

# Tool invocations
# Executable "cTrace" depends on the files bin/main.o - bin/Agent.o - bin/Graph.o - bin/Session.o - bin/Tree.o

cTrace: bin/main.o bin/Session.o bin/Agent.o bin/Graph.o bin/Tree.o

	@echo 'Building target: cTrace'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/cTrace bin/main.o bin/Agent.o bin/Graph.o bin/Session.o bin/Tree.o $(LFLAGS)
	@echo 'Finished building target: cTrace'
	@echo ' '


bin/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/main.o src/main.cpp


bin/Agent.o: src/Agent.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Agent.o src/Agent.cpp


bin/Graph.o: src/Graph.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Graph.o src/Graph.cpp


bin/Session.o: src/Session.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Session.o src/Session.cpp


bin/Tree.o: src/Tree.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Tree.o src/Tree.cpp

#Clean the build directory
clean:
	rm -f bin/*