package bgu.spl.net.Objects;

import bgu.spl.net.srv.ConnectionHandler;


import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class User {

    private String userName;
    private String password;
    private boolean loggedIn;
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<String>> books;
    private ConcurrentHashMap<String,String> genres;// <id,genre>
    private int connectionId=0;

    public Map<String,String> getGenres() {
        return genres;
    }

    public User(){
        loggedIn = true;
        books = new ConcurrentHashMap<>();
        genres = new ConcurrentHashMap<>();
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public Map<String, ConcurrentLinkedQueue<String>> getBooks() {
        return books;
    }

    public void login(int connectionId){
        loggedIn = true;
        this.connectionId= connectionId;
    }

    public void logout(){
        genres.clear();
        loggedIn = false;
        this.connectionId= 0;
    }

    public boolean isLoggedIn() {
        return loggedIn;
    }

    public void setLoggedIn(boolean loggedIn) {
        this.loggedIn = loggedIn;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public void setConnectionId(int connectionId) {
        this.connectionId = connectionId;
    }


}
