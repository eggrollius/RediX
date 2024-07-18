server: server/server.h server/server.c
	gcc -o server.out server/server.c

client: client/client.h client/client.c
	gcc -o client.out client/client.c

runserver: server.out	
	./server.out

runclient: client.out
	./client.out

clean: 
	rm server.out