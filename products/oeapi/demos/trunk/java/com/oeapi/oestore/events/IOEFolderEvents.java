package com.oeapi.oestore.events;

import com4j.*;

/**
 * IOEFolderEvents Interface
 */
@IID("{12BAF0A2-264C-464B-9D58-C83B3781DD4B}")
public abstract class IOEFolderEvents {
    /**
     * Event triggered when a message arrives to this folder.
     */
    @DISPID(1)
    public void onNewMessage(
        int msgId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a message of this folder is deleted.
     */
    @DISPID(2)
    public void onMessageDeleted(
        int msgId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when the folder is deleted.
     */
    @DISPID(3)
    public void onDeletion() {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a message changes its unread state.
     */
    @DISPID(4)
    public void onMessageUnreadChange(
        int msgId,
        int unread) {
            throw new UnsupportedOperationException();
    }

}
