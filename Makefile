CLUSTER_FILES = $(hashing.o clustering.o cube.o utils.o)
LSH_FILES = $(hashing.o lsh.o utils.o)

all: cube lsh cluster GNSS

cube: hashing.o cube.o hypercube.o utils.o
	g++ $^ -o $@ -g

lsh: $(LSH_FILES)
	g++ $^ -o $@ -g

cluster: hashing.o clustering.o hypercube.o cluster.o utils.o
	g++ $^ -o $@ -g

GNSS: hashing.o GNSS.o lsh.o utils.o
	g++ $^ -o $@ -g

clean:
	rm -f cube lsh cluster GNSS *.o

hashing.o : hashing.cpp
	g++ -g -c $<

clustering.o: clustering.cpp
	g++ -g -c $<

cluster.o: cluster.cpp
	g++ -g -c $<

hypercube.o: hypercube.cpp
	g++ -g -c $<

utils.o: utils.cpp
	g++ -g -c $<

main.o: main.cpp lsh.h utils.h
	g++ -g -c $<
