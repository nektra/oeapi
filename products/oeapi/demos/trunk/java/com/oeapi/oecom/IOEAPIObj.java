package com.oeapi.oecom  ;

import com4j.*;

/**
 * IOEAPIObj interface
 */
@IID("{12BAF137-264C-464B-9D58-C83B3781DD4C}")
public interface IOEAPIObj extends Com4jObject {
    /**
     * Get the id.
     */
    @VTID(7)
    int getID();

    /**
     * Get a menu by index.
     */
    @VTID(8)
    com.oeapi.oecom.IOEMenu getMenu(
        int index);

    /**
     * Get the message window by id.
     */
    @VTID(9)
    com.oeapi.oecom.IOEMsgWnd getMsgWnd(
        int msgWndId);

    /**
     * Create a toolbar in the send message window. It will appear when the user creates / replies / forwards an email.
     */
    @VTID(10)
    com.oeapi.oecom.IOEToolbar createToolbarInMsgWnd(
        com.oeapi.oecom.tagWNDSTYLE wndStyle);

    /**
     * Get the OE toolbar in msg wnd. It will appear when the user creates / replies / forwards an email.
     */
    @VTID(11)
    com.oeapi.oecom.IOEToolbar getOEToolbarInMsgWnd(
        com.oeapi.oecom.tagWNDSTYLE wndStyle);

    /**
     * Get the first msg window id of the OEAPIObj.
     */
    @VTID(12)
    int getFirstMsgWndID();

    /**
     * Get the next msg window id of the OEAPIObj.
     */
    @VTID(13)
    int getNextMsgWndID();

    /**
     * Get the next msg window id of the OEAPIObj.
     */
    @VTID(14)
    int getActiveMsgWndID();

    /**
     * Send a Key to the msg window.
     */
    @VTID(15)
    void sendKey(
        int keyCode,
        int controlCode,
        int setForeground);

    /**
     * Create a toolbar.
     */
    @VTID(16)
    com.oeapi.oecom.IOEToolbar createToolbar();

    /**
     * Get Outlook Express' toolbar.
     */
    @VTID(17)
    com.oeapi.oecom.IOEToolbar getOEToolbar();

    /**
     * Get the first toolbar of the OEAPIObj.
     */
    @VTID(18)
    int getFirstToolbarID();

    /**
     * Get the next toolbar's id of the OEAPIObj.
     */
    @VTID(19)
    int getNextToolbarID();

    /**
     * Get toolbar of the OEAPIObj by id.
     */
    @VTID(20)
    com.oeapi.oecom.IOEToolbar getToolbar(
        int toolbarId);

    /**
     * Remove all the toolbars created with this object.
     */
    @VTID(21)
    void removeAllToolbars();

    /**
     * Get first menu item's id created in this OEAPIObj. If you use GetMenu and then create the items, you can list the items using this function too.
     */
    @VTID(22)
    int getFirstMenuItemID();

    /**
     * Get next menu item's id created in this OEAPIObj.
     */
    @VTID(23)
    int getNextMenuItemID();

    /**
     * Get the menu item object.
     */
    @VTID(24)
    com.oeapi.oecom.IOEMenuItem getMenuItem(
        int itemId);

    /**
     * Remove all the menu items created with this object or any OEMenu obtained using GetMenu in this object.
     */
    @VTID(25)
    void removeAllMenuItems();

    /**
     * Send messages in Outbox folder and receive pending messages in server. Equivalent to press the 'Send/Receive' button, except that it does not display the progress dialog.
     */
    @VTID(26)
    void sendAndReceiveMessages();

    /**
     * Create a message in Outbox folder to be sent. The message will be sent at next the 'Send' / 'Send and Receive' process, or will be sent immediately according to the email client's configuration.
     */
    @VTID(27)
    void sendMessage(
        int folderId,
        int msgId);

    /**
     * Get the handle of OE's main window.
     */
    @VTID(28)
    int getOEMainWindow();

    /**
     * Get the selected folder's id.
     */
    @VTID(29)
    int getSelectedFolderID();

    /**
     * Get the first selected message's id in the current folder. Returns -1 if the selected folder is not local.
     */
    @VTID(30)
    int getFirstSelectedMessageID();

    /**
     * Get the next selected message's id in the current folder. If no more selected messages it returns -1.
     */
    @VTID(31)
    int getNextSelectedMessageID();

    /**
     * Get the current message's id of the current folder. Current message is displayed in the bottom part of OE.
     */
    @VTID(32)
    int getCurrentMessageID();

    /**
     * Set the next selected message by id in the current folder. This function clears the previous selection. After calling this function, only msgId will be selected.
     */
    @VTID(33)
    void setSelectedMessageID(
        int msgId);

    /**
     * Add a message by id to the current selection. After calling this function, the previous selection is kept and msgId is added to the selection.
     */
    @VTID(34)
    void addSelectedMessageID(
        int msgId);

    /**
     * Returns TRUE if there is any message selected in the messages' listview.
     */
    @VTID(35)
    int isAnyMessageSelected();

    /**
     * Select folder by id.
     */
    @VTID(36)
    void setSelectedFolderID(
        int selFolderId);

    /**
     * Display current message in a new window. Equivalent to double-click the message.
     */
    @VTID(37)
    void openCurrentMessage();

    /**
     * Open a new message window to compose a message. Equivalent to press 'Create Mail' button.
     */
    @VTID(38)
    void composeMessage();

    /**
     * Get a msg wnd menu by index.
     */
    @VTID(39)
    com.oeapi.oecom.IOEMenu getMsgWndMenu(
        com.oeapi.oecom.tagWNDSTYLE wndStyle,
        int index);

    /**
     * Get a msg wnd menu item object.
     */
    @VTID(40)
    com.oeapi.oecom.IOEMenuItem getMsgWndMenuItem(
        com.oeapi.oecom.tagWNDSTYLE wndStyle,
        int itemId);

}
