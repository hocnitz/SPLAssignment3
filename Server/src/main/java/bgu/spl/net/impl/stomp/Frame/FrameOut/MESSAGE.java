package bgu.spl.net.impl.stomp.Frame.FrameOut;

import java.util.concurrent.atomic.AtomicInteger;

public class MESSAGE extends FrameOut {

    private static AtomicInteger runningId=new AtomicInteger(20);

    public MESSAGE() {
        int id=runningId.getAndIncrement();
        StringBuilder ids= new StringBuilder(id + "");

        while (ids.length()<5)
            ids.insert(0, "0");

        addHeader("Message-id", ids.toString());
    }

    public static AtomicInteger getRunningId() {
        return runningId;
    }

}
