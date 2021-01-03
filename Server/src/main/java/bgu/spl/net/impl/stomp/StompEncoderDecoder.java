package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.impl.stomp.Frame.Frame;
import bgu.spl.net.impl.stomp.Frame.FrameIn.FrameIn;

import java.lang.reflect.Constructor;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

public class StompEncoderDecoder implements MessageEncoderDecoder<Frame> {

    List<Byte> bytesAsList = new ArrayList<>();

    int i = 0;

    @Override
    public Frame decodeNextByte(byte nextByte) {
        FrameIn frame = null;
        if (nextByte != '\0') {
            bytesAsList.add(nextByte);
        } else {
            byte[] bytes = new byte[bytesAsList.size()];
            bytesAsList.forEach(b ->
                    bytes[i++] = b
            );
            String str = new String(bytes, StandardCharsets.UTF_8);
            frame = frameGenerator(str);
            i = 0;
            bytesAsList.clear();
        }
        return frame;
    }

    @SuppressWarnings("unchecked")
    private FrameIn frameGenerator(String str) {
        FrameIn frame = null;
        LinkedList<String> lines = FixAndSplit(str);
        String type_str = "bgu.spl.net.impl.stomp.Frame.FrameIn."+lines.removeFirst();
        try {
            Class<? extends FrameIn> type = (Class<? extends FrameIn>) Class.forName(type_str);
            Constructor<? extends FrameIn> constructor = type.getConstructor();
            frame = constructor.newInstance();
            for (String line : lines) {
                String[] seperated = line.split(":", 2);
                String key = seperated[0];
                String value = seperated[1];

                frame.addHeader(key, value);
            }
            frame.setBody(frame.getHeaders().get("body"));
        } catch (Exception e){
            e.printStackTrace();
        }
        return frame;
    }

    private LinkedList<String> FixAndSplit(String msg) {

        msg = msg.replaceAll("\n\r\n\r", "\r\r");
        msg = msg.replaceAll("\r\n\r\n", "\r\r");
        msg = msg.replaceAll("\r\n", "\r");
        msg = msg.replaceAll("\n\r", "\r");
        msg = msg.replaceAll("\n", "\r");
        msg = msg.replaceAll("\r\r", "\rbody:");
        String[] lines = msg.split("\r", 0);
        List<String> lines_lst = Arrays.asList(lines);
        return new LinkedList<>(lines_lst);

    }

    @Override
    public byte[] encode(Frame message) {
        String output =message.getClass().getSimpleName()+"\n";
        String body = "";
        for (String h:message.getHeaders().keySet()) {
            if(!h.equals("body"))
                output += h+":"+message.getHeaders().get(h)+"\n";
            else
                body = message.getHeaders().get(h);
        }
        output += "\n" + body ;
        output += "\0";
        return output.getBytes();
    }
}
