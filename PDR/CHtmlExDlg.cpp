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
#include "dhtml_api.h"
#include "CHtmlExDlg.h"
#include "DHtmlMfcThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PARAM_EQ( a, b )  (a && strcmp(a,b)==0)
#define PARAM_EQn( a, b, n )  (a && strncmp(a,b,n)==0)
#define PARAM_EQpn( a, b, p, n )  (a && strncmp(a+p,b,n)==0)
#define PARAM_EQp( a, b, p )  (a && strcmp(a+p,b)==0)

BEGIN_EVENTSINK_MAP(CHtmlExDlg, CDHtmlDialog)
	ON_EVENT(CHtmlExDlg, AFX_IDC_BROWSER, 250 /* BeforeNavigate2 */, _OnBeforeNavigate2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

/*
BEGIN_DHTML_EVENT_MAP(CHtmlExDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()
*/

const DHtmlEventMapEntry* CHtmlExDlg::GetDHtmlEventMap() {
	return m_dynMap.m_eventMap;
}

CHtmlExDlg::CHtmlExDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CHtmlExDlg::IDD, CHtmlExDlg::IDH, pParent)
	, m_url(_T(""))
{
	m_allowPostEvent= FALSE;
	m_allowNavigate= FALSE;
	m_cb_post_event= NULL;

	if( m_iconfile.IsEmpty() )
	{
		// m_hIcon = AfxGetApp()->LoadIcon(IDR_DHTML);
	}
	else {
		m_hIcon= (HICON)LoadImage(NULL, m_iconfile, IMAGE_ICON, 
			GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 
			LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_SHARED);

		m_hIcon = AfxGetApp()->LoadIcon(m_iconfile);
	}
	m_hostFlags= DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_SCROLL_NO;
}


void CHtmlExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHtmlExDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_WIN32STD_TRAY_ICON, onTrayIcon)
END_MESSAGE_MAP()


BOOL CHtmlExDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
/*
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu("About");
		//strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
*/
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	/* Load default page */
	m_allowPostEvent= TRUE;
	PostEvent("init");

	return TRUE;  // retourne TRUE, sauf si vous avez défini le focus sur un contrôle
}


void CHtmlExDlg::PostEvent(const char *type, const char *value, const char *extra) {
	if( m_allowPostEvent )
		::dhtml_post_message(((DHtmlMfcThread*)AfxGetThread())->m_parent_thread_id, type, value, extra, NULL);
    if( m_allowPostEvent && this->m_cb_post_event )
		this->m_cb_post_event((long)this, type, value, extra, this->m_cb_data);
}


void CHtmlExDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	/*if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.m_text= "Exemple text";
		dlgAbout.m_icon= m_hIcon;
		dlgAbout.DoModal();
	}
	else*/
		CDHtmlDialog::OnSysCommand(nID, lParam);
}


void CHtmlExDlg::OnPaint() 
{
	if (IsIconic()) {
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDHtmlDialog::OnPaint();
	}
}


