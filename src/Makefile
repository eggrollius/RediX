# Compiler
CXX = g++

# Flags
CXXFLAGS = -std=c++2a -Wall -Wextra

run: redix
	./redix

redix: operationtypes.o requesthandler.o clientconnection.o database.o server.o main.o
	$(CXX) $(CXXFLAGS) operationtypes.o requesthandler.o clientconnection.o database.o server.o main.o -o redix

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp -o server.o

database.o: database.cpp
	$(CXX) $(CXXFLAGS) -c database.cpp -o database.o

clientconnection.o: ClientConnection.cpp
	$(CXX) $(CXXFLAGS) -c ClientConnection.cpp -o clientconnection.o

requesthandler.o: RequestHandler.cpp
	$(CXX) $(CXXFLAGS) -c RequestHandler.cpp -o requesthandler.o

operationtypes.o: OperationTypes.cpp
	$(CXX) $(CXXFLAGS) -c OperationTypes.cpp -o operationtypes.o

clean:
	rm operationtypes.o requesthandler.o clientconnection.o database.o server.o main.o redix