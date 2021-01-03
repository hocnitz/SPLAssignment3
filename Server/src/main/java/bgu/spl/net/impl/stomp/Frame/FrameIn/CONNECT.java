package bgu.spl.net.impl.stomp.Frame.FrameIn;

import bgu.spl.net.Objects.Database;
import bgu.spl.net.Objects.User;
import bgu.spl.net.impl.stomp.Frame.Frame;
import bgu.spl.net.impl.stomp.Frame.FrameOut.CONNECTED;
import bgu.spl.net.impl.stomp.Frame.FrameOut.ERROR;
import bgu.spl.net.impl.stomp.Frame.FrameOut.FrameOut;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.srv.ConnectionsImpl;


public class CONNECT extends FrameIn {

    @Override
    public void runMe(StompMessagingProtocolImpl protocol) {
        User curUser;
        Database database = protocol.getDatabase();
        int connectionId = protocol.getConnectionId();
        FrameOut output;
        ConnectionsImpl<Frame> connections = protocol.getConnections();

        String tmpName = get("login");
        String password = get("passcode");
        if (database.getClients()!=null && !database.getClients().containsKey(tmpName)) {
            database.newUser(tmpName, password, connectionId);
            curUser = database.getClients().get(tmpName);
            curUser.login(connectionId);
            output = new CONNECTED();
            output.addHeader("version", get("accept-version"));
            connections.send(connectionId, output);
            protocol.setCurUser(curUser);
        } else if (!database.getClients().get(tmpName).getPassword().equals(password)) {
            output = new ERROR();
            output.addHeader("body", "Wrong password");
            connections.send(connectionId, output);
        } else if (database.getClients().get(tmpName).isLoggedIn()) {
            output = new ERROR();
            output.addHeader("body", "User already logged in");
            connections.send(connectionId, output);
        } else {
            curUser = database.getClients().get(tmpName);
            curUser.login(connectionId);
            output = new CONNECTED();
            output.addHeader("version", get("accept-version"));
            connections.send(connectionId, output);
            protocol.setCurUser(curUser);
        }
    }
}
