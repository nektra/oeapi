package com.oeapi.oecom  ;

import com4j.*;

/**
 * IOEToolbar interface
 */
@IID("{12BAF140-264C-464B-9D58-C83B3781DD4C}")
public interface IOEToolbar extends Com4jObject {
    /**
     * Show or hide toolbar.
     */
    @VTID(7)
    void setShown(
        int show);

    /**
     * Create a button in the toolbar. The bmpNormal is the image displayed in the button and the bmpMouseOver is optional to display a different image when the mouse is over.
     */
    @VTID(8)
    com.oeapi.oecom.IOEButton createButton(
        java.lang.String name,
        java.lang.String bmpNormal,
        java.lang.String bmpMouseOver);

    /**
     * Create a toolbar separator.
     */
    @VTID(9)
    com.oeapi.oecom.IOEButton createSeparator();

    /**
     * Get the toolbar's id.
     */
    @VTID(10)
    int getID();

    /**
     * Create toolbar.
     */
    @VTID(11)
    void create(
        int oeapiId);

    /**
     * Create a toolbar in the send message window. It will appear when the user creates / replies / forwards an email.
     */
    @VTID(12)
    void createInMsgWnd(
        com.oeapi.oecom.tagWNDSTYLE wndStyle,
        int oeapiId);

    /**
     * Destroy the toolbar.
     */
    @VTID(13)
    void destroy();

    /**
     * If TRUE the toolbar displays large buttons.
     */
    @VTID(14)
    void setLargeButtons(
        int largeBtns);

    /**
     * Get the toolbar's first button.
     */
    @VTID(15)
    int getFirstButtonID();

    /**
     * Get the toolbar's next button.
     */
    @VTID(16)
    int getNextButtonID();

    /**
     * Get the button object by id.
     */
    @VTID(17)
    com.oeapi.oecom.IOEButton getButton(
        int buttonId);

    /**
     * Get button using the order in which they were created.
     */
    @VTID(18)
    com.oeapi.oecom.IOEButton getButtonByIndex(
        int index);

    /**
     * Get toolbar handle.
     */
    @VTID(19)
    int getHandle();

    /**
     * Set the toolbar buttons' maximum text rows. Setting it to 0 shows no text.
     */
    @VTID(20)
    void setMaxTextRows(
        int rows);

    /**
     * Enable tooltips. For tooltips to be displayed, this option must be set right after toolbar creation, before creating the toolbar's buttons.
     */
    @VTID(21)
    void enableTooltips(
        int bEnable);

    /**
     * Set the toolbar buttons images' transparent color.
     */
    @VTID(22)
    void setMaskColor(
        int color);

    /**
     * Get toolbar buttons images' transparent color.
     */
    @VTID(23)
    int getMaskColor();

    /**
     * Set the toolbar pictures' size in pixels.
     */
    @VTID(24)
    int setPictureSize(
        int cx,
        int cy);

    /**
     * Get the toolbar pictures' size in pixels.
     */
    @VTID(25)
    int getPictureSize(
        Holder<Integer> cx,
        Holder<Integer> cy);

}
