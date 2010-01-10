/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 6/1/98 4:50:35 PM
  Comments: MyFD.cpp : implementation file
 ************************************/

#include "stdafx.h"
#include "FolderDialog.h"


/////////////////////////////////////////////////////////////////////////////
// CFolderDialog

IMPLEMENT_DYNAMIC(CFolderDialog, CFileDialog)

WNDPROC CFolderDialog::m_wndProc = NULL;


// Function name	: CFolderDialog::CFolderDialog
// Description	    : Constructor
// Return type		: 
// Argument         : CString* pPath ; represent string where selected folder wil be saved
CFolderDialog::CFolderDialog(CString* pRetPath,LPCTSTR psPath,LPCTSTR psTitle)
: CFileDialog(TRUE, "_____|....____[#@$%^]_____||", psPath)
	, m_psTitle(psTitle)
	, m_pPath(pRetPath)
{}


BEGIN_MESSAGE_MAP(CFolderDialog, CFileDialog)
	//{{AFX_MSG_MAP(CFolderDialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Function name	: WindowProcNew
// Description	    : Call this function when user navigate into CFileDialog.
// Return type		: LRESULT
// Argument         : HWND hwnd
// Argument         : UINT message
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
LRESULT CALLBACK WindowProcNew(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	
	if (message ==  WM_COMMAND)
		if (HIWORD(wParam) == BN_CLICKED)
			if (LOWORD(wParam) == IDOK)
			{
				if (CFileDialog* pDlg = (CFileDialog*)CWnd::FromHandle(hwnd))
				{
					TCHAR path[MAX_PATH];
					GetCurrentDirectory(MAX_PATH, path);
					*((CFolderDialog*)pDlg->GetDlgItem(0))->m_pPath = CString(path);
					pDlg->EndDialog(IDOK);
					return NULL;
				}
			}
	return CallWindowProc(CFolderDialog::m_wndProc, hwnd, message, wParam, lParam);
}

// Function name	: CFolderDialog::OnInitDone
// Description	    : For update the wiew of CFileDialog
// Return type		: void 
void CFolderDialog::OnInitDone()
{
	HideControl(edt1);
	HideControl(stc3);
	HideControl(cmb1);
	HideControl(stc2);
	HideControl(0x47C) ;

	CWnd* pFD = GetParent();
	CRect rectCancel; pFD->GetDlgItem(IDCANCEL)->GetWindowRect(rectCancel);
	pFD->ScreenToClient(rectCancel);
	CRect rectOK; pFD->GetDlgItem(IDOK)->GetWindowRect(rectOK);
	pFD->ScreenToClient(rectOK);
	pFD->GetDlgItem(IDOK)->SetWindowPos(0,rectCancel.left - rectOK.Width() - 4, rectCancel.top, 0,0, SWP_NOZORDER | SWP_NOSIZE);
	CRect rectList2; pFD->GetDlgItem(lst1)->GetWindowRect(rectList2);
	pFD->ScreenToClient(rectList2);
	int nHeight = rectList2.top - (rectCancel.top - 4) ;
	if(nHeight<0)
	{
		nHeight = 0-nHeight ;
	}
	pFD->GetDlgItem(lst1)->SetWindowPos(0,0,0,rectList2.Width(), nHeight, SWP_NOMOVE | SWP_NOZORDER);
	
	m_wndProc = (WNDPROC)SetWindowLong(pFD->m_hWnd, GWL_WNDPROC, (long)WindowProcNew);

	if(m_psTitle)
	{
		pFD->SetWindowText(m_psTitle) ;
	}
}
