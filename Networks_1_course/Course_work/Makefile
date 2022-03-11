all:
	clear && gcc server-fork.c -Wall CreateSockets.c -o server-fork && gcc -Wall client.c CreateSockets.c -o client && gcc -Wall client2.c CreateSockets.c -o client2
clean:
	rm -rf server client server-fork client2
