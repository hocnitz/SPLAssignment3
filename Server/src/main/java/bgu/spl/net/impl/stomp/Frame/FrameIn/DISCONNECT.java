package bgu.spl.net.impl.stomp.Frame.FrameIn;

import bgu.spl.net.Objects.Database;
import bgu.spl.net.Objects.User;
import bgu.spl.net.impl.stomp.Frame.Frame;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;

public class DISCONNECT extends FrameIn {
    @Override
    public void runMe(StompMessagingProtocolImpl protocol) {
        User curUser=protocol.getCurUser();
        Database database=protocol.getDatabase();
        curUser.getGenres().values().forEach((s) -> {
            database.removeUserFromTopic(s,curUser);
        });
        curUser.logout();
        protocol.shouldTerminate();
    }
}
