//
// Created by lebeld@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include <string>
#include <iostream>
#include "Frame.h"
#include "InputReader.h"
#include "connectionHandler.h"

using namespace std;

InputReader::InputReader(ConnectionHandler *handler, StompEncoderDecoder *encdec, User* user): handler(handler), encdec(encdec), user(user), shouldCont(true){

}
void InputReader::operator()() {
    while (handler->getIsConnected()/*!encdec->getShouldTerminate()*/) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        Frame* f = encdec->toFrame(line);
        if (f != nullptr) {
            string toSend = encdec->encode(*f);
            shouldCont = handler->sendLine(toSend);
        }
        delete f;
        f= nullptr;
        if(line == "logout")
            break;
        if(!shouldCont)
            break;
    }
}


