libshared: libshared/libshared.cpp
	g++ -c libshared/libshared.cpp -o libshared/libshared.o
	
server: server/server.h server/server.cpp libshared/libshared.o
	g++ -c server/server.cpp -o server/server.o -I libshared
	g++ -o server.out server/server.o libshared/libshared.o

client: client/client.h client/client.cpp
	g++ -o client/client.o -c client/client.cpp -I libshared
	g++ -o client.out client/client.o  libshared/libshared.o

test: test/test.cpp libshared/libshared.o client/client.o server/server.o
	g++ -o test.out test/test.cpp libshared/libshared.o client/client.o server/server.o -I libshared -I client -I server
	./test.out

runserver: server.out	
	./server.out

runclient: client.out
	./client.out

clean: 
	rm server.out client.out server/server.o client/client.o libshared/libshared.o