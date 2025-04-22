#include "oeapi_wrapper.h"

HINSTANCE hInst = NULL;


//-----------------------------------------------------//
class MyOEFolderManager : public OEFolderManagerWrapper {
public:
	MyOEFolderManager(BOOL create=FALSE) : OEFolderManagerWrapper(create) {}

	MyOEFolderManager &operator=(const OEFolderManagerWrapper &obj) { OEFolderManagerWrapper::operator=(obj); return *this; }
	MyOEFolderManager &operator=(com_ptr<OESTORE::IOEFolderManager> &iface) { OEFolderManagerWrapper::operator=(iface); return *this; }
	MyOEFolderManager &operator=(int null) { OEFolderManagerWrapper::operator=(null); return *this; }

	void OnNewMessage(long folderId, long msgId)
	{
		OEFolderWrapper folder;
		OEMessageWrapper msg;

		folder = (*this)->GetFolder(folderId);
		if(!folder.is_null()) {
			msg = folder->GetMessage(msgId);
			if(msg) {
				char msgStr[1024];

				sprintf(msgStr, "NEW MESSAGE: %s.\n", msg->GetSubject().t_str().c_str());
				OutputDebugString(msgStr);
			}
		}
		//MessageBox(NULL, "Message selection changed", "OEAPIObj event", MB_OK);
	}
};

//-----------------------------------------------------//
class MyOEAPIObj : public OEAPIObjWrapper {
public:
	MyOEAPIObj(BOOL create=FALSE) : OEAPIObjWrapper(create) {}

	MyOEAPIObj &operator=(const OEAPIObjWrapper &obj) { OEAPIObjWrapper::operator=(obj); return *this; }
	MyOEAPIObj &operator=(com_ptr<OEAPI::IOEAPIObj> &iface) { OEAPIObjWrapper::operator=(iface); return *this; }
	MyOEAPIObj &operator=(int null) { OEAPIObjWrapper::operator=(null); return *this; }

	void OnMessageSelectionChanged()
	{
		//MessageBox(NULL, "Message selection changed", "OEAPIObj event", MB_OK);
	}

	void OnToolbarButtonClicked(long toolbarId, long buttonId)
	{
//		PostMessage((HWND) (*this)->GetOEMainWindow(), WM_COMMAND, 40109, 0);
		MessageBox(NULL, "Button Clicked", "OEAPI event", MB_OK);
	}
};

//-----------------------------------------------------//
class MyOEToolbar : public OEToolbarWrapper {
public:
	MyOEToolbar(BOOL create=FALSE) : OEToolbarWrapper(create) {}

	void OnButtonClicked(long buttonId)
	{
//		MessageBox(NULL, "Button Clicked", "OEAPI Toolbar event", MB_OK);
	}

	MyOEToolbar &operator=(const OEToolbarWrapper &obj) { OEToolbarWrapper::operator=(obj); return *this; }
	MyOEToolbar &operator=(com_ptr<OEAPI::IOEToolbar> &iface) { OEToolbarWrapper::operator=(iface); return *this; }
	MyOEToolbar &operator=(int null) { OEToolbarWrapper::operator=(null); return *this; }

private:
};


//-----------------------------------------------------//
class MyOEAPIInit : public OEAPIInitWrapper {
public:
	MyOEAPIInit(BOOL create=FALSE) : OEAPIInitWrapper(create) { }

	void OnInitOEAPI() 
	{
		m_oeapi.Create();
		MessageBox((HWND) m_oeapi->GetOEMainWindow(), "OEAPI Loading.", "OnInitOEAPI", MB_OK);
//		TCHAR filename[MAX_PATH];
//		std::string appPath, butPath;
//		std::string normal, over;
//
//		m_oeapi.Create();
//		m_toolbar = m_oeapi->CreateToolbar();
//		m_fm = NULL;
//		m_fm.Create();
//		m_fm->ActivateGlobalNotification();
//
//		if(GetModuleFileName(hInst, filename, MAX_PATH)) {
//			*_tcsrchr(filename, '\\') = 0;
//			butPath = filename;
//			normal = butPath + _T("\\images\\img_normal.bmp");
//			over = butPath + _T("\\images\\img_mouseover.bmp");
//			m_button = m_toolbar->CreateButton(_T("Nektra"), normal.c_str(), over.c_str());
//			m_button = m_toolbar->CreateButton(_T("Nektra"), normal.c_str(), over.c_str());
////			m_button->CreateSubButton(_T("Nektra"), normal.c_str(), over.c_str());
//		}
	}
	void OnShutdownOEAPI()
	{
		m_oeapi = NULL;
		m_toolbar = NULL;
		m_button = NULL;
	}

private:
	MyOEAPIObj m_oeapi;
	MyOEFolderManager m_fm;
	MyOEToolbar m_toolbar;
	OEButtonWrapper m_button;
};


