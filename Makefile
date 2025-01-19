# Makefile

CXX = clang++
CXXFLAGS = -g -O0 -Werror
TARGET = main
SRCS = main.cpp octominx.cpp lookup_tables.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)