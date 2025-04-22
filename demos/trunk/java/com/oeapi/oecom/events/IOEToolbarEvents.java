package com.oeapi.oecom.events;

import com4j.*;

/**
 * IOEToolbarEvents Interface
 */
@IID("{12BAF155-264C-464B-9D58-C83B3781DD4C}")
public abstract class IOEToolbarEvents {
    /**
     * Event triggered when a button of this toolbar is clicked.
     */
    @DISPID(1)
    public void onButtonClicked(
        int buttonId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a button of this toolbar is clicked in a message window.
     */
    @DISPID(2)
    public void onButtonMsgWndClicked(
        int buttonId,
        int msgWndId) {
            throw new UnsupportedOperationException();
    }

}
