package bgu.spl.net.impl.stomp.Frame.FrameIn;

import bgu.spl.net.impl.stomp.Frame.Frame;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;

public abstract class FrameIn extends Frame {
    public abstract void runMe(StompMessagingProtocolImpl protocol);
}
