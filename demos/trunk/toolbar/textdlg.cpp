#include "stdafx.h"
#include "textdlg.h"


LRESULT CTextDialog::OnInitDialog( UINT msg, WPARAM wparam, LPARAM lparam, BOOL bHandle )
{
	SetWindowText(m_title.c_str());
	
	SendMessage(GetDlgItem(IDC_TEXTEDIT), WM_SETTEXT, 0, (LPARAM)m_text.c_str());
	
	return 0L;
}

LRESULT CTextDialog::OnClose( UINT msg, WPARAM wparam, LPARAM lparam, BOOL bHandle )
{
	EndDialog(0);
	return 0L;
}
