# SPL Assignment 3

Implementation of both a server, which provides STOMP server services, and a client, which a user can use
in order to interact with the rest of the users. The server is implemented in Java and supports both
Thread-Per-Client (TPS) and the Reactor, choosing which one according to arguments given on startup. The
client is in C++ , and holds the required logic as described in the assignment3.pdf file.
All communication between the clients and the server are according to the STOMP `Simple-Text-Oriented-
Messaging-Protocol' protocol.
