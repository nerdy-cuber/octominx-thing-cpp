# Makefile

CXX = clang++
CXXFLAGS = -O2 -Werror
TARGET = main
SRCS = main.cpp octominx.cpp lookup_tables.cpp findWhiteFaceSolution.cpp intToMove.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)