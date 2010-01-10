/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 6/1/98 4:50:35 PM
  Comments: MyFD.cpp : implementation file
 ************************************/

#include "stdafx.h"
#include "FileDialogEx.h"


CFileDialogEx::CFileDialogEx(
		BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt ,
		LPCTSTR lpszFileName ,
		DWORD dwFlags,
		LPCTSTR lpszFilter,
		CWnd* pParentWnd ,
		DWORD dwSize ,
		LPCTSTR psTitle )
	: CFileDialog(bOpenFileDialog,lpszDefExt,lpszFileName,dwFlags,lpszFilter,pParentWnd,dwSize) 
	//, m_psTitle(psTitle)
{}

void CFileDialogEx::OnInitDone()
{
	if(m_psTitle)
	{
		GetParent()->SetWindowText(m_psTitle) ;
	}

	CFileDialog::OnInitDone() ;
}
