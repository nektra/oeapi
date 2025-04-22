package com.oeapi.oeapiinitcom  ;

import com4j.*;

/**
 * IOEIdentity interface
 */
@IID("{12BAF072-264C-464B-9D58-C83B3781DD4B}")
public interface IOEIdentity extends Com4jObject {
    /**
     * Get current identity. Useful to verify if the identity has changed.
     */
    @VTID(7)
    java.lang.String getCurrentIdentity();

}
