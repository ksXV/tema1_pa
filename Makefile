CC = gcc
CFLAGS = -Wall -g -O1
TARGET = lanParty
all: final

test: clean final
	./$(TARGET) ./date/t$(TESTNUMBER)/c.in ./date/t$(TESTNUMBER)/d.in ./result$(TESTNUMBER).out

final: 
	$(CC) $(CFLAGS) -o $(TARGET) $(shell find . -name '*.c')
	chmod +x $(TARGET) 

clean: 
	rm $(TARGET) &> /dev/null
	rm -r out &> /dev/null
	rm result.out &> /dev/null

