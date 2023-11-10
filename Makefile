CLUSTER_FILES = $(hashing.o clustering.o cube.o utils.o)

all: cube lsh cluster

cube: hashing.o cube.o hypercube.o utils.o
	g++ hashing.o cube.o hypercube.o utils.o -o $@ -g

lsh: hashing.o lsh.o utils.o
	g++ hashing.o lsh.o utils.o -o $@ -g

cluster: hashing.o clustering.o cluster.o hypercube.o utils.o
	g++ hashing.o clustering.o hypercube.o cluster.o utils.o -o $@ -g

clean:

hashing.o : hashing.cpp
	g++ -g -c hashing.cpp 

clustering.o: clustering.cpp
	g++ -g -c clustering.cpp

cluster.o: cluster.cpp
	g++ -g -c cluster.cpp
	
hypercube.o: hypercube.cpp
	g++ -g -c hypercube.cpp

utils.o: utils.cpp
	g++ -g -c utils.cpp

main.o : main.cpp
	g++ -g -c main.cpp

