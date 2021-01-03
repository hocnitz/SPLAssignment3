package bgu.spl.net.impl.stomp.Frame.FrameIn;

import bgu.spl.net.impl.stomp.Frame.Frame;
import bgu.spl.net.impl.stomp.Frame.FrameOut.MESSAGE;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.srv.ConnectionsImpl;

public class SEND extends FrameIn {

    @Override
    public void runMe(StompMessagingProtocolImpl protocol) {
        ConnectionsImpl<Frame> connections = protocol.getConnections();

        String topic = get("destination");
        MESSAGE firstMessage = new MESSAGE();
        firstMessage.addHeader("Message-id", firstMessage.getRunningId().toString());
        firstMessage.addHeader("destination", topic);
        firstMessage.addHeader("body", getBody());
        connections.send(topic, firstMessage);
    }
}
