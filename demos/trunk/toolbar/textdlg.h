#pragma  once

#include <atlwin.h>
#include <string>

class CTextDialog : public CDialogImpl<CTextDialog>
{
	std::wstring m_title;
	std::wstring m_text;

public:

	CTextDialog(const std::wstring& wndTitle, const std::wstring& text) : CDialogImpl<CTextDialog>()
	{
		m_title = wndTitle;
		m_text = text;
	}

	enum { IDD = IDD_TEXTDISPLAY };

	BEGIN_MSG_MAP(CTextDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose);
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT msg, WPARAM wparam, LPARAM lparam, BOOL bHandle);
	LRESULT OnClose(UINT msg, WPARAM wparam, LPARAM lparam, BOOL bHandle);
};