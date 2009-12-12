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

#include "stdafx.h"
#include "DHtmlMfcThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(DHtmlMfcThread, CWinThread)

DHtmlMfcThread::DHtmlMfcThread() : CWinThread() {m_sync_event=NULL; m_parent_thread_id=0; m_dlg=NULL;}
DHtmlMfcThread::~DHtmlMfcThread() {}

BOOL DHtmlMfcThread::InitInstance()
{
	AfxEnableControlContainer();
	CoInitialize(NULL);

	m_dlg= new CHtmlExDlg;
	if( !m_dlg->Create(IDR_HTML_DHTMLDLG, NULL) ) {
		delete m_dlg;
		return FALSE;
	}
    
	this->m_pMainWnd= m_dlg;
	
	if( m_sync_event ) SetEvent( m_sync_event );

    return TRUE;
}

int DHtmlMfcThread::ExitInstance() {
	m_dlg->DestroyWindow();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(DHtmlMfcThread, CWinThread)
	//{{AFX_MSG_MAP(DHtmlMfcThread)
	ON_THREAD_MESSAGE(WM_DHTML_EVENT,OnPropertyHandler)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int DHtmlMfcThread::IsVisible(void)
{
    // See if window exist and is shown
    if( m_pMainWnd && m_pMainWnd->GetSafeHwnd() && m_pMainWnd->GetStyle()&WS_VISIBLE )
        return FALSE;
    return TRUE;
}


void DHtmlMfcThread::OnPropertyHandler( WPARAM thId, LPARAM value )
{
	int ret;
	dhtml_event * event= (dhtml_event *)value;
	int get= event->result!=NULL;

	TRACE( "PropertyHandler: %s=%s %s\n", event->type, event->value, event->extra );
	
	if( event->type && strcmp(event->type, "quit")==0 ) { // Emergency Quit
		PostQuitMessage(atoi(event->value));
		ret= 1;
	}
	else {
		ret= m_dlg->PropertyHandler( event->type, event->value, event->extra, event->result );
	}

	if( event->result )
		::PostThreadMessage(m_parent_thread_id, WM_DHTML_EVENT_ACK, m_nThreadID, ret );

	dhtml_free_event(event);
}

