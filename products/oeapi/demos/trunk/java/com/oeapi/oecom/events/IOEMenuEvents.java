package com.oeapi.oecom.events;

import com4j.*;

/**
 * IOEMenuEvents Interface
 */
@IID("{12BAF167-264C-464B-9D58-C83B3781DD4C}")
public abstract class IOEMenuEvents {
    /**
     * Event triggered when an item of this menu is clicked.
     */
    @DISPID(1)
    public void onItemClicked(
        int itemId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when an item of this menu is clicked.
     */
    @DISPID(2)
    public void onMsgWndItemClicked(
        int itemId,
        int msgWndId) {
            throw new UnsupportedOperationException();
    }

}
