package com.oeapi.oecom  ;

import com4j.*;

/**
 * IOEButton interface
 */
@IID("{12BAF144-264C-464B-9D58-C83B3781DD4C}")
public interface IOEButton extends Com4jObject {
    /**
     * Create a button in the specified toolbar. The bmpNormal is the image displayed in the button and the bmpMouseOver is optional to display a different image when the mouse is over.
     */
    @VTID(7)
    void create(
        int toolbarId,
        java.lang.String name,
        java.lang.String bmpNormal,
        java.lang.String bmpMouseOver);

    /**
     * Create a button with separator style.
     */
    @VTID(8)
    void createSeparator(
        int toolbarId);

    /**
     * Destroy the button.
     */
    @VTID(9)
    void destroy();

    /**
     * Create a sub button that is displayed if the drop down arrow is clicked by the user.
     */
    @VTID(10)
    com.oeapi.oecom.IOEButton createSubButton(
        java.lang.String name,
        java.lang.String bmpNormal,
        java.lang.String bmpMouseOver);

    /**
     * Get the button's id.
     */
    @VTID(11)
    int getID();

    /**
     * Set if the button is enabled or not.
     */
    @VTID(12)
    void setEnabled(
        int enabled);

    /**
     * Set if the button is shown or not.
     */
    @VTID(13)
    void setShown(
        int show);

    /**
     * Set the button's name.
     */
    @VTID(14)
    void setName(
        java.lang.String name);

    /**
     * Set the image displayed in the button.
     */
    @VTID(15)
    void setImageNormal(
        java.lang.String bmpNormal);

    /**
     * Set the image displayed in the button when the mouse pointer is over the button.
     */
    @VTID(16)
    void setImageMouseOver(
        java.lang.String bmp);

    /**
     * Get first sub button's id.
     */
    @VTID(17)
    int getFirstSubButtonID();

    /**
     * Get next sub button's id.
     */
    @VTID(18)
    int getNextSubButtonID();

    /**
     * Get sub button using the order in which they were created.
     */
    @VTID(19)
    com.oeapi.oecom.IOEButton getSubButtonByIndex(
        int index);

    /**
     * Returns TRUE if it is a separator.
     */
    @VTID(20)
    int isSeparator();

    /**
     * Set the button as with popup style or not.
     */
    @VTID(21)
    void setPopupStyle(
        int popup);

    /**
     * Set button style to check.
     */
    @VTID(22)
    void setCheckStyle(
        int check);

    /**
     * For check style buttons, set button as checked.
     */
    @VTID(23)
    void setChecked(
        int checked);

    /**
     * Returns TRUE if the button is checked.
     */
    @VTID(24)
    int isChecked();

    /**
     * Set button's tooltip. To be displayed, the toolbar must have EnableTooltips set to TRUE before the button's creation.
     */
    @VTID(25)
    void setTooltip(
        java.lang.String tooltip);

    /**
     * Returns TRUE if the button is visible.
     */
    @VTID(26)
    int isVisible();

    /**
     * Returns TRUE if the button is enabled.
     */
    @VTID(27)
    int isEnabled();

    /**
     * Get the button's name.
     */
    @VTID(28)
    java.lang.String getName();

    /**
     * Create a sub buttons separator that is displayed if the drop down arrow is clicked by the user.
     */
    @VTID(29)
    com.oeapi.oecom.IOEButton createSubSeparator();

        }
