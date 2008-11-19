package com.oeapi.oestore  ;

import com4j.*;

/**
 * Defines methods to create COM objects
 */
public abstract class ClassFactory {
    private ClassFactory() {} // instanciation is not allowed


    /**
     * OEMessage coclass
     */
    public static com.oeapi.oestore.IOEMessage createOEMessage() {
        return COM4J.createInstance( com.oeapi.oestore.IOEMessage.class, "{12BAF0B1-264C-464B-9D58-C83B3781DD4B}" );
    }

    /**
     * OEFolder coclass
     */
    public static com.oeapi.oestore.IOEFolder createOEFolder() {
        return COM4J.createInstance( com.oeapi.oestore.IOEFolder.class, "{12BAF070-264C-464B-9D58-C83B3781DD4B}" );
    }

    /**
     * OEFolderManager coclass
     */
    public static com.oeapi.oestore.IOEFolderManager createOEFolderManager() {
        return COM4J.createInstance( com.oeapi.oestore.IOEFolderManager.class, "{12BAF0A5-264C-464B-9D58-C83B3781DD4B}" );
    }
}
