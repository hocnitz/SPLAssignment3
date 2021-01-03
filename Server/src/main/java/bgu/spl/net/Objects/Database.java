package bgu.spl.net.Objects;

import org.javatuples.Pair;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Database {
    private ConcurrentHashMap<String, User> clients;// <username,user>
    private ConcurrentHashMap<String, LinkedList<Pair<String, User>>> topics; // (Genre,Pair<sub-id,user>)

    private static class SingletonHolder {
        private static Database instance = new Database();
    }
    public Database(){
        clients = new ConcurrentHashMap<String, User>();
        topics = new ConcurrentHashMap<String, LinkedList<Pair<String, User>>>();
    }
    public static Database getInstance() {
        return SingletonHolder.instance;
    }

    public Map<String, User> getClients() {
        return clients;
    }

    public boolean newUser(String username, String password,int connectionId) {
        if (clients.containsKey(username))
            return false;
        User newClient = new User();
        newClient.setUserName(username);
        newClient.setPassword(password);
        clients.put(username, newClient);
        return true;
    }

    public ConcurrentHashMap<String, LinkedList<Pair<String, User>>> getTopics() {
        return topics;
    }

    public void removeUserFromTopic(String topic, User user) {
        LinkedList<Pair<String, User>> topicUsersList = topics.get(topic);
        topicUsersList.removeIf(p -> p.getValue1() == user);
    }
}