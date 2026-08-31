CXX = g++
CXXFLAGS = -std=c++11 -g --coverage -pthread -Wall

TARGET = eventflow

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

HDRS = $(wildcard *.h)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.gcda *.gcno
	