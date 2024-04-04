CC = gcc
CFLAGS = -Wall
TARGET = lanParty
all: final

final: 
	$(CC) $(CFLAGS) -o $(TARGET) $(shell find . -name '*.c')
	chmod +x $(TARGET) 

clean: 
	rm $(TARGET) 
	rm -r out

