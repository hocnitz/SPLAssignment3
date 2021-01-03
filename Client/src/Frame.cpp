//
// Created by lebeld@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include <unordered_map>
#include <utility>
#include "Frame.h"

Frame::Frame(): headers(new std::unordered_map<std::string,std::string>()),headers_mutex() {}

void Frame::addHeader(std::string header, std::string content) {
    headers_mutex.lock();
    headers->insert(std::make_pair(header,content));
    headers_mutex.unlock();
    //headers->insert(header,content);
}

std::unordered_map<std::string,std::string>* Frame::getHeaders() {
    return headers;
}

std::string Frame::getHeader(const std::string& header) {
    headers_mutex.lock();
    std::unordered_map<std::string,std::string>::iterator got = headers->find(header);
    headers_mutex.unlock();
    return got->second;
}

Frame::~Frame() {
    headers->clear();
    delete headers;
    headers = nullptr;
}

Frame &Frame::operator=(const Frame & f) {
    return *this;
}

Frame::Frame(const Frame & f): headers(new std::unordered_map<std::string,std::string>()),headers_mutex() {}
