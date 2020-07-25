BIN := main
SOURCES := main.c
CC := gcc
CFLAGS := 
LDFLAGS := lpanel -lncurses
all:
	clear && $(CC) $(SOURCES) -o $(BIN) $(CFLAGS) -$(LDFLAGS)

clean:
	rm -rf $(BIN)