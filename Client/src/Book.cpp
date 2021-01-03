//
// Created by lebeld@wincs.cs.bgu.ac.il on 13/01/2020.
//

#include "Book.h"
#include <utility>

string Book::getBookName() {
    return bookName;
}

string Book::getOwner() {
    return owner;
}

Book::Book(string book, string userName) : owner(std::move(userName)), bookName(std::move(book)), isAvailable(true){}

void Book::setOwner(string o) {
    owner = o;
}

void Book::setAvailability(bool b) {
    isAvailable=b;
}

bool Book::getAvailability() {
    return isAvailable.load();
}

Book::~Book() {}

