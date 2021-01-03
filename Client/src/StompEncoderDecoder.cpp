//
// Created by lebeld@wincs.cs.bgu.ac.il on 12/01/2020.
//
#include <Book.h>
#include <unordered_map>
#include <iostream>
#include "StompEncoderDecoder.h"
using namespace std;
//
StompEncoderDecoder::StompEncoderDecoder(ConnectionHandler &handler,User* user) : user(user), handler(handler), shouldTerminate(false){}

std::string StompEncoderDecoder::encode(Frame &frame) {
    string output=frame.getHeader("type")+"\n";
    string body="";
    unordered_map<string,string>::iterator it = frame.getHeaders()->begin();
    while(it != frame.getHeaders()->end()){
        if ((it->first!="body") & (it->first!="type")){
            output=output+it->first+":"+it->second+"\n";
        } else if (it->first=="body"){
            body=it->second;
        }
        it.operator++();
    }
    output=output+"\n"+body+"\n"+"\0";
    return output;
}



Frame *StompEncoderDecoder::decode(std::string &line) {
    Frame *f = nullptr;
    std::vector<std::string> lines = split(line, "\n");
    if (lines[0] == "CONNECTED") {
        std::cout << "login successful" << endl;
        delete f;
        return nullptr;
    } else if (lines[0] == "RECEIPT") {
        std::vector<string> receipt = split(lines[1], ":");
        string msg = user->getRecipeText(receipt[1]);
        if (msg!="") {
            cout << msg << endl;//prints the receipt
        }
        if (msg == "disconnecting...") {

            handler.close();
            user->clear();
            cout<<"socket closed, type bye to exit or login to continue"<<endl;
        }
        delete f;
        return nullptr;
    } else if (lines[0] == "ERROR") {
        cout<<line<<endl;//prints the error frame
        handler.close();
        user->clear();
        cout<<"socket closed, type bye to exit or login to continue"<<endl;
        delete f;
        return nullptr;
    } else if (lines[0] == "MESSAGE") {
        string genre = split(lines[2], ":")[1];
        string body = lines[lines.size()-1];

        cout<< body<<endl;//prints the body
        std::vector<std::string> bodyWords = split(body, " ");
        int bodysize = bodyWords.size();
        if ((bodysize >=5) && (bodyWords[1] == "wish")) {//"X wish to borrow Y"
            string whoWants = bodyWords[0];
            string bookName = bodyWords[4];
            int unsigned i;
            for (i = 5; i < bodyWords.size(); i++) {
                bookName += " " + bodyWords[i];
            }
            if (user->isBookAvailable(genre, bookName)) {
                f = new Frame();
                f->addHeader("type", "SEND");
                f->addHeader("destination", genre);
                f->addHeader("body", user->getName() + " has " + bookName);
            }else{
                delete f;
                return nullptr;
            }
        } else if ((bodysize >=3) && (bodyWords[1] == "has") && (bodyWords[2] != "added")) {//"X has Y"
            string whoHas = bodyWords[0];
            string bookName = bodyWords[2];
            int unsigned i;
            for (i = 3; i < bodyWords.size(); i++) {
                bookName += " " + bodyWords[i];
            }
            if (user->isToBorrow(bookName)) {
                f = new Frame();
                f->addHeader("type", "SEND");
                f->addHeader("destination", genre);
                f->addHeader("body", "Taking " + bookName + " from " + whoHas);
                user->borrowBook(genre, bookName, whoHas);

            }else{
                delete f;
                return nullptr;
            }
        } else if ((bodysize >=4) && (bodyWords[0] == "Taking")) {//"Taking {bookname} from {whoHas}"
            string whoHas = bodyWords[bodyWords.size() - 1];
            string bookName = bodyWords[1];
            int unsigned i;
            for (i = 2; i < bodyWords.size()-2; i++) {
                bookName += " " + bodyWords[i];
            }
            if (user->getName() == whoHas) {
                Book *book = user->getBook(genre, bookName);
                book->setAvailability(false);
            }
            delete f;
            return nullptr;
        } else if (bodysize >=4 && bodyWords[0] == "Returning") {//""Returning {bookname} to {whoHas}""
            string whoHas = bodyWords[bodyWords.size() - 1];
            string bookName = bodyWords[1];
            int unsigned i;
            for (i = 2; i < bodyWords.size()-2; i++) {
                bookName += " " + bodyWords[i];
            }
            if (user->getName() == whoHas) {
                Book *book = user->getBook(genre, bookName);
                book->setAvailability(true);
            }
            delete f;
            return nullptr;
        }else if (bodysize >=2 && ((bodyWords[0] == "book") | (bodyWords[0] == "Book"))) {//"book status"
            string status = user->status(genre);
            f = new Frame();
            f->addHeader("type", "SEND");
            f->addHeader("destination", genre);
            f->addHeader("body", status);
        }
    }
    return f;
}

