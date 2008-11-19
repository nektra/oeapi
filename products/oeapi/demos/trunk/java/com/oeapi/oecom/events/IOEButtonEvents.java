package com.oeapi.oecom.events;

import com4j.*;

/**
 * IOEButtonEvents Interface
 */
@IID("{12BAF156-264C-464B-9D58-C83B3781DD4C}")
public abstract class IOEButtonEvents {
    /**
     * Event triggered when the button is clicked.
     */
    @DISPID(1)
    public void onClick() {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when the button is clicked in a message window.
     */
    @DISPID(2)
    public void onMsgWndClick(
        int msgWndId) {
            throw new UnsupportedOperationException();
    }

}
