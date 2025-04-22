package com.oeapi.oecom  ;

import com4j.*;

/**
 * Defines methods to create COM objects
 */
public abstract class ClassFactory {
    private ClassFactory() {} // instanciation is not allowed


    /**
     * OEMenuItem coclass
     */
    public static com.oeapi.oecom.IOEMenuItem createOEMenuItem() {
        return COM4J.createInstance( com.oeapi.oecom.IOEMenuItem.class, "{12BAF159-264C-464B-9D58-C83B3781DD4C}" );
    }

    /**
     * OEMsgWnd coclass
     */
    public static com.oeapi.oecom.IOEMsgWnd createOEMsgWnd() {
        return COM4J.createInstance( com.oeapi.oecom.IOEMsgWnd.class, "{12BAF15C-264C-464B-9D58-C83B3781DD4C}" );
    }

    /**
     * OEMenu coclass
     */
    public static com.oeapi.oecom.IOEMenu createOEMenu() {
        return COM4J.createInstance( com.oeapi.oecom.IOEMenu.class, "{12BAF169-264C-464B-9D58-C83B3781DD4C}" );
    }

    /**
     * OEButton coclass
     */
    public static com.oeapi.oecom.IOEButton createOEButton() {
        return COM4J.createInstance( com.oeapi.oecom.IOEButton.class, "{12BAF148-264C-464B-9D58-C83B3781DD4C}" );
    }

    /**
     * OEToolbar coclass
     */
    public static com.oeapi.oecom.IOEToolbar createOEToolbar() {
        return COM4J.createInstance( com.oeapi.oecom.IOEToolbar.class, "{12BAF145-264C-464B-9D58-C83B3781DD4C}" );
    }

    /**
     * OEAPIObj coclass
     */
    public static com.oeapi.oecom.IOEAPIObj createOEAPIObj() {
        return COM4J.createInstance( com.oeapi.oecom.IOEAPIObj.class, "{12BAF138-264C-464B-9D58-C83B3781DD4C}" );
    }
}
