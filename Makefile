CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lboost_thread -pthread

all: EchoClient
	g++ -o bin/BGSclient bin/EchoClient.o bin/ConnectionHandler.o bin/SessionData.o bin/Caster.o bin/ClientToServerMessage.o bin/ServerToClientMessage.o $(LDFLAGS)

EchoClient: bin/ConnectionHandler.o bin/ClientToServerMessage.o bin/ServerToClientMessage.o bin/EchoClient.o bin/SessionData.o bin/Caster.o

bin/EchoClient.o: src/EchoClient.cpp
	g++ $(CFLAGS) -o bin/EchoClient.o src/EchoClient.cpp

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/ClientToServerMessage.o: src/ClientToServerMessage.cpp
	g++ $(CFLAGS) -o bin/ClientToServerMessage.o src/ClientToServerMessage.cpp

bin/ServerToClientMessage.o: src/ServerToClientMessage.cpp
	g++ $(CFLAGS) -o bin/ServerToClientMessage.o src/ServerToClientMessage.cpp

bin/Caster.o: src/Caster.cpp
	g++ $(CFLAGS) -o bin/Caster.o src/Caster.cpp

bin/SessionData.o: src/SessionData.cpp
	g++ $(CFLAGS) -o bin/SessionData.o src/SessionData.cpp

.PHONY: clean
clean:
	rm -f bin/*