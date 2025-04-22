package com.oeapi.oestore  ;

import com4j.*;

/**
 * IOEMessage interface
 */
@IID("{12BAF0B5-264C-464B-9D58-C83B3781DD4B}")
public interface IOEMessage extends Com4jObject {
    /**
     * Get the id.
     */
    @VTID(7)
    int getID();

    /**
     * Get the folder id.
     */
    @VTID(8)
    int getFolderID();

    /**
     * Get the subject.
     */
    @VTID(9)
    java.lang.String getSubject();

    /**
     * Get the normalized subject.
     */
    @VTID(10)
    java.lang.String getNormalSubject();

    /**
     * Get the string that is displayed as to.
     */
    @VTID(11)
    java.lang.String getDisplayTo();

    /**
     * Get the string that is displayed as from.
     */
    @VTID(12)
    java.lang.String getDisplayFrom();

    /**
     * Get the message's state flags. See OEAPIMSGSTATE for the state list.
     */
    @VTID(13)
    int getState();

    /**
     * Set the state flags. See OEAPIMSGSTATE for the state list. Only useful when a message is cloned, the new message can change state. Messages stored cannot change their state.
     */
    @VTID(14)
    void setState(
        int state);

    /**
     * Mark as read.
     */
    @VTID(15)
    int markAsRead();

    /**
     * Mark as unread.
     */
    @VTID(16)
    int markAsUnread();

    /**
     * The message is moved to the 'Deleted items' folder. If permanent is TRUE the message is permanently deleted instead of moved to the 'Deleted items' folder.
     */
    @VTID(17)
    int delete(
        int permanent);

    /**
     * Get the entire message length.
     */
    @VTID(18)
    int getSourceLength();

    /**
     * Get all the source of the message. The message could be long, so this function could take a lot of time.
     */
    @VTID(19)
    java.lang.String getAllSource();

    /**
     * Get the first (in the first call) maxBytesToRead (if possible) of the message. Next calls to this function read next parts of the message. It returns an empty string when finished.
     */
    @VTID(20)
    java.lang.String getSource(
        int maxBytesToRead);

    /**
     * Get the header of the message.
     */
    @VTID(21)
    java.lang.String getHeader();

    /**
     * Get the length of the header of the message.
     */
    @VTID(22)
    int getHeaderLength();

    /**
     * Get the entire body of the message. The message could be long, so this function could take a lot of time.
     */
    @VTID(23)
    java.lang.String getAllBody();

    /**
     * Get the first (in the first call) maxBytesToRead (if possible). Next calls to this function read next parts of the message. It returns an empty string when finished.
     */
    @VTID(24)
    java.lang.String getBody(
        int maxBytesToRead);

    /**
     * Get the length of the body.
     */
    @VTID(25)
    int getBodyLength();

    /**
     * After calling this function GetBody and GetSource functions starts again from the beginning.
     */
    @VTID(26)
    void getContentReset();

    /**
     * Returns true if the message has normal priority.
     */
    @VTID(27)
    int hasNormalPriority();

    /**
     * Returns true if the message has low priority.
     */
    @VTID(28)
    int hasLowPriority();

    /**
     * Returns true if the message has high priority.
     */
    @VTID(29)
    int hasHighPriority();

    /**
     * Get the first HTML body handle (if any) of the message, or 0 if the message has no HTML body. If the message is multi-part this function is useless.
     */
    @VTID(30)
    int getHTMLBody();

    /**
     * Get the first text (plain) body handle (if any) of the message or 0. If the message is multi-part this function is useless.
     */
    @VTID(31)
    int getPlainBody();

    /**
     * Get body handle specifying the location. If it is not found, returns 0.
     */
    @VTID(32)
    int getBodyHandle(
        int prevBodyHandle,
        com.oeapi.oestore.tagOEAPIBODYLOCATION bodyLoc);

    /**
     * Taking bodyHandle as reference, inserts a new body using bodyLoc as the relation between the new body and bodyHandle. Returns 0 on error.
     */
    @VTID(33)
    int insertBody(
        int bodyHandle,
        com.oeapi.oestore.tagOEAPIBODYLOCATION bodyLoc);

    /**
     * Delete a body. Returns TRUE on success.
     */
    @VTID(34)
    int deleteBody(
        int bodyHandle,
        com.oeapi.oestore.tagOEAPIBODYDELFLAGS flags);

    /**
     * Returns TRUE if the body is of the specified content type.
     */
    @VTID(35)
    int isBodyContentType(
        int bodyHandle,
        java.lang.String priContentType,
        java.lang.String secContentType);

    /**
     * Returns the primary content type of the body handle.
     */
    @VTID(36)
    java.lang.String getBodyPrimaryContentType(
        int bodyHandle);

    /**
     * Returns the secondary content type of the body handle.
     */
    @VTID(37)
    java.lang.String getBodySecondaryContentType(
        int bodyHandle);

