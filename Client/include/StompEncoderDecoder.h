//
// Created by lebeld@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H
#define BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H


#include "Frame.h"
#include "User.h"
#include "connectionHandler.h"

class StompEncoderDecoder {

private:
    User* user;
    ConnectionHandler& handler;
    atomic<bool> shouldTerminate;
public:
    StompEncoderDecoder(ConnectionHandler& handler, User* user);
    std::string  encode(Frame&);
    Frame* decode(std::string&);
    Frame* toFrame(string line);
    vector<std::string> split(string str, const string &delimiter);
    bool getShouldTerminate();
};


#endif //BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H
