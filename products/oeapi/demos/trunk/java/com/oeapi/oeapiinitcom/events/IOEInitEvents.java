package com.oeapi.oeapiinitcom.events;

import com4j.*;

/**
 * IOEInitEvents Interface
 */
@IID("{12BAF050-264C-464B-9D58-C83B3781DD4B}")
public abstract class IOEInitEvents {
    /**
     * Event triggered when the OEAPI initializes.
     */
    @DISPID(1)
    public void onInitOEAPI() {
            throw new UnsupportedOperationException();
    }

    /**
     * Event triggered when the OEAPI shutdowns.
     */
    @DISPID(2)
    public void onShutdownOEAPI() {
            throw new UnsupportedOperationException();
    }

}
