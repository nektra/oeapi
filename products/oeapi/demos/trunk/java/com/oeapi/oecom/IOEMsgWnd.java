package com.oeapi.oecom  ;

import com4j.*;

/**
 * IOEMsgWnd interface
 */
@IID("{12BAF15B-264C-464B-9D58-C83B3781DD4C}")
public interface IOEMsgWnd extends Com4jObject {
    /**
     * Get the 'To' address list of the message.
     */
    @VTID(7)
    java.lang.String getTo();

    /**
     * Set the first address of the message. By default 'To:'.
     */
    @VTID(8)
    void setTo(
        java.lang.String address);

    /**
     * Get the 'Cc' address list of the message.
     */
    @VTID(9)
    java.lang.String getCc();

    /**
     * Set the 'Cc' address list of the message.
     */
    @VTID(10)
    void setCc(
        java.lang.String address);

    /**
     * Cancel send process of the message window. This function should be called inside the event IOEAPIObjEvents::OnSendButtonMsgWndClicked to cancel the 'Send' action started by the user or by the code.
     */
    @VTID(11)
    void cancelSend();

    /**
     * Get the message's subject.
     */
    @VTID(12)
    java.lang.String getSubject();

    /**
     * Set the message's subject.
     */
    @VTID(13)
    void setSubject(
        java.lang.String subject);

    /**
     * Get the body of the message part of the window.
     */
    @VTID(14)
    java.lang.String getBody();

    /**
     * Get the body length of the message part of the window in characters not including the NULL termination.
     */
    @VTID(15)
    int getBodyLength();

    /**
     * Set the source of the message part of the window.
     */
    @VTID(16)
    void setBody(
        Holder<java.lang.String> body);

    /**
     * Set the source of the message part of the window. This function is recommended if you want to modify an outgoing message.
     */
    @VTID(17)
    void setBodyHTML(
        Holder<java.lang.String> body);

    /**
     * Get the window style.
     */
    @VTID(18)
    com.oeapi.oecom.tagWNDSTYLE getWndStyle();

    /**
     * Press the 'Send' button of the window. Only valid for windows with wnd style different than OE_MSG_DETAIL_WND.
     */
    @VTID(19)
    void sendMail();

    /**
     * Send a Key to the message window.
     */
    @VTID(20)
    void sendKey(
        int keyCode,
        int controlCode,
        int setForeground);

    /**
     * Get message window's id.
     */
    @VTID(21)
    int getID();

    /**
     * Get message window's toolbar by index.
     */
    @VTID(22)
    com.oeapi.oecom.IOEToolbar getToolbarByIndex(
        int toolbarIndex);

    /**
     * Get message window's toolbar by id.
     */
    @VTID(23)
    com.oeapi.oecom.IOEToolbar getToolbar(
        int toolbarId);

    /**
     * Get message window's menu by index.
     */
    @VTID(24)
    com.oeapi.oecom.IOEMenu getMenu(
        int menuIdx);

    /**
     * Get message window's menu item by id
     */
    @VTID(25)
    com.oeapi.oecom.IOEMenuItem getMenuItem(
        int itemId);

    /**
     * Get IHTMLDocument2 interface of this message window.
     */
    @VTID(26)
    @ReturnValue(type=NativeType.Dispatch)
    com4j.Com4jObject getIHTMLDocument2();

}
