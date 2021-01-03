//
// Created by lebeld@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

#ifndef BOOST_ECHO_CLIENT_FRAME_H
#define BOOST_ECHO_CLIENT_FRAME_H


class Frame {
private:
    std::unordered_map<std::string,std::string>* headers;
    std::mutex headers_mutex;
public:
    Frame();
    ~Frame();
    Frame& operator=(const Frame&);
    Frame(const Frame&);
    std::string getHeader(const std::string&);

    std::unordered_map<std::string,std::string>* getHeaders();

    void addHeader(std::string, std::string);
};


#endif //BOOST_ECHO_CLIENT_FRAME_H
