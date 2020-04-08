CC=$(CROSS_COMPILE)g++
CFLAGS=-std=c++17 -Wall
OBJS := main.o globals.o memory.o bulkLineWrapper.o utility.o
LIBS=-lgpiod
memory-game: main
main: $(OBJS)
		$(CC) $(LIBS) -o memory-game $(CFLAGS) $(LDFLAGS) $(OBJS)
$(OBJS)	:	%.o : %.cpp
		$(CC) $(LIBS) -c $(CFLAGS) $< -o $@

.PHONY: clean all
clean:
	rm main $(OBJS)