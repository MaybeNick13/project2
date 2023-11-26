CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

SRCS = main.cpp GNSS.cpp lsh.cpp mrng.cpp utils.cpp hashing.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = main

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
