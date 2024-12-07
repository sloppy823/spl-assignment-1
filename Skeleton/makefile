all: clean compile run

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -o ./bin/simulation src/* -Iinclude
	
run:
	./bin/simulation config_file.txt

clean:
	rm -f ./bin/simulation