CXX = g++
CXXFLAGS = -Wall -Wextra
TARGET = program
OBJ = main.o reader.o ivent_handler.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET)