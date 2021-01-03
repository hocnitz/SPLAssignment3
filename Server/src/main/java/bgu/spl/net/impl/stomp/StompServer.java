package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        boolean blocking = true;
        if(args[1] == "tpc"){
            blocking = true;
        }
        else if(args[1] == "reactor"){
            blocking = false;
        }
        if(blocking) {
            System.out.println("SERVER STARTED - Thread Per Client");
            Server.threadPerClient(
                    Integer.parseInt(args[0]), //port
                    StompMessagingProtocolImpl::new, //protocol factory
                    StompEncoderDecoder::new //message encoder decoder factory
            ).serve();
        }
        else {
            System.out.println("SERVER STARTED - Reactor");
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    Integer.parseInt(args[0]), //port
                    StompMessagingProtocolImpl::new, //protocol factory
                    StompEncoderDecoder::new //message encoder decoder factory
            ).serve();
        }
    }


}
