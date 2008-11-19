package oeapi;

import com.oeapi.oeapiinitcom.events.IOEInitEvents;

public class MyEvents2 extends IOEInitEvents {

	public MyEvents2() {
		// TODO Auto-generated constructor stub
	}
	
	public void onInitOEAPI() {
        System.out.println("oe Loaded");
    }
	public void onShutdownOEAPI() {
        System.out.println("oe shutdown");
    }
}
