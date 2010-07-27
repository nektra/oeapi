#ifndef __OEAPI_MSGWND_STYLES__
#define __OEAPI_MSGWND_STYLES__ 1


typedef enum tagWNDSTYLE {
	OE_MSG_INVALID = 0,
	/**
	Window opened when the user dbl clicks a message.
	*/
	OE_MSG_DETAIL_WND = 0x0001,
	/**
	Windows opened when the user press 'Create Mail' / Forward / Reply / Reply All.
	*/
	OE_MSG_SEND_WND = 0x0002,
	/**
	Window displayed in the bottom part of OE that shows the current message in OE main window.
	*/
	OE_CURRENT_MSG_WND = 0x0004,
	/**
	Window displayed in the bottom part of OE that shows a text that says that there is no message selected.
	*/
	OE_EMPTY_MSG_WND = 0x0008,
	/**
	Window displayed when OE root folder is selected.
	*/
	OE_OTHER_WND = 0x0010,
	/**
	Window displayed when OE root folder is selected.
	*/
	OE_MAIN_WND = 0x0020,
	/**
	All message windows.
	*/
	OE_ALL_MSG_WND = OE_MSG_DETAIL_WND | OE_CURRENT_MSG_WND | OE_MSG_SEND_WND | OE_OTHER_WND
} WNDSTYLE;



#endif // __OEAPI_MSGWND_STYLES__
