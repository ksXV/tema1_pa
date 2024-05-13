CC = gcc
CFLAGS = -Wall -g -fsanitize=address -O1
TARGET = lanParty
all: final

test: clean final
	./$(TARGET) ./date/t8/c.in ./date/t8/d.in ./result.out

final: 
	$(CC) $(CFLAGS) -o $(TARGET) $(shell find . -name '*.c')
	chmod +x $(TARGET) 

clean: 
	rm $(TARGET) &> /dev/null
	rm -r out &> /dev/null
	rm result.out &> /dev/null

