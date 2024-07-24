libshared: libshared/libshared.c
	gcc -c libshared/libshared.c -o libshared/libshared.o
	
server: server/server.h server/server.c libshared/libshared.o
	gcc -c server/server.c -o server/server.o -I libshared
	gcc -o server.out server/server.o libshared/libshared.o

client: client/client.h client/client.c
	gcc -o client/client.o -c client/client.c -I libshared
	gcc -o client.out client/client.o  libshared/libshared.o

test: test/test.c libshared/libshared.o client/client.o server/server.o
	gcc -o test.out test/test.c libshared/libshared.o client/client.o server/server.o -I libshared -I client -I server
	./test.out

runserver: server.out	
	./server.out

runclient: client.out
	./client.out

clean: 
	rm server.out client.out server/server.o client/client.o libshared/libshared.o