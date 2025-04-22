package com.oeapi.oecom  ;

import com4j.*;

public enum tagOE_DATABASE_TRANSACTION implements ComEnum {
    OE_TR_INSERT_MESSAGE(257),
    OE_TR_DELETE_MESSAGE(260),
    OE_TR_UNREAD_MESSAGE(264),
    OE_TR_READ_MESSAGE(272),
    OE_TR_INSERT_FOLDER(513),
    OE_TR_DELETE_FOLDER(516),
    OE_TR_RENAME_FOLDER(544),
    OE_TR_MOVE_FOLDER(576),
    ;

    private final int value;
    tagOE_DATABASE_TRANSACTION(int value) { this.value=value; }
    public int comEnumValue() { return value; }
}
