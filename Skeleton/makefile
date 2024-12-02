all: clean main

main: Skeleton/bin/main.o Skeleton/bin/Action.o Skeleton/bin/Auxiliary.o Skeleton/bin/Facility.o Skeleton/bin/Plan.o Skeleton/bin/SelectionPolicy.o Skeleton/bin/Settelment.o Skeleton/bin/Simulation.o
	g++ -o Skeleton/bin/main Skeleton/bin/main.o Skeleton/bin/Action.o Skeleton/bin/Auxiliary.o Skeleton/bin/Facility.o Skeleton/bin/Plan.o Skeleton/bin/SelectionPolicy.o Skeleton/bin/Settelment.o Skeleton/bin/Simulation.o

Skeleton/bin/main.o: skeleton/src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -ISkeleton/include -o Skeleton/bin/main.o skeleton/src/main.cpp

Skeleton/bin/Action.o: skeleton/src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -ISkeleton/include -o Skeleton/bin/Action.o skeleton/src/Action.cpp

Skeleton/bin/Auxiliary.o: skeleton/src/Auxiliary.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -ISkeleton/include -o Skeleton/bin/Auxiliary.o skeleton/src/Auxiliary.cpp

Skeleton/bin/Facility.o: skeleton/src/Facility.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -ISkeleton/include -o Skeleton/bin/Facility.o skeleton/src/Facility.cpp

Skeleton/bin/Plan.o: skeleton/src/Plan.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -ISkeleton/include -o Skeleton/bin/Plan.o skeleton/src/Plan.cpp

Skeleton/bin/SelectionPolicy.o: skeleton/src/SelectionPolicy.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -ISkeleton/include -o Skeleton/bin/SelectionPolicy.o skeleton/src/SelectionPolicy.cpp

Skeleton/bin/Settelment.o: skeleton/src/Settelment.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -ISkeleton/include -o Skeleton/bin/Settelment.o skeleton/src/Settelment.cpp

Skeleton/bin/Simulation.o: skeleton/src/Simulation.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -ISkeleton/include -o Skeleton/bin/Simulation.o skeleton/src/Simulation.cpp

clean:
	rm -f Skeleton/bin/*

valgrind:
	valgrind --leak-check=full --show-reachable=yes Skeleton/bin/main
