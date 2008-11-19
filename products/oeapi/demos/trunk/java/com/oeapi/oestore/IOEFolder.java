package com.oeapi.oestore  ;

import com4j.*;

/**
 * IOEFolder interface
 */
@IID("{12BAF0A1-264C-464B-9D58-C83B3781DD4B}")
public interface IOEFolder extends Com4jObject {
    /**
     * Get the folder's id.
     */
    @VTID(7)
    int getID();

    /**
     * Get first child folder. Returns NULL if there is no child folder.
     */
    @VTID(8)
    com.oeapi.oestore.IOEFolder getFirstChild();

    /**
     * Get next child folder. Returns NULL when finishes.
     */
    @VTID(9)
    com.oeapi.oestore.IOEFolder getNextChild();

    /**
     * Get the folder's first message. Returns NULL if no message is found.
     */
    @VTID(10)
    com.oeapi.oestore.IOEMessage getFirstMessage();

    /**
     * Get next message of the folder. Returns NULL when finishes.
     */
    @VTID(11)
    com.oeapi.oestore.IOEMessage getNextMessage();

    /**
     * Get a folder's message by id. If the id does not match it returns NULL.
     */
    @VTID(12)
    com.oeapi.oestore.IOEMessage getMessage(
        int msgId);

    /**
     * Create a new message in the folder. msgSource must contain all the source of the message.
     */
    @VTID(13)
    com.oeapi.oestore.IOEMessage createMessage(
        java.lang.String msgSource,
        @DefaultValue("0")int unread);

    /**
     * Move the message to the 'Deleted items' folder, by id. If permanent is TRUE the message is permanently deleted, instead of just moved to the 'Deleted items' folder.
     */
    @VTID(14)
    int deleteMessage(
        int msgId,
        int permanent);

    /**
     * Get the folder's name.
     */
    @VTID(15)
    java.lang.String name();

    /**
     * Verifies if it's a valid folder.
     */
    @VTID(16)
    int isNull();

    /**
     * Marks the message as read.
     */
    @VTID(17)
    int markAsRead(
        int msgId);

    /**
     * Marks the message as unread.
     */
    @VTID(18)
    int markAsUnread(
        int msgId);

    /**
     * Returns TRUE if it is the special folder Inbox.
     */
    @VTID(19)
    int isInbox();

    /**
     * Returns TRUE if it is the special folder Outbox.
     */
    @VTID(20)
    int isOutbox();

    /**
     * Returns TRUE if it is the special folder Sent items.
     */
    @VTID(21)
    int isSent();

    /**
     * Returns TRUE if it is the special folder Deleted items.
     */
    @VTID(22)
    int isDeleted();

    /**
     * Returns TRUE if it is the special folder Drafts.
     */
    @VTID(23)
    int isDraft();

    /**
     * Renames the folder. Returns TRUE on success.
     */
    @VTID(24)
    int rename(
        java.lang.String newName);

    /**
     * Moves the folder. Returns TRUE on success.
     */
    @VTID(25)
    int move(
        int newParentId);

    /**
     * Deletes the folder. If the function returns TRUE the folder is no longer valid.
     */
    @VTID(26)
    int delete();

    /**
     * Creates a folder. If the function fails it returns -1. Otherwise, it returns the folderId of the new folder.
     */
    @VTID(27)
    int createFolder(
        java.lang.String name);

    /**
     * Gets the number of sub folders of the folder.
     */
    @VTID(28)
    int getSubFolderCount();

    /**
     * Gets the number of unread messages in the folder.
     */
    @VTID(29)
    int getUnreadCount();

    /**
     * Gets the number of messages in the folder.
     */
    @VTID(30)
    int getMessageCount();

}