MyOEAPIInit OeapiInit;

VOID WINAPI InitializePlugin()
{
//	MessageBox(NULL, "Initialize", "InitializePlugin Function", MB_OK);
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	switch(dwReason) {
	case DLL_PROCESS_ATTACH:
		{
			hInst = hInstance;
			OeapiInit.Create();
			break;
		}
	case DLL_PROCESS_DETACH:
		{
			break;
		}
	}

	return TRUE;
}


/*
#include "oeapi_wrapper.h"

HINSTANCE hInst = NULL;

std::list<OEMessageWrapper> MessageList;

//-----------------------------------------------------//
class MyOEAPIObj : public OEAPIObjWrapper {
public:
	MyOEAPIObj(BOOL create=FALSE) : OEAPIObjWrapper(create) {  }


	MyOEAPIObj &operator=(const OEAPIObjWrapper &obj) { OEAPIObjWrapper::operator=(obj); return *this; }
	MyOEAPIObj &operator=(com_ptr<OEAPI::IOEAPIObj> &iface) { OEAPIObjWrapper::operator=(iface); return *this; }
	MyOEAPIObj &operator=(int null) { OEAPIObjWrapper::operator=(null); return *this; }

};

//-----------------------------------------------------//
class MyOEToolbar : public OEToolbarWrapper {
public:
	MyOEToolbar(BOOL create=FALSE) : OEToolbarWrapper(create) {}

	void OnButtonClicked(long buttonId)
	{
		MessageBox(NULL, "Button Clicked", "OEAPI Toolbar event", MB_OK);
	}

	MyOEToolbar &operator=(const OEToolbarWrapper &obj) { OEToolbarWrapper::operator=(obj); return *this; }
	MyOEToolbar &operator=(com_ptr<OEAPI::IOEToolbar> &iface) { OEToolbarWrapper::operator=(iface); return *this; }
	MyOEToolbar &operator=(int null) { OEToolbarWrapper::operator=(null); return *this; }

private:
};


class MyOEButton2 : public OEButtonWrapper 
{
	OEFolderManagerWrapper m_fm;
public:
	MyOEButton2() : m_fm(TRUE) {}
	MyOEButton2 &operator=(const MyOEButton2 &obj) { OEButtonWrapper::operator=(obj); return *this; }
	MyOEButton2 &operator=(com_ptr<OEAPI::IOEButton> &iface) { OEButtonWrapper::operator=(iface); return *this; }
	MyOEButton2 &operator=(int null) { OEButtonWrapper::operator=(null); return *this; }
	void OnClick()
	{
//		MessageBox(NULL, "MyOEButton OnClick()", "MyOEButton OnClick()", MB_OK);

		try{
			OEFolderWrapper folder = m_fm->GetSentFolder();
//			MessageBox(NULL, folder->Name().t_str().c_str(), "Folder", MB_OK);

//			OEMessageWrapper m = folder->GetMessage(3430);

//			MessageBox(NULL, m->GetSubject().t_str().c_str(), "Subject", MB_OK);
			
			OEAPIObjWrapper oeapi(TRUE);
		
			oeapi->SendMessage(folder->GetID(), 3430);
			oeapi->SendAndReceiveMessages();

		}
		catch(const std::exception &e) {
			MessageBox(NULL, "Error!!!", "Error", MB_OK);			
		}

	}

};

//-----------------------------------------------------//
class MyOEAPIInit : public OEAPIInitWrapper {
public:
	MyOEAPIInit(BOOL create=FALSE) : OEAPIInitWrapper(create) { }

	void OnInitOEAPI() 
	{
		TCHAR filename[MAX_PATH];
		std::string appPath, butPath;
		std::string normal, over;

		m_oeapi.Create();
//		m_toolbar = m_oeapi->CreateToolbar();
		m_toolbar = m_oeapi->GetOEToolbar();

		if(GetModuleFileName(hInst, filename, MAX_PATH)) {
			*_tcsrchr(filename, '\\') = 0;
			butPath = filename;
			normal = butPath + _T("\\images\\img_normal.bmp");
			over = butPath + _T("\\images\\img_mouseover.bmp");
			m_button = m_toolbar->CreateButton(_T("Nektra"), normal.c_str(), over.c_str());
		}
	}
	void OnShutdownOEAPI()
	{
		m_oeapi = NULL;
		m_toolbar = NULL;
		m_button = NULL;
	}

private:
	OEAPIObjWrapper m_oeapi;
	OEToolbarWrapper m_toolbar;
	MyOEButton2 m_button;
};


MyOEAPIInit OeapiInit;


extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	switch(dwReason) {
	case DLL_PROCESS_ATTACH:
		{
			hInst = hInstance;
			OeapiInit.Create();
			break;
		}
	case DLL_PROCESS_DETACH:
		{
			break;
		}
	}

	return TRUE;
}
*/
/*
#include "oeapi_wrapper.h"

HINSTANCE hInst = NULL;

//-----------------------------------------------------//

class MyOEToolbar : public OEToolbarWrapper {
public:
	MyOEToolbar(BOOL create=FALSE) : OEToolbarWrapper(create) {}

	void OnButtonClicked(long buttonId)
	{
		MessageBox(NULL, "Button Clicked", "OEAPI Toolbar event", MB_OK);
	}

	MyOEToolbar &operator=(const OEToolbarWrapper &obj) { OEToolbarWrapper::operator=(obj); return *this; }
	MyOEToolbar &operator=(com_ptr<OEAPI::IOEToolbar> &iface) { OEToolbarWrapper::operator=(iface); return *this; }
	MyOEToolbar &operator=(int null) { OEToolbarWrapper::operator=(null); return *this; }

private:
};


class MyOEButton2 : public OEButtonWrapper 
{
	OEFolderManagerWrapper m_fm;
public:
	MyOEButton2() : m_fm(TRUE) {};
	MyOEButton2 &operator=(const MyOEButton2 &obj) { OEButtonWrapper::operator=(obj); return *this; }
	MyOEButton2 &operator=(com_ptr<OEAPI::IOEButton> &iface) { OEButtonWrapper::operator=(iface); return *this; }
	MyOEButton2 &operator=(int null) { OEButtonWrapper::operator=(null); return *this; }
	void OnClick()
	{
//		MessageBox(NULL, "MyOEButton OnClick()", "MyOEButton OnClick()", MB_OK);

		try{
			OEFolderWrapper folder = m_fm->GetDeletedFolder();
//			MessageBox(NULL, folder->Name().t_str().c_str(), "Folder", MB_OK);

			bstr_t source;
			source+="From: gliu317@swbell.net\r\n";
			source+="To: xilong@swbell.net\r\n";
			source+="Subject: test subject\r\n";
			source+="\r\n";
			source+="This is a test.\r\n";

			OEMessageWrapper m = folder->CreateMessage(source);
			
			OEAPIObjWrapper oeapi(TRUE);
		
			oeapi->SendMessage(m->GetFolderID(), m->GetID());
			oeapi->SendAndReceiveMessages();

			m->Delete(TRUE);
		}
		catch(const std::exception &) {
			MessageBox(NULL, "Error!!!", "Error", MB_OK);			
		}
	}
};

//-----------------------------------------------------//
class MyOEAPIInit : public OEAPIInitWrapper {
public:
	MyOEAPIInit(BOOL create=FALSE) : OEAPIInitWrapper(create) { }

	void OnInitOEAPI() 
	{
		TCHAR filename[MAX_PATH];
		std::string appPath, butPath;
		std::string normal, over;

		OutputDebugString("OnInitOEAPI\n");
		m_oeapi.Create();
		OutputDebugString("Create\n");
//		m_toolbar = m_oeapi->CreateToolbar();
		m_toolbar = m_oeapi->GetOEToolbar();
		OutputDebugString("GetOEToolbar\n");

		if(GetModuleFileName(hInst, filename, MAX_PATH)) {
			OutputDebugString("GetModuleFileName\n");
			*_tcsrchr(filename, '\\') = 0;
			OutputDebugString("_tcsrchr\n");
			butPath = filename;
			normal = butPath + _T("\\images\\img_normal.bmp");
			over = butPath + _T("\\images\\img_mouseover.bmp");
			OutputDebugString("CreateButton 1\n");
			m_button = m_toolbar->CreateButton(_T("Nektra"), normal.c_str(), over.c_str());
			m_button->CreateSubButton(_T("Sub Nektra"), normal.c_str(), over.c_str());

			m_toolbar->CreateSeparator();
			m_toolbar->CreateButton(_T("Nektra"), normal.c_str(), over.c_str());
			OutputDebugString("CreateButton 2\n");
		}
	}
	void OnShutdownOEAPI()
	{
		OutputDebugString("OnShutdownOEAPI\n");
		m_oeapi = NULL;
		OutputDebugString("OnShutdownOEAPI m_oeapi\n");
		m_toolbar = NULL;
		OutputDebugString("OnShutdownOEAPI m_toolbar\n");
		m_button = NULL;
		OutputDebugString("OnShutdownOEAPI m_button\n");
	}

private:
	OEAPIObjWrapper m_oeapi;
	OEToolbarWrapper m_toolbar;
	MyOEButton2 m_button;
};


MyOEAPIInit OeapiInit;


extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	switch(dwReason) {
	case DLL_PROCESS_ATTACH:
		{
			hInst = hInstance;
			OeapiInit.Create();
			break;
		}
	case DLL_PROCESS_DETACH:
		{
			break;
		}
	}

	return TRUE;
}
*/
