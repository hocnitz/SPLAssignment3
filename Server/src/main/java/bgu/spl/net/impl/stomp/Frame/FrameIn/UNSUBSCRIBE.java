package bgu.spl.net.impl.stomp.Frame.FrameIn;

import bgu.spl.net.Objects.Database;
import bgu.spl.net.Objects.User;
import bgu.spl.net.impl.stomp.Frame.Frame;
import bgu.spl.net.impl.stomp.Frame.FrameOut.ERROR;
import bgu.spl.net.impl.stomp.Frame.FrameOut.FrameOut;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.srv.ConnectionsImpl;

public class UNSUBSCRIBE extends FrameIn {

    @Override
    public void runMe(StompMessagingProtocolImpl protocol) {
        User curUser = protocol.getCurUser();
        Database database = protocol.getDatabase();
        int connectionId = protocol.getConnectionId();
        ConnectionsImpl<Frame> connections = protocol.getConnections();


        String id = get("id");
        if (curUser.getGenres().containsKey(id)) {
            String topic = curUser.getGenres().get(id);
            database.removeUserFromTopic(topic, curUser);
            curUser.getGenres().remove(id);
        }
    }
}
