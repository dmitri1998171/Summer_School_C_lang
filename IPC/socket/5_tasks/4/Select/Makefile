all: client server
client:
	gcc TCPEchoClient.c DieWithError.c -o client
server:
	gcc TCPEchoServer-Select.c CreateTCPServerSocket.c AcceptTCPConnection.c DieWithError.c HandleTCPClient.c -lpthread -o server-select
clean:
	rm -f client server
