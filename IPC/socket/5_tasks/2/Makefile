all: client server
client:
	gcc TCPEchoClient.c DieWithError.c -o client
server:
	gcc TCPEchoServer-ForkN.c DieWithError.c CreateTCPServerSocket.c AcceptTCPConnection.c HandleTCPClient.c -lpthread -o server
	
clean:
	rm -rf client server
