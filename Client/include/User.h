//
// Created by lebeld@wincs.cs.bgu.ac.il on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H


#include <string>
#include <list>
#include <unordered_map>
#include <vector>
#include <atomic>
#include <mutex>
#include "Book.h"

using namespace std;
class User {
private:
    string name;
    unordered_map<string,list<Book*>> booksByGenre;
    unordered_map<string,string> idByGenre;
    unordered_map<string,string> receiptById;
    atomic<int> genreId;
    atomic<int> receiptId;
    vector<string> booksToBorrow;
    std::mutex booksByGenre_mutex;
    std::mutex idByGenre_mutex;
    std::mutex receiptById_mutex;
    std::mutex booksToBorrow_mutex;
public:
    User();
    ~User();
    int newGenreId();
    int newReceiptId();
    void addBook(string genre,string book);
    void removeBook(string,string);
    void addGenre(string,string);
    string getGenreId(string);
    string getName();
    void setName(string& name);
    bool isToBorrow(string name);
    void setBookToBorrow(string name);
    bool isBookAvailable(string genre,string name);
    Book* getBook(string genre, string name);
    string getRecipeText(string id);
    void addReceiptText(string id, string text);
    void borrowBook(string genre, string book,string owner);
    string status(string genre);
    void clear();
};


#endif //BOOST_ECHO_CLIENT_USER_H
