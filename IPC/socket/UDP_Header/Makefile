BIN_1 := server 
BIN_2 := client
SOURCES_1 := server.c
SOURCES_2 := client.c
CC := gcc
CFLAGS :=
LDFLAGS :=

$(BIN_1): $(SOURCES_1)
	clear && $(CC) $(SOURCES_1) -o $(BIN_1)
$(BIN_2): $(SOURCES_2)
	clear && $(CC) $(SOURCES_2) -o $(BIN_2)
clean:
	rm -rf $(BIN_1) $(BIN_2)