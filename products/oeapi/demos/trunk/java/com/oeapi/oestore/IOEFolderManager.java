package com.oeapi.oestore  ;

import com4j.*;

/**
 * IOEFolderManager interface
 */
@IID("{12BAF0C9-264C-464B-9D58-C83B3781DD4B}")
public interface IOEFolderManager extends Com4jObject {
    /**
     * Activate the IOEFolderManagerEvents::OnNewMessage notification. You need to activate it explicitly because the activation takes some time.
     */
    @VTID(7)
    void activateGlobalNotification();

    /**
     * Gets the special folder Inbox.
     */
    @VTID(8)
    com.oeapi.oestore.IOEFolder getInboxFolder();

    /**
     * Gets the special folder Outbox.
     */
    @VTID(9)
    com.oeapi.oestore.IOEFolder getOutboxFolder();

    /**
     * Gets the special folder Sent items.
     */
    @VTID(10)
    com.oeapi.oestore.IOEFolder getSentFolder();

    /**
     * Gets the special folder Deleted items.
     */
    @VTID(11)
    com.oeapi.oestore.IOEFolder getDeletedFolder();

    /**
     * Gets the special folder Drafts.
     */
    @VTID(12)
    com.oeapi.oestore.IOEFolder getDraftFolder();

    /**
     * Get the folder using the id.
     */
    @VTID(13)
    com.oeapi.oestore.IOEFolder getFolder(
        int folderId);

    /**
     * Reset the internal OEFolder iterator. Using GetNextFolder the iterator browses all the folder hierarchy.
     */
    @VTID(14)
    com.oeapi.oestore.IOEFolder getFirstFolder();

    /**
     * Get next folder. This function returns the first child folder of the previous folder if any, the sibling folder if any, or the parent. If the previous was the last folder, it returns NULL.
     */
    @VTID(15)
    com.oeapi.oestore.IOEFolder getNextFolder();

    /**
     * Get the current folder of the iterator. The last folder retrieved with GetFirstFolder or GetNextFolder.
     */
    @VTID(16)
    com.oeapi.oestore.IOEFolder getCurrentFolder();

    /**
     * Renames the folder.
     */
    @VTID(17)
    int renameFolder(
        int folderId,
        java.lang.String newName);

    /**
     * Moves the folder.
     */
    @VTID(18)
    int moveFolder(
        int folderId,
        int newParentId);

    /**
     * Deletes the specified folder. If the function returns TRUE the folderId is no longer valid.
     */
    @VTID(19)
    int deleteFolder(
        int folderId);

    /**
     * Creates a folder. If the function fails it returns -1. Otherwise, returns the folderId of the new folder.
     */
    @VTID(20)
    int createFolder(
        int parentFolderId,
        java.lang.String name);

    /**
     * Copies a message from srcFolderId to destFolderId.
     */
    @VTID(21)
    int copyMessage(
        int srcFolderId,
        int destFolderId,
        int msgId);

    /**
     * Moves a message from srcFolderId to destFolderId.
     */
    @VTID(22)
    int moveMessage(
        int srcFolderId,
        int destFolderId,
        int msgId);

}
