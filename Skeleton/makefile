all: clean compile 

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -o ./bin/simulation src/* -Iinclude
	
clean:
	rm -f ./bin/simulation

valgrind: compile
	valgrind --leak-check=full --show-leak-kinds=all ./bin/simulation config_file.txt