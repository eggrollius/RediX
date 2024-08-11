# Compiler
CXX = g++

# Flags
CXXFLAGS = -std=c++2a -Wall -Wextra -Ilibshared 

# Linker flags
LDFLAGS = 

# Debug	
DEBUG = -g

# Targets
all: server client

server: server/server.o libshared/libshared.o server/database.o
	$(CXX) $(CXXFLAGS) $(DEBUG) -o server.out server/server.o libshared/libshared.o server/database.o

client: client/client.o libshared/libshared.o
	$(CXX) $(CXXFLAGS) $(DEBUG) -o client.out client/client.o libshared/libshared.o

unittests: test/unit/DatabaseUnitTests.o libshared/libshared.o server/database.o
	$(CXX) $(CXXFLAGS) $(DEBUG) -o unittests.out test/unit/DatabaseUnitTests.o libshared/libshared.o server/database.o

runserver: server.out	
	./server.out

runclient: client.out
	./client.out

debugserver: server.out
	gdb ./server.out

rununittests: unittests.out	
	./unittests.out

clean:
	rm -f server.out client.out test.out server/server.o client/client.o libshared/libshared.o server/database.o

# Compilation rules
server/server.o: server/server.cpp server/server.h
	$(CXX) -c server/server.cpp -o server/server.o $(CXXFLAGS)

server/database.o: server/database.cpp server/database.h
	$(CXX) -c server/database.cpp -o server/database.o $(CXXFLAGS)

client/client.o: client/client.cpp client/client.h
	$(CXX) -c client/client.cpp -o client/client.o $(CXXFLAGS)

libshared/libshared.o: libshared/libshared.cpp
	$(CXX) -c libshared/libshared.cpp -o libshared/libshared.o $(CXXFLAGS)

test/unit/DatabaseUnitTests.o: test/unit/DatabaseUnitTests.cpp
	$(CXX) -c test/unit/DatabaseUnitTests.cpp -o test/unit/DatabaseUnitTests.o $(CXXFLAGS)
