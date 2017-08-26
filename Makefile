CC = gcc
TARGET = timecodecalc
CCFLAGS = -Wall -lm

ifeq ($(OS),Windows_NT)
	CCFLAGS += -D WIN
	DELETE = del
else
	CCFLAGS += -D NIX
	DELETE = rm
endif

all:
	$(CC) -s $(TARGET).c -o $(TARGET) $(CCFLAGS)
debug:
	$(CC) -O0 -ggdb $(TARGET).c -o $(TARGET) $(CCFLAGS)
clean:
	$(DELETE) $(TARGET)
distclean: clean
