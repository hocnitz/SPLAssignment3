package bgu.spl.net.impl.stomp.Frame;

import java.util.LinkedHashMap;
import java.util.Map;

public abstract class Frame {

    private Map<String,String> headers= new LinkedHashMap<>();

    private String body="";

    public String getBody(){
        return body;
    }

    public void setBody(String body){
        this.body=body;
    }

    public Map<String, String> getHeaders() {
        return headers;
    }

    public String get(String name) {
        return headers.get(name);
    }

    public void addHeader(String key, String value) {
        this.headers.put(key,value);
    }

}
