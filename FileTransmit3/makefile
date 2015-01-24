all: client server
client:
	gcc client.c transmit.c connect.c  -o client -lrt  -pthread
server:
	gcc server.c transmit.c connect.c  -o server -lrt -pthread

clean:
	rm client server recv

