package bgu.spl.net.srv;

import bgu.spl.net.Objects.Database;
import bgu.spl.net.impl.stomp.Frame.Frame;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionsImpl<T> implements Connections<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> handlerMap=new ConcurrentHashMap<>();
    private Database database = Database.getInstance();
    private AtomicInteger runningId = new AtomicInteger(0);

    @Override
    public boolean send(int connectionId, T msg) {
        if (!handlerMap.containsKey(connectionId))
            return false;
        handlerMap.get(connectionId).send(msg);
        return true;
    }

    @Override
    public void send(String channel, T msg) {
        if(database.getTopics()!=null && database.getTopics().get(channel)!=null)
        database.getTopics().get(channel).forEach( stringUserPair -> {
            String topicId = stringUserPair.getValue0();
            ((Frame) msg).addHeader("subscription", topicId);
            int tmpid = stringUserPair.getValue1().getConnectionId();
            send(tmpid,msg);
        });
    }

    @Override
    public void disconnect(int connectionId) {
        handlerMap.remove(connectionId);
    }

    public Map<Integer, ConnectionHandler<T>> getHandlerMap() {
        return handlerMap;
    }

    public int addClient(ConnectionHandler<T> handler) {
        int id;
        handlerMap.put(id = runningId.getAndIncrement(), handler);
        return id;
    }
    public int size(){
        return handlerMap.size();
    }
}
