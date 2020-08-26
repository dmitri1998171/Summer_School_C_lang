server:
	clear && gcc server-fork.c -Wall CreateSockets.c -o server
client:
	clear && gcc -Wall client.c CreateSockets.c -o client && gcc -Wall client2.c CreateSockets.c -o client2
clean:
	rm -rf client server client2