HCURSOR CHtmlExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/* CHtmlExDlg::PropertyHandler
	All possibles actions are handled here
*/
#define HEDSP_RESULT(value)        { *return_value= strdup(value); }
int CHtmlExDlg::PropertyHandler(const char *param, const char *value, const char *extra, char **return_value)
{
	/* Window */
	if( PARAM_EQ( param, "show" ) ) {
		if( return_value ) return 0;
		
		ShowWindow( SW_SHOW );
		SetForegroundWindow();
		return 1;
	}
	else if( PARAM_EQ( param, "hide" ) ) {
		if( return_value ) return 0;
		
		ShowWindow( SW_HIDE );
		return 1;
	}
	else if( PARAM_EQ( param, "focus" ) ) {
		if( return_value ) return 0;
		
		SetForegroundWindow();
		return 1;
	}
	else if( PARAM_EQ( param, "minimize" ) ) {
		if( return_value ) return 0;
		
		ShowWindow( SW_MINIMIZE );
		return 1;
	}
	else if( PARAM_EQ( param, "maximize" ) ) {
		if( return_value ) return 0;
		
		ShowWindow( SW_MAXIMIZE );
		return 1;
	}
	else if( PARAM_EQ( param, "restore" ) ) {
		if( return_value ) return 0;
		
		ShowWindow( SW_RESTORE );
		return 1;
	}
	else if( PARAM_EQ( param, "enable") ) {
		if( return_value ) return 0;
		this->EnableWindow(1);
		return 1;
	}
	else if( PARAM_EQ( param, "disable") ) {
		if( return_value ) return 0;
		this->EnableWindow(0);
		return 1;
	}
	else if( PARAM_EQ( param, "title" ) ) {
		if( return_value ) {
			CString title;
			GetWindowText(title);
			HEDSP_RESULT(title)
		}
		else
			SetWindowText( value );
		return 1;
	}
	else if( PARAM_EQ( param, "size" ) ) {
		int cx, cy;
		CRect r;

		if( return_value ) return 0;
		
		GetWindowRect(&r);
		if( sscanf( value, "%dx%d", &cx, &cy )==2 )
		{
			MoveWindow( r.left, r.top, cx, cy, TRUE );
			return 1;
		}
	}
	else if( PARAM_EQ( param, "move" ) ) {
		int cx, cy;
		CRect r;
		
		if( return_value ) return 0;

		GetWindowRect(&r);
		if( sscanf( value, "%dx%d", &cx, &cy )==2 )
		{
			SetWindowPos( NULL, cx, cy, 0, 0, SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER );
			return 1;
		}
	}
	else if( PARAM_EQ( param, "close" ) ) {
		if( return_value ) return 0;
		this->DestroyWindow();
		return 1;
	}
	else if( PARAM_EQ( param, "timer" ) ) {
		if( return_value ) return 0;
		if( atoi(extra) )
			SetTimer( atoi(value), atoi(extra), NULL );
		else
			KillTimer( atoi(value) );
		return 1;
	}
	/* DHTML 	*/
	else if( PARAM_EQ( param, "load_default" ) ) {
		if( return_value ) return 0;
		this->LoadPage("res://php_dhtml.dll/DEFAULT_HTML_TEMPLATE", value);
		return 1;
	}
	else  if( PARAM_EQ( param, "url" ) ) {
		if( return_value )
			HEDSP_RESULT(m_url)
        else if( value && *value ) this->LoadPage( value, extra );
        return 1;
    }
    else if( PARAM_EQ( param, "reload" ) ) {
		if( return_value ) return 0;

        this->LoadPage( m_url, extra );
        return 1;
    }
	else if( PARAM_EQ( param, "3d_look" ) ) {
		if( return_value ) return 0;

		if( PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			m_hostFlags|= DOCHOSTUIFLAG_NO3DBORDER;
		else
			m_hostFlags&= ~DOCHOSTUIFLAG_NO3DBORDER;
		this->SetHostFlags(m_hostFlags);
        return 1;
	}
	else if( PARAM_EQ( param, "caption" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)&~WS_CAPTION);
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)|WS_CAPTION);
		}
	}
	else if( PARAM_EQ( param, "border" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)&~WS_BORDER );
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)|WS_BORDER );
		}
	}
	else if( PARAM_EQ( param, "maximizebox" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)&~WS_MAXIMIZEBOX);
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)|WS_MAXIMIZEBOX);
		}
	}
	else if( PARAM_EQ( param, "minimizebox" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)&~WS_MINIMIZEBOX);
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)|WS_MINIMIZEBOX);
		}
	}
	else if( PARAM_EQ( param, "sysmenu" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)&~WS_SYSMENU);
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)|WS_SYSMENU);
		}
	}
	else if( PARAM_EQ( param, "sizebox" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)&~WS_SIZEBOX );
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)|WS_SIZEBOX );
		}
	}
	else if( PARAM_EQ( param, "dlgframe" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)&~WS_DLGFRAME );
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE)|WS_DLGFRAME );
		}
	}
	else if( PARAM_EQ( param, "appwindow" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE)&~WS_EX_APPWINDOW);
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE)|WS_EX_APPWINDOW);
		}
	}
	else if( PARAM_EQ( param, "clientedge" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE)&~WS_EX_CLIENTEDGE);
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE)|WS_EX_CLIENTEDGE);
		}
	}
	else if( PARAM_EQ( param, "toolwindow" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE)&~ WS_EX_TOOLWINDOW);
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE)|WS_EX_TOOLWINDOW );
		}
	}
	else if( PARAM_EQ( param, "topmost" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") )
			::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else if( PARAM_EQ( param, "windowedge" ) ) {
		if( return_value ) return 0;
		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") )
			SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE)&~WS_EX_WINDOWEDGE);
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE)|WS_EX_WINDOWEDGE);
		}
	}
	else if( PARAM_EQ( param, "redraw" ) ) {
		if( return_value ) return 0;
		::SetWindowPos(this->m_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	else if( PARAM_EQ( param, "scrollbar" ) ) {
		if( return_value ) return 0;

		if( !value || PARAM_EQ(value, "0") || PARAM_EQ(value, "off") ) {
			m_hostFlags|= DOCHOSTUIFLAG_SCROLL_NO;
		}
		else if( PARAM_EQ(value, "on") || PARAM_EQ(value, "1") ) {
			m_hostFlags&= ~DOCHOSTUIFLAG_SCROLL_NO;
			m_hostFlags&= ~DOCHOSTUIFLAG_FLAT_SCROLLBAR;
		}
		else if( PARAM_EQ(value, "flat") ) {
			m_hostFlags&= ~DOCHOSTUIFLAG_SCROLL_NO;
			m_hostFlags|= DOCHOSTUIFLAG_FLAT_SCROLLBAR;
		}
		this->SetHostFlags(m_hostFlags);
        return 1;
	}
	else if( PARAM_EQpn(param, "event_", 0, 6) ) { // Handles event_*
		if( return_value ) return 0;
		if( !value || !*value ) return 0;
		int apply_to_class= PARAM_EQ(extra, "1")||PARAM_EQ(extra, "class")||PARAM_EQ(extra, "on");
/* Mouse clicks events */
		if( PARAM_EQ( param, "event_click" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::CLICK, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));

		else if( PARAM_EQ( param, "event_dblclick" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::DBLCLICK, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));

		else if( PARAM_EQ( param, "event_ctxmenu" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::CTXMENU, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));
/* Key events */
		else if( PARAM_EQ( param, "event_keypress" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::KEYPRESS, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));

		else if( PARAM_EQ( param, "event_keydown" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::KEYDOWN, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));

		else if( PARAM_EQ( param, "event_keyup" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::KEYUP, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));
/* Mouse move events */
		else if( PARAM_EQ( param, "event_mouseout" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::MOUSEOUT, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));

		else if( PARAM_EQ( param, "event_mouseover" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::MOUSEOVER, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));

		else if( PARAM_EQ( param, "event_mousewheel" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::MOUSEWHEEL, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));
/* Focus events */
		else if( PARAM_EQ( param, "event_focusin" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::FOCUSIN, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));

		else if( PARAM_EQ( param, "event_focusout" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::FOCUSOUT, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));
/*
		else if( PARAM_EQ( param, "event_" ) ) 
			m_dynMap.Add(
				apply_to_class?CDynDHtmlEventMap::ELEM::CLASS:CDynDHtmlEventMap::ELEM::NAME, 
				CDynDHtmlEventMap::TYPE::, value, 
				CDYNDHEM_FUNC(&CHtmlExDlg::OnHtmlEvent));
				*/
		else return 0;
        return 1;
    }
	else if( PARAM_EQpn(param, "id_", 0, 3) ) { // Handles (get/set)_id_*
        BOOL get= (return_value!=NULL?TRUE:FALSE);
        DISPID dispid;
        CString result;
        CDataExchange de(this, get);

        if( PARAM_EQp( param, "value", 3 ) ) dispid= DISPID_A_VALUE;
        else if( PARAM_EQp( param, "text", 3 ) ) dispid= DISPID_IHTMLELEMENT_INNERTEXT;
        else if( PARAM_EQp( param, "html", 3 ) ) dispid= DISPID_IHTMLELEMENT_INNERHTML;
        else if( PARAM_EQp( param, "outer_html", 3 ) ) dispid= DISPID_IHTMLELEMENT_OUTERHTML;
        else if( PARAM_EQp( param, "a_href", 3 ) ) dispid= DISPID_IHTMLANCHORELEMENT_HREF;
        else if( PARAM_EQp( param, "a_target", 3 ) ) dispid= DISPID_IHTMLANCHORELEMENT_TARGET;
        else if( PARAM_EQp( param, "img_src", 3 ) ) dispid= DISPID_IHTMLIMGELEMENT_SRC;
        else if( PARAM_EQp( param, "frame_src", 3 ) ) dispid= DISPID_IHTMLFRAMEBASE_SRC;
        else return 0;
        if( !get )
            result= extra;
        CDHtmlDialog::DDX_DHtml_ElementText(&de, value, dispid, result);
		if( get )
				HEDSP_RESULT(result)
        return 1;
    }

    return 0;
}


HRESULT CHtmlExDlg::OnHtmlEvent( IHTMLElement *pElement )
{
	IHTMLEventObj * pEvent;
	CComBSTR bs_id, bs_type;
	
	if( GetEvent( &pEvent )!=S_OK ) {
		return S_OK;
	}

	pElement->get_id(&bs_id);
	pEvent->get_type(&bs_type);
	
    PostEvent( CString("event_")+CString(bs_type), CString(bs_id), NULL );

	return S_OK;
}


void CHtmlExDlg::OnOK()
{
	PostEvent( "event_ok", NULL, NULL );
}


void CHtmlExDlg::OnCancel() {
	PostEvent( "event_cancel", NULL, NULL );
}


void CHtmlExDlg::OnClose() {
	PostEvent( "event_close", NULL, NULL );
}


void CHtmlExDlg::LoadPage( const char * page, const char * targetFrame ) {
	m_allowNavigate= TRUE;
	this->Navigate(page, navNoHistory, targetFrame);
	m_url= page;
}


void CHtmlExDlg::_OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel) {
	Flags; // unused
	TargetFrameName; // unused
	PostData; // unused
	Headers; // unused
	Cancel; // unused

    COleVariant var_url(URL), var_target(TargetFrameName);
    CString url(var_url), target(var_target);

    if( !m_allowNavigate )
    {
        PostEvent( "request", url, target );
        *Cancel= TRUE;
        return;
    }

	CString str(V_BSTR(URL));
	OnBeforeNavigate(pDisp, str);
	m_allowNavigate= FALSE;

    //CDHtmlDialog::_OnBeforeNavigate2(pDisp, URL, Flags, TargetFrameName, PostData, Headers, Cancel );
}


