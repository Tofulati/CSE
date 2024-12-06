all: chat-server

chat-server: chat-server.c
	gcc chat-server.c http-server.c -o chat-server

run: chat-server
	./chat-server 8000

clean:
	rm -f chat-server 