Frame *StompEncoderDecoder::toFrame(string line) {
    std::vector<std::string> words = split(line, " ");
    //Frame *f = new Frame();
    Frame *f = nullptr;
    if (!handler.getIsConnected()) {
    if (words[0] == "login") { // "login {host} {username} {password}"
        f = new Frame();
        if (!handler.getIsConnected()) {
            if (!handler.connect()) {
                std::cerr << "Cannot connect! " << std::endl;
                delete f;
                return nullptr;
            }
        }
        f->addHeader("type", "CONNECT");
        f->addHeader("accept-version", "1.2");
        f->addHeader("host", words[1]);
        f->addHeader("login", words[2]);
        f->addHeader("passcode", words[3]);
        user->setName(words[2]);
    }
    } else if (handler.getIsConnected()) {//continue only if logged in
        if ((words[0] == "join") & (words.size() > 1)) {// "join {club name}"
            f = new Frame();
            f->addHeader("type", "SUBSCRIBE");
            f->addHeader("destination", words[1]);
            string genreId = to_string(user->newGenreId());
            f->addHeader("id", genreId);
            string tempid = to_string(user->newReceiptId());
            user->addReceiptText(tempid, "Joined club " + words[1]);
            f->addHeader("receipt", tempid);
            user->addGenre(words[1], genreId);
        } else if ((words[0] == "exit") & (words.size() > 1)) {// "exit {club name}"
            if (user->getGenreId(words[1]) == "-1") {//if user is not in club
                cout << "user is not subscribed to " + words[1] << endl;
                delete f;
                return nullptr;
            } else {
                f = new Frame();
                f->addHeader("type", "UNSUBSCRIBE");
                f->addHeader("destination", words[1]);
                f->addHeader("id", user->getGenreId(words[1]));//need to send id that matches destination
                string tempid = to_string(user->newReceiptId());
                user->addReceiptText(tempid, "Exited club " + words[1]);
                f->addHeader("receipt", tempid);
            }
        } else if ((words[0] == "add") & (words.size() > 2)) {// "add {club name} {book name}"
            f = new Frame();
            f->addHeader("type", "SEND");
            f->addHeader("destination", words[1]);
            string bookName = words[2];
            int unsigned i;
            for (i = 3; i < words.size(); i++)
                bookName += " " + words[i];
            string msg = user->getName() + " has added the book " + bookName;
            user->addBook(words[1], bookName);
            f->addHeader("body", msg);
            //f->addHeader("receipt", to_string(user->newReceiptId()));
        } else if ((words[0] == "borrow") & (words.size() > 2)) {// "borrow {club name} {book name}"
            f = new Frame();
            f->addHeader("type", "SEND");
            f->addHeader("destination", words[1]);
            string bookName = words[2];
            int unsigned i;
            for (i = 3; i < words.size(); i++)
                bookName += " " + words[i];
            string msg = user->getName() + " wish to borrow " + bookName;
            user->setBookToBorrow(bookName);
            f->addHeader("body", msg);
            f->addHeader("id", user->getGenreId(words[1]));
            string tempid = to_string(user->newReceiptId());
            user->addReceiptText(tempid, "");
            f->addHeader("receipt", tempid);
        } else if ((words[0] == "return") & (words.size() > 2)) {// "return {club name} {book name}"
            f = new Frame();
            std::string bookName = words[2];
            int unsigned i;
            for (i = 3; i < (words.size() - 2); i++)
                bookName += " " + words[i];
            if (user->isBookAvailable(words[1], bookName)) {
                Book *book = user->getBook(words[1], bookName);
                f->addHeader("type", "SEND");
                f->addHeader("destination", words[1]);
                std::string msg = "Returning " + bookName + " to " + book->getOwner();
                f->addHeader("body", msg);
                f->addHeader("id", user->getGenreId(words[1]));
                string tempid = to_string(user->newReceiptId());
                user->addReceiptText(tempid, "");
                f->addHeader("receipt", tempid);
                user->removeBook(words[1], bookName);
            }
        } else if ((words[0] == "status") & (words.size() > 1)) {// "status {club name}"
            f = new Frame();
            f->addHeader("type", "SEND");
            f->addHeader("destination", words[1]);
            std::string msg = "book status";
            f->addHeader("body", msg);
        } else if ((words[0] == "logout") & (words.size() == 1)) {
            f = new Frame();
            f->addHeader("type", "DISCONNECT");
            string tempid = to_string(user->newReceiptId());
            user->addReceiptText(tempid, "disconnecting...");
            f->addHeader("receipt", tempid);
        } else {//if input is non of the above, then input is invalid
            cout << "invalid input" << endl;
            delete f;
            return nullptr;
        }
    }else if (words[0]=="bye"){
//        cout<<"closing program..."<<endl;
//        if(handler.getIsConnected())
//            handler.close();
//        shouldTerminate=true;
//        delete f;
//        return nullptr;
    }else{

        cout<<"not connected, login first"<<endl;
    }
    return f;
}


std::vector<std::string> StompEncoderDecoder::split(std::string str, const std::string &delimiter) {
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

bool StompEncoderDecoder::getShouldTerminate() {
    return shouldTerminate.load();
}