void CHtmlExDlg::OnBeforeNavigate(LPDISPATCH pDisp, LPCTSTR szUrl) {
	if( !m_url.IsEmpty() )
		PostEvent("page_unload");
    CDHtmlDialog::OnBeforeNavigate(pDisp, szUrl);
}


void CHtmlExDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl) {
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);
	PostEvent("page_load", szUrl);
}


void CHtmlExDlg::OnTimer(UINT nIDEvent)  {
	char buffer[255];
	PostEvent("timer",itoa(nIDEvent,buffer,255));
	CDialog::OnTimer(nIDEvent);
}

HRESULT STDMETHODCALLTYPE CHtmlExDlg::ShowContextMenu(DWORD dwID,
   POINT * ppt,
   IUnknown * pcmdtReserved,
   IDispatch * pdispReserved 
) { return S_OK; }


LRESULT CHtmlExDlg::onTrayIcon(WPARAM id, LPARAM message) {
	char buffer[255];
	char *which;
	switch( message ) {
		case WM_LBUTTONDOWN: which= "LBUTTONDOWN"; break;
		case WM_LBUTTONUP: which= "LBUTTONUP"; break;
		case WM_LBUTTONDBLCLK: which= "LBUTTONDBLCLK"; break;
		case WM_RBUTTONDOWN: which= "RBUTTONDOWN"; break;
		case WM_RBUTTONUP: which= "RBUTTONUP"; break;
		case WM_RBUTTONDBLCLK: which= "RBUTTONDBLCLK"; break;
		case WM_MBUTTONDOWN: which= "MBUTTONDOWN"; break;
		case WM_MBUTTONUP: which= "MBUTTONUP"; break;
		case WM_MBUTTONDBLCLK: which= "MBUTTONDBLCLK"; break;
		default:
		case WM_MOUSEMOVE: which= "MOUSEMOVE"; return 0; // Too verbose - slow down things
	}
	PostEvent("tray_event", itoa(id, buffer, 255), which);
	return 0;
}
