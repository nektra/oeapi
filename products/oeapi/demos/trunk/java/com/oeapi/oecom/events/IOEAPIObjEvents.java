package com.oeapi.oecom.events;

import com4j.*;

/**
 * IOEAPIObjEvents Interface
 */
@IID("{12BAF139-264C-464B-9D58-C83B3781DD4C}")
public abstract class IOEAPIObjEvents {
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
     * Event triggered when the selected folder changes.
     */
    @DISPID(3)
    public void onFolderSelectionChanged(
        int folderId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when the message selection changes in the current folder.
     */
    @DISPID(4)
    public void onMessageSelectionChanged() {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when the current message (that is displayed in OE's Preview Pane) changes.
     */
    @DISPID(5)
    public void onCurrentMessageChanged(
        int msgId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when OE starts downloading and uploading messages from the server.
     */
    @DISPID(6)
    public void onMessageDownloadStarted() {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when OE finishes downloading and uploading messages from the server.
     */
    @DISPID(7)
    public void onMessageDownloadFinished() {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a message is created in Outbox folder and OE is prepared to send it. After this function returns, the message is sent.
     */
    @DISPID(8)
    public void onNewMessageInOutbox(
        int msgId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a button of this toolbar is clicked in a message window.
     */
    @DISPID(9)
    public void onToolbarButtonMsgWndClicked(
        int toolbarId,
        int buttonId,
        int msgWndId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when the 'Send' button is clicked in a message window.
     */
    @DISPID(10)
    public void onSendButtonMsgWndClicked(
        int msgWndId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a message window is created.
     */
    @DISPID(11)
    public void onNewMsgWnd(
        int msgWndId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a message window is closed.
     */
    @DISPID(12)
    public void onMsgWndClosed(
        int msgWndId,
        int isMainWindow) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a message or folder changes.
     */
    @DISPID(13)
    public void onDatabaseChange(
        com.oeapi.oecom.tagOE_DATABASE_TRANSACTION dt,
        int folderId,
        int objId,
        int newParentId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a menu item is pressed on a message window.
     */
    @DISPID(14)
    public void onMsgWndMenuItemClicked(
        int menuIndex,
        int menuItemId,
        int msgWndId) {
            throw new UnsupportedOperationException();
    }

}
