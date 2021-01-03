//
// Created by lebeld@wincs.cs.bgu.ac.il on 13/01/2020.
//
#include <unordered_map>
#include <list>
#include <string>
#include <Book.h>
#include "User.h"
#include <iostream>
#include <mutex>

using namespace std;
User::User(): name(""),booksByGenre(unordered_map<string,list<Book*>>()),idByGenre(unordered_map<string,string>()),receiptById(unordered_map<string,string>()),genreId(0), receiptId(0),
        booksToBorrow(vector<string>()),booksByGenre_mutex(),idByGenre_mutex(), receiptById_mutex(), booksToBorrow_mutex(){}
int User::newGenreId() {
    return genreId.fetch_add(1);;
}

//adds book to list of genre while creating the genre list if it doesn't exist already
void User::addBook(string genre, string book) {
    Book* newBook = new Book(book, name);
    booksByGenre_mutex.lock();
    unordered_map<string,list<Book*>>::iterator got = booksByGenre.find(genre);
    if(got == booksByGenre.end()){
        list<Book*> l;
        l.push_back(newBook);
        booksByGenre.insert(make_pair(genre,l));
    } else {
        got->second.push_back(newBook);
    }
    booksByGenre_mutex.unlock();
}
void User::borrowBook(string genre, string book,string owner) {
    booksByGenre_mutex.lock();
    if(isToBorrow(book)) {
        Book *newBook = new Book(book, name);
        unordered_map<string, list<Book *>>::iterator got = booksByGenre.find(genre);
        if (got != booksByGenre.end()) {
            got->second.push_back(newBook);
        } else {
            list<Book*> l = list<Book*>();
            l.push_back(newBook);
            booksByGenre.insert(make_pair(genre, l));
        }
        booksByGenre_mutex.unlock();
        newBook->setOwner(owner);
        bool removed = false;
        booksToBorrow_mutex.lock();
        int  unsigned i;
        for(i=0; ( i<booksToBorrow.size() ) & (!removed) ; i++)
            if(booksToBorrow[i]==name) {
                booksToBorrow.erase(booksToBorrow.begin() + i);
                removed = true;
            }
    }
    booksToBorrow_mutex.unlock();

}

int User::newReceiptId() {
    return receiptId.fetch_add(1);
}

//removes if book exists, but if it doesn't it does nothing:
void User::removeBook(string genre, string book) {
    booksByGenre_mutex.lock();
    unordered_map<string,list<Book*>>::iterator got = booksByGenre.find(genre);
    if(got!=booksByGenre.end()){
        for (Book* b : got->second){
            if(b->getBookName()==book){
                delete b;
                got->second.remove(b);
                booksByGenre_mutex.unlock();
                break;
            }
        }
    }
    booksByGenre_mutex.unlock();
}

void User::addGenre(string genre, string id) {
    idByGenre.insert(make_pair(genre, id));
    booksByGenre_mutex.lock();
    unordered_map<string,list<Book*>>::iterator got = booksByGenre.find(genre);
    if(got == booksByGenre.end()){
        list<Book*> l;
        booksByGenre.insert(make_pair(genre,l));
    }
    booksByGenre_mutex.unlock();
}

string User::getGenreId(string genre) {
    idByGenre_mutex.lock();
    unordered_map<string,string>::iterator got = idByGenre.find(genre);
    if (got!=idByGenre.end()){
        idByGenre_mutex.unlock();
        return got->second;
    }
    idByGenre_mutex.unlock();
    return "-1";
}

string User::getName() {
    return name;
}


void User::setBookToBorrow(string name) {
    booksToBorrow_mutex.lock();
    booksToBorrow.push_back(name);
    booksToBorrow_mutex.unlock();
}

bool User::isBookAvailable(string genre, string name) {
    booksByGenre_mutex.lock();
    unordered_map<string,list<Book*>>::iterator got = booksByGenre.find(genre);
    bool ans = false;
    for(Book* book: got->second){
        if ((book->getBookName() == name) & (book->getAvailability()))
            ans = true;
    }
    booksByGenre_mutex.unlock();
    return ans;
}

Book *User::getBook(string genre, string name) {
    booksByGenre_mutex.lock();
    unordered_map<string,list<Book*>>::iterator got = booksByGenre.find(genre);
    Book* ans =nullptr;
    for(Book* book: got->second){
        if (book->getBookName() == name){
            ans = book;
            booksByGenre_mutex.unlock();
            return ans;
        }
    }
    booksByGenre_mutex.unlock();
    return ans;
}

void User::setName(string& newName){
    name=newName;
}

string User::getRecipeText(string id) {
    receiptById_mutex.lock();
    unordered_map<string,string>::iterator it = receiptById.find(id);
    if(it != receiptById.end()) {
        string ans = it->second;
        receiptById.erase(id);
        receiptById_mutex.unlock();
        return ans;
    }
    receiptById_mutex.unlock();
    return "";
}

void User::addReceiptText(string id, string text) {
    receiptById_mutex.lock();
    receiptById.insert(make_pair(id,text));
    receiptById_mutex.unlock();
}

string User::status(string genre) {
    booksByGenre_mutex.lock();
    unordered_map<string,list<Book*>>::iterator got = booksByGenre.find(genre);
    string output;
    list<Book*> l= got->second;
    if(got != booksByGenre.end()) {
        output = name + ":";
        for (Book *book: l) {
            if (book->getAvailability())
                output += book->getBookName() + ",";
        }
        if (output[output.size() - 1] == ',')
            output=output.substr(0, output.size() - 1);
    }else{
        output = "";
    }
    booksByGenre_mutex.unlock();
    return output;
}

void User::clear() {
    name="";
    genreId=0;
    receiptId=0;
    booksToBorrow.clear();
    idByGenre.clear();
    receiptById.clear();
    for(pair<string,list<Book*>> pair : booksByGenre){
        for(Book* b: pair.second){
            delete b;
            b= nullptr;
        }
    }
    booksByGenre.clear();
}

bool User::isToBorrow(string name) {
    booksToBorrow_mutex.lock();
    int unsigned i;
    for(i=0; i<booksToBorrow.size();i++){
        if (booksToBorrow[i]==name){
            booksToBorrow_mutex.unlock();
            return true;
        }
    }
    booksToBorrow_mutex.unlock();
    return false;
}

User::~User() {
    clear();
}
