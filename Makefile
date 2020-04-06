CC=$(CROSS_COMPILE)g++
CFLAGS=-std=c++17 -Wall
OBJS := main.o globals.o memory.o bulkLineWrapper.o lineWrapper.o utility.o
LIBS=-lgpiod
all: main
main: $(OBJS)
		$(CC) $(LIBS) -o main $(CFLAGS) $(LDFLAGS) $(OBJS)
$(OBJS)	:	%.o : %.cpp
		$(CC) $(LIBS) -c $(CFLAGS) $< -o $@

.PHONY: clean all
clean:
	rm main $(OBJS)