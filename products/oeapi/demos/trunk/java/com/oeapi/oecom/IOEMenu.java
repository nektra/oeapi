package com.oeapi.oecom  ;

import com4j.*;

/**
 * IOEMenu interface
 */
@IID("{12BAF168-264C-464B-9D58-C83B3781DD4C}")
public interface IOEMenu extends Com4jObject {
    /**
     * Get the menu's index.
     */
    @VTID(7)
    int getIndex();

    /**
     * Create a menu item.
     */
    @VTID(8)
    com.oeapi.oecom.IOEMenuItem createItem(
        java.lang.String name);

    /**
     * Create a menu item before an existing menu item that has the specified string as it's text.
     */
    @VTID(9)
    com.oeapi.oecom.IOEMenuItem createItemBefore(
        java.lang.String name,
        java.lang.String before,
        int subString);

    /**
     * Create a menu items' separator.
     */
    @VTID(10)
    com.oeapi.oecom.IOEMenuItem createSeparator();

    /**
     * Create a separator before an existing menu item that has the specified string as it's text.
     */
    @VTID(11)
    com.oeapi.oecom.IOEMenuItem createSeparatorBefore(
        java.lang.String before,
        int subString);

    /**
     * Create a menu item at the specified position.
     */
    @VTID(12)
    com.oeapi.oecom.IOEMenuItem createItemAt(
        java.lang.String name,
        int position);

    /**
     * Create a menu separator at the specified position.
     */
    @VTID(13)
    com.oeapi.oecom.IOEMenuItem createSeparatorAt(
        int position);

}
