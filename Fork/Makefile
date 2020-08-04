BIN := 1
BIN_2 := 2
SOURCES := 1.c 
SOURCES_2 := 2.c
HEADERS := 
CC :=gcc
CFLAGS :=
LDFLAGS :=

$(BIN): $(SOURCES)
		clear && $(CC) $(SOURCES) -o $(BIN) $(CFLAGS) $(LDFLAGS)
$(BIN_2): $(SOURCES_2)
		clear && $(CC) $(SOURCES_2) -o $(BIN_2) $(CFLAGS) $(LDFLAGS)
clean:
		rm -rf $(BIN) $(BIN_2)