//
// Created by lebeld@wincs.cs.bgu.ac.il on 13/01/2020.
//

#include "serverReader.h"

serverReader::serverReader(ConnectionHandler *handler, StompEncoderDecoder *encdec, User *user) : handler(handler), encdec(encdec), user(user){}

void serverReader::operator()() {
    while(handler->getIsConnected()/*!encdec->getShouldTerminate()*/) {
        std::string line = "";
        if (!handler->getLine(line)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        Frame* frame = encdec->decode(line);
        if(frame != nullptr){
            string toSend = encdec->encode(*frame);
            handler->sendLine(toSend);

        }
        delete frame;
        frame= nullptr;

    }
}


