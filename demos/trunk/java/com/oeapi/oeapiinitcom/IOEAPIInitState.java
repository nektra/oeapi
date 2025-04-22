package com.oeapi.oeapiinitcom  ;

import com4j.*;

/**
 * IOEAPIInitState interface
 */
@IID("{12BAF051-264C-464B-9D58-C83B3781DD4D}")
public interface IOEAPIInitState extends Com4jObject {
    /**
     * Returns the number of instances.
     */
    @VTID(7)
    int getOEAPIInitObjectsCount();

    /**
     * Returns the number of objects that finishes the OnOEAPIInit event.
     */
    @VTID(8)
    int getOEAPIInitInitializedObjectsCount();

    /**
     * Resets the counter.
     */
    @VTID(9)
    void resetOEAPIInitInitializedObjectsCount();

}
