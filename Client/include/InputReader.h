//
// Created by lebeld@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_INPUTREADER_H
#define BOOST_ECHO_CLIENT_INPUTREADER_H


#include <vector>
#include "connectionHandler.h"
#include "StompEncoderDecoder.h"

class InputReader {
private:
    ConnectionHandler *handler;
    StompEncoderDecoder *encdec;
    User* user;
    bool shouldCont;
public:
    InputReader(ConnectionHandler *handler,StompEncoderDecoder *encdec, User* user);

    //void readInput();
    void operator()();
};


#endif //BOOST_ECHO_CLIENT_INPUTREADER_H