    /**
     * Get the display name of the body.
     */
    @VTID(38)
    java.lang.String getBodyDisplayName(
        int bodyHandle);

    /**
     * Get the text of the specified body handle.
     */
    @VTID(39)
    java.lang.String getBodyText(
        int bodyHandle);

    /**
     * Set the text of the specified body handle. The type of the text must be specified, if you do not want to change it, send empty strings in both parameters and the type will remain unchanged.
     */
    @VTID(40)
    int setBodyText(
        int bodyHandle,
        java.lang.String bodyText,
        java.lang.String priContentType,
        java.lang.String secContentType);

    /**
     * Save the specified body in a file.
     */
    @VTID(41)
    int saveBodyToFile(
        int bodyHandle,
        java.lang.String path,
        int useDefaultName);

    /**
     * Set the body content from the specified file. This function is useful if you want to add an attachment.
     */
    @VTID(42)
    int setBodyFromFile(
        int bodyHandle,
        java.lang.String filename,
        java.lang.String priContentType,
        java.lang.String secContentType);

    /**
     * Returns TRUE if the specified body is an attachment.
     */
    @VTID(43)
    int isBodyAttachment(
        int bodyHandle);

    /**
     * Get the text of the first property of the body. If the property does not exist propId is -1. Otherwise, contains property id.
     */
    @VTID(44)
    java.lang.String getFirstBodyProp(
        int bodyHandle,
        Holder<Integer> propId);

    /**
     * Get the text of the next property of the body. If the property does not exist propId is -1. Otherwise, contains property id.
     */
    @VTID(45)
    java.lang.String getNextBodyProp(
        Holder<Integer> propId);

    /**
     * Gets the text of the specified property. If the property does not exists it returns 0 (False). propId is a long value and can be the value obtained using GetFirstBodyProp / GetNextBodyProp.
     */
    @VTID(46)
    java.lang.String getBodyProp(
        int bodyHandle,
        com.oeapi.oestore.tagOEAPIMIMEPROPID propId,
        Holder<Integer> exist);

    /**
     * Sets the value of the property, specifying the property by id. Returns TRUE on success. propId is a long value and can be the value obtained using GetFirstBodyProp / GetNextBodyProp.
     */
    @VTID(47)
    int setBodyProp(
        int bodyHandle,
        com.oeapi.oestore.tagOEAPIMIMEPROPID propId,
        java.lang.String propText);

    /**
     * Sets the value of the property, specifying the property by name. Returns TRUE on success.
     */
    @VTID(48)
    int setBodyPropByName(
        int bodyHandle,
        java.lang.String propName,
        java.lang.String propText);

    /**
     * Gets the value of the property, specifying the property by name. If the property does not exist, returns 0 (False).
     */
    @VTID(49)
    java.lang.String getBodyPropByName(
        int bodyHandle,
        java.lang.String propName,
        Holder<Integer> exist);

    /**
     * Remove a property specifying the id. Returns TRUE on success. propId is a long value and can be obtained using GetFirstBodyProp / GetNextBodyProp.
     */
    @VTID(50)
    int deleteBodyProp(
        int bodyHandle,
        com.oeapi.oestore.tagOEAPIMIMEPROPID propId);

    /**
     * Remove a property specifying the name. Returns TRUE on success.
     */
    @VTID(51)
    int deleteBodyPropByName(
        int bodyHandle,
        java.lang.String propName);

    /**
     * Create a copy of the message in the specified folder. Only those functions that manipulate bodies and SetState can be used before you call Commit because the message does not have an id yet.
     */
    @VTID(52)
    com.oeapi.oestore.IOEMessage clone(
        int folderId);

    /**
     * Commit changes made in the bodies (properties and content). If you do not call this function after modifying bodies, the changes will not be saved.
     */
    @VTID(53)
    int commit();

    /**
     * The message is committed (all the changes made are saved) and sent using the default account of current user. Returns TRUE on success.
     */
    @VTID(54)
    int send();

    /**
     * Get the first attachment of the message. Returns 0 if the message does not have attachments.
     */
    @VTID(55)
    int getFirstAttachment();

    /**
     * Get next attachment of the message. Returns 0 if the message does not have attachments.
     */
    @VTID(56)
    int getNextAttachment();

    /**
     * Get the filename prop (equivalent to: GetBodyProp(bodyHandle, OE_PID_PAR_FILENAME, exist, filename).
     */
    @VTID(57)
    java.lang.String getFilename(
        int bodyHandle);

    /**
     * Add an attachment to the message. Returns the new body handle or 0 on error.
     */
    @VTID(58)
    int attachFile(
        java.lang.String filename);

    /**
     * Set the new message's folder id.
     */
    @VTID(59)
    void setFolderID(
        int folderId);

    /**
     * Save message source to the specified filename.
     */
    @VTID(60)
    int saveAsFile(
        java.lang.String filename);

}
