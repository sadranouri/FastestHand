CXX = g++
CXXFLAGS = -std=c++20 -Wall

TARGET = UTGame

OBJS = main.o fastestHand.o players.o admins.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)