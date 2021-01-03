package bgu.spl.net.impl.stomp;

import bgu.spl.net.Objects.Database;
import bgu.spl.net.Objects.User;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.Frame.Frame;
import bgu.spl.net.impl.stomp.Frame.FrameIn.FrameIn;
import bgu.spl.net.impl.stomp.Frame.FrameOut.FrameOut;
import bgu.spl.net.impl.stomp.Frame.FrameOut.RECEIPT;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;
import org.w3c.dom.ls.LSOutput;

import java.util.Map;

public class StompMessagingProtocolImpl implements StompMessagingProtocol<Frame> {

    private boolean shouldTerm = false;
    private int connectionId;
    private ConnectionsImpl<Frame> connections;
    private User curUser;
    private Database database = Database.getInstance();
    Map<String, String> headers;

    @Override
    public void start(int connectionId, Connections<Frame> connections) {
        this.connectionId = connectionId;
        this.connections = (ConnectionsImpl<Frame>) connections;
        System.out.println("id: " + connectionId + " connected succesfully");
    }

    @Override
    public void process(Frame frame) {
        FrameOut output;
        FrameIn message = (FrameIn) frame;
        headers = message.getHeaders();
        message.runMe(this);
        if (message.getHeaders().containsKey("receipt")) {
            output = new RECEIPT();
            output.addHeader("receipt-id", message.getHeaders().get("receipt"));
            connections.send(connectionId, output);
        }
    }

    public User getCurUser() {
        return curUser;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public ConnectionsImpl<Frame> getConnections() {
        return connections;
    }

    public Database getDatabase() {
        return database;
    }

    public Map<String, String> getHeaders() {
        return headers;
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerm;
    }

    public void setCurUser(User curUser) {
        this.curUser = curUser;
    }
}
