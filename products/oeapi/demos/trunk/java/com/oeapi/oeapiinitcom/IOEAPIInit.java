package com.oeapi.oeapiinitcom  ;

import com4j.*;

/**
 * IOEAPIInit interface
 */
@IID("{12BAF051-264C-464B-9D58-C83B3781DD4B}")
public interface IOEAPIInit extends Com4jObject {
    /**
     * Returns TRUE if OEAPI was loaded.
     */
    @VTID(7)
    int isOEAPIActive();

}
