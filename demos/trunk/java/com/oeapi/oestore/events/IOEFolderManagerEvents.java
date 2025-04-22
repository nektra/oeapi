package com.oeapi.oestore.events;

import com4j.*;

/**
 * IOEFolderManagerEvents Interface
 */
@IID("{12BAF091-264C-464B-9D58-C83B3781DD4B}")
public abstract class IOEFolderManagerEvents {
    /**
     * Event triggered when a folder is created.
     */
    @DISPID(1)
    public void onFolderCreated(
        int folderId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a folder is deleted.
     */
    @DISPID(2)
    public void onFolderDeleted(
        int folderId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a folder is renamed.
     */
    @DISPID(3)
    public void onFolderRenamed(
        int folderId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a folder is moved.
     */
    @DISPID(4)
    public void onFolderMoved(
        int folderId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a message from a folder changes its read flag.
     */
    @DISPID(5)
    public void onFolderUnreadChanged(
        int folderId) {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when a new message is stored in a folder. This is a global event and can be used to know when a message is created in any folder.
     */
    @DISPID(6)
    public void onNewMessage(
        int folderId,
        int newMsgId) {
            throw new UnsupportedOperationException();
    }

}
