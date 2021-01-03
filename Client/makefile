CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

all: Client
	g++ -pthread -o bin/Client bin/connectionHandler.o bin/Client.o bin/InputReader.o bin/serverReader.o bin/StompEncoderDecoder.o bin/User.o bin/Book.o bin/Frame.o $(LDFLAGS) 

Client: bin/connectionHandler.o bin/Client.o bin/InputReader.o bin/serverReader.o bin/StompEncoderDecoder.o bin/User.o bin/Book.o bin/Frame.o
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ -pthread $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Client.o: src/Client.cpp
	g++ -pthread $(CFLAGS) -o bin/Client.o src/Client.cpp

bin/InputReader.o: src/InputReader.cpp
	g++ -pthread $(CFLAGS) -o bin/InputReader.o src/InputReader.cpp

bin/serverReader.o: src/serverReader.cpp
	g++ -pthread $(CFLAGS) -o bin/serverReader.o src/serverReader.cpp

bin/StompEncoderDecoder.o: src/StompEncoderDecoder.cpp
	g++ -pthread $(CFLAGS) -o bin/StompEncoderDecoder.o src/StompEncoderDecoder.cpp

bin/User.o: src/User.cpp
	g++ -pthread $(CFLAGS) -o bin/User.o src/User.cpp

bin/Book.o: src/Book.cpp
	g++ -pthread $(CFLAGS) -o bin/Book.o src/Book.cpp

bin/Frame.o: src/Frame.cpp
	g++ -pthread $(CFLAGS) -o bin/Frame.o src/Frame.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
