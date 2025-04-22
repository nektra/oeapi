package com.oeapi.oecom.events;

import com4j.*;

/**
 * IOEMenuItemEvents Interface
 */
@IID("{12BAF157-264C-464B-9D58-C83B3781DD4C}")
public abstract class IOEMenuItemEvents {
    /**
     * Event triggered when the menu is clicked.
     */
    @DISPID(1)
    public void onClick() {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when the menu is clicked.
     */
    @DISPID(2)
    public void onMsgWndClick(
        int msgWndId) {
            throw new UnsupportedOperationException();
    }

}
