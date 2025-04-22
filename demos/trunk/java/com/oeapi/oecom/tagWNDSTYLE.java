package com.oeapi.oecom  ;

import com4j.*;

public enum tagWNDSTYLE implements ComEnum {
    OE_MSG_INVALID(0),
    OE_MSG_DETAIL_WND(1),
    OE_MSG_SEND_WND(2),
    OE_CURRENT_MSG_WND(4),
    OE_EMPTY_MSG_WND(8),
    OE_OTHER_WND(16),
    OE_ALL_MSG_WND(23),
    ;

    private final int value;
    tagWNDSTYLE(int value) { this.value=value; }
    public int comEnumValue() { return value; }
}
