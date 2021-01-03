//
// Created by lebeld@wincs.cs.bgu.ac.il on 12/01/2020.
//
#include <cstdlib>
#include "connectionHandler.h"
#include "InputReader.h"
#include <thread>
#include <StompEncoderDecoder.h>
#include <serverReader.h>
#include "Frame.h"
std::vector<std::string> split(std::string str, const std::string &delimiter);
int main (int argc, char *argv[]) {
    ConnectionHandler connectionHandler;
    std::string command;
    std::getline(cin,command);
    User* user = new User();
    while("bye"!=command){
        std::vector<std::string> words = split(command, " ");
        if(words[0]=="login" && words.size()==4){
            std::vector<std::string> host_port = split(words[1],":");
            std::string host = host_port[0];
            short port = stoi(host_port[1]);
            std::string userName = words[2];
            std::string password = words[3];
            ConnectionHandler connectionHandler(host, port);
            //User* user = new User();
            StompEncoderDecoder encdec(connectionHandler,user);
            Frame* f = encdec.toFrame(command);
//            if (!connectionHandler.connect()) {
//                std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
//                return 1;
//            }//maybe break?
            if (f != nullptr) {
                string toSend = encdec.encode(*f);
                command = connectionHandler.sendLine(toSend);
            }
            delete f;
            std::string line = "";
            if (!connectionHandler.getLine(line)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
            Frame* frame = encdec.decode(line);
            if(frame != nullptr) {
                string toSend = encdec.encode(*frame);
                connectionHandler.sendLine(toSend);
            }
            delete frame;
            if(connectionHandler.getIsConnected()) {
                InputReader inReader(&connectionHandler, &encdec, user);
                serverReader servReader(&connectionHandler, &encdec, user);
                std::thread th1(inReader);
                std::thread th2(servReader);
                th1.join();
                th2.join();
            }
        }
        std::getline(cin,command);
    }
    cout<<"closing program..."<<endl;
    connectionHandler.close();
    delete user;
    user= nullptr;
    return 0;
}
std::vector<std::string> split(std::string str, const std::string &delimiter) {
    vector<string> words;
    size_t position = 0;
    string token;
    while ((position = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, position);
        words.push_back(token);
        str.erase(0, position + delimiter.length());
    }
    words.push_back(str);
    return words;
}
