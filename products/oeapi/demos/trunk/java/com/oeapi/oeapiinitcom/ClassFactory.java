package com.oeapi.oeapiinitcom  ;

import com4j.*;

/**
 * Defines methods to create COM objects
 */
public abstract class ClassFactory {
    private ClassFactory() {} // instanciation is not allowed


    /**
     * OEAPIInitState coclass
     */
    public static com.oeapi.oeapiinitcom.IOEAPIInitState createOEAPIInitState() {
        return COM4J.createInstance( com.oeapi.oeapiinitcom.IOEAPIInitState.class, "{12BAF052-264C-464B-9D58-C83B3781DD4C}" );
    }

    /**
     * OEIdentity coclass
     */
    public static com.oeapi.oeapiinitcom.IOEIdentity createOEIdentity() {
        return COM4J.createInstance( com.oeapi.oeapiinitcom.IOEIdentity.class, "{12BAF062-264C-464B-9D58-C83B3781DD4B}" );
    }

    /**
     * OEAPIInit coclass
     */
    public static com.oeapi.oeapiinitcom.IOEAPIInit createOEAPIInit() {
        return COM4J.createInstance( com.oeapi.oeapiinitcom.IOEAPIInit.class, "{12BAF052-264C-464B-9D58-C83B3781DD4B}" );
    }
}
