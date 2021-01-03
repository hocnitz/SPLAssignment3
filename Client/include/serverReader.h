//
// Created by lebeld@wincs.cs.bgu.ac.il on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_SERVERREADER_H
#define BOOST_ECHO_CLIENT_SERVERREADER_H


#include "StompEncoderDecoder.h"
#include "connectionHandler.h"
class serverReader {
private:
    ConnectionHandler* handler;
    StompEncoderDecoder* encdec;
    User* user;
public:
    serverReader(ConnectionHandler *handler,StompEncoderDecoder *encdec, User* user);
    void operator()();
};


#endif //BOOST_ECHO_CLIENT_SERVERREADER_H
