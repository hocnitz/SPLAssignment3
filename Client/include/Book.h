//
// Created by lebeld@wincs.cs.bgu.ac.il on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H

#include <string>
#include <atomic>


using namespace std;

class Book {
private:
    string owner;
    string bookName;
    atomic<bool> isAvailable;
public:
    ~Book();
    bool getAvailability();
    void setAvailability(bool);
    Book(string,string);
    string getOwner();
    void setOwner(string);
    string getBookName();


};


#endif //BOOST_ECHO_CLIENT_BOOK_H
