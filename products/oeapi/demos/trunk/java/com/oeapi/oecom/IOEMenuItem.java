package com.oeapi.oecom  ;

import com4j.*;

/**
 * IOEMenuItem interface
 */
@IID("{12BAF158-264C-464B-9D58-C83B3781DD4C}")
public interface IOEMenuItem extends Com4jObject {
    /**
     * Get the menu item id.
     */
    @VTID(7)
    int getID();

    /**
     * Create a menu item.
     */
    @VTID(8)
    void create(
        int menuId,
        java.lang.String name);

    /**
     * Destroy menu item.
     */
    @VTID(9)
    void destroy();

    /**
     * Create a menu item before an existing menu item that has the specified string as text.
     */
    @VTID(10)
    void createBefore(
        int menuId,
        java.lang.String name,
        java.lang.String before,
        int subString);

    /**
     * Create a sub item of this menu item.
     */
    @VTID(11)
    com.oeapi.oecom.IOEMenuItem createSubItem(
        java.lang.String name);

    /**
     * Set the menu item as enabled or not.
     */
    @VTID(12)
    void setEnabled(
        int enabled);

    /**
     * Create a separator as child of this menu item.
     */
    @VTID(13)
    com.oeapi.oecom.IOEMenuItem createSeparator();

    /**
     * Set the menu item as with check style.
     */
    @VTID(14)
    void setCheck(
        int check);

    /**
     * Set the checked property for check style menu items.
     */
    @VTID(15)
    void setChecked(
        int checked);

    /**
     * For check style menu items, returns TRUE when it is checked.
     */
    @VTID(16)
    int isChecked();

    /**
     * Set the menu item as visible or not.
     */
    @VTID(17)
    void setVisible(
        int visible);

    /**
     * Returns TRUE if the menu item is visible.
     */
    @VTID(18)
    int isVisible();

    /**
     * Returns TRUE if the menu item is enabled.
     */
    @VTID(19)
    int isEnabled();

    /**
     * Set menu item's text.
     */
    @VTID(20)
    void setText(
        java.lang.String name);

    /**
     * Get menu item's text.
     */
    @VTID(21)
    java.lang.String getText();

    /**
     * Create a menu item at the specified position.
     */
    @VTID(22)
    void createAt(
        int menuId,
        java.lang.String name,
        int position);

}
