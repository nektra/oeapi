package com.oeapi.oestore  ;

import com4j.*;

public enum tagOEAPIMSGSTATE implements ComEnum {
    OE_MSG_DELETED(1),
    OE_MSG_UNREAD(2),
    OE_MSG_SUBMITTED(4),
    OE_MSG_UNSENT(8),
    OE_MSG_RECEIVED(16),
    OE_MSG_NEWSMSG(32),
    OE_MSG_NOSECUI(64),
    OE_MSG_VOICEMAIL(128),
    OE_MSG_REPLIED(256),
    OE_MSG_FORWARDED(512),
    OE_MSG_RCPTSENT(1024),
    OE_MSG_FLAGGED(2048),
    OE_MSG_LAST(512),
    OE_MSG_EXTERNAL_FLAGS(254),
    OE_MSG_FLAGS(15),
    ;

    private final int value;
    tagOEAPIMSGSTATE(int value) { this.value=value; }
    public int comEnumValue() { return value; }
}
