package bgu.spl.net.impl.stomp.Frame.FrameIn;

import bgu.spl.net.Objects.Database;
import bgu.spl.net.Objects.User;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import org.javatuples.Pair;

import java.util.LinkedList;

public class SUBSCRIBE extends FrameIn {
    @Override
    public void runMe(StompMessagingProtocolImpl protocol) {
        String topic = get("destination");
        String id = get("id");
        User curUser = protocol.getCurUser();
        Database database = protocol.getDatabase();
        Pair<String, User> newPair = new Pair<>(id, curUser);
        if (curUser.getGenres()!=null && !curUser.getGenres().containsValue(topic)) {
            if (!database.getTopics().containsKey(topic)) {
                LinkedList<Pair<String, User>> clientList = new LinkedList<>();
                clientList.add(newPair);
                database.getTopics().put(topic, clientList);
            } else {
                database.getTopics().get(topic).add(newPair);
            }
            curUser.getGenres().put(id, topic);
        }
    }
}
