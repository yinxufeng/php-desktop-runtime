/**
 * php_dhtml
 * Copyright (C) 2004-2005  Eric Colinet <ecolinet - gmail - com>
 * http://wildphp.free.fr/wiki/doku.php?id=php_dhtml:index
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#if !defined(AFX_DHTMLMFCTHREAD_H__58789E43_F154_42C0_9A91_7FF17AF4F93B__INCLUDED_)
#define AFX_DHTMLMFCTHREAD_H__58789E43_F154_42C0_9A91_7FF17AF4F93B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dhtml_api.h"
#include "CHtmlExDlg.h"

#define VART2STR( variant ) CString(COleVariant(variant).bstrVal)


class DHtmlMfcThread : public CWinThread
{
	DECLARE_DYNCREATE(DHtmlMfcThread)

public:
	DWORD m_parent_thread_id;
	HANDLE m_sync_event;
	CHtmlExDlg       *m_dlg;
	
public:
	DHtmlMfcThread();
	virtual ~DHtmlMfcThread();

	int IsVisible(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DHtmlMfcThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DHtmlMfcThread)
	afx_msg void OnPropertyHandler( WPARAM thId, LPARAM value );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DHTMLMFCTHREAD_H__58789E43_F154_42C0_9A91_7FF17AF4F93B__INCLUDED_)
