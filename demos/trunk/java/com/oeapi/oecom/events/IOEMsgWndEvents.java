package com.oeapi.oecom.events;

import com4j.*;

/**
 * IOEMsgWndEvents Interface
 */
@IID("{12BAF15A-264C-464B-9D58-C83B3781DD4C}")
public abstract class IOEMsgWndEvents {
    /**
     * Event triggered when a toolbar button is clicked.
     */
    @DISPID(1)
    public void onToolbarButtonClicked(
        int toolbarId,
        int buttonId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a menu item is clicked.
     */
    @DISPID(2)
    public void onMenuItemClicked(
        int menuIndex,
        int menuItemId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a message window is closed.
     */
    @DISPID(3)
    public void onClose(
        int isMainWindow) {
            throw new UnsupportedOperationException();
    }

}
