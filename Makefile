CXX=$(CROSS_COMPILE)g++
CXXFLAGS=-std=c++17 -Wall -Wextra 
OBJS := main.o globals.o memory.o bulkLineWrapper.o utility.o
LIBS=-lgpiod
memory-game: main
main: $(OBJS)
		$(CXX) $(LIBS) -o memory-game $(CXXFLAGS) $(LDFLAGS) $(OBJS)
$(OBJS)	:	%.o : %.cpp
		$(CXX) $(LIBS) -c $(CXXFLAGS) $< -o $@

.PHONY: clean all
clean:
	rm memory-game $(OBJS)