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

#pragma once

#include "resource.h"
#include "dhtml_api.h"

  //
 // CDynDHtmlEventMap Def
//
#define CDYNDHEM_FUNC(f) ((DHEVTFUNCCONTROL) (DHEVTFUNC)f)

class CDynDHtmlEventMap
{
public:
	DHtmlEventMapEntry * m_eventMap;
	long m_size;

	enum ELEM { NAME=DHTMLEVENTMAPENTRY_NAME, CLASS=DHTMLEVENTMAPENTRY_CLASS, END=DHTMLEVENTMAPENTRY_END };
	enum TYPE { 
		CLICK=DISPID_HTMLELEMENTEVENTS_ONCLICK,
		DBLCLICK=DISPID_HTMLELEMENTEVENTS_ONDBLCLICK,
		CTXMENU=DISPID_HTMLELEMENTEVENTS_ONCONTEXTMENU,
		
		KEYPRESS=DISPID_HTMLELEMENTEVENTS_ONKEYPRESS,
		KEYDOWN=DISPID_HTMLELEMENTEVENTS_ONKEYDOWN,
		KEYUP=DISPID_HTMLELEMENTEVENTS_ONKEYUP,

		MOUSEOUT=DISPID_HTMLELEMENTEVENTS_ONMOUSEOUT,
		MOUSEOVER=DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER,
		//MOUSEMOVE=DISPID_HTMLELEMENTEVENTS_ONMOUSEMOVE,
		//MOUSEDOWN=DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN,
		//MOUSEUP=DISPID_HTMLELEMENTEVENTS_ONMOUSEUP,
		MOUSEWHEEL=DISPID_HTMLELEMENTEVENTS_ONMOUSEWHEEL,
		
		FOCUSIN= DISPID_HTMLELEMENTEVENTS_ONFOCUSIN,
		FOCUSOUT= DISPID_HTMLELEMENTEVENTS_ONFOCUSOUT
	};

	CDynDHtmlEventMap()  {
		m_eventMap= NULL;
		m_size= -1;
		_bigger();
		m_eventMap[0].nType= DHTMLEVENTMAPENTRY_END;
		m_eventMap[0].dispId= 0;
		m_eventMap[0].szName= NULL;
		m_eventMap[0].pfnEventFunc= (DHEVTFUNCCONTROL) NULL;
	}
	
	~CDynDHtmlEventMap() {
		_destroy();
	}

	void Add( long elem, long type, const char * id, DHEVTFUNCCONTROL function ) {
		long current= _get_next();
		if( !id || !id ) return;  // Error: bad id
		if( current==-1 ) return; // Error: Not found

		if( current+2>=m_size )
			_bigger();

		m_eventMap[current].nType= (DHtmlEventMapEntryType)elem;
		m_eventMap[current].dispId= type;
		m_eventMap[current].szName= strdup(id);
		m_eventMap[current].pfnEventFunc= function;

		m_eventMap[current+1].nType= DHTMLEVENTMAPENTRY_END;
		m_eventMap[current+1].dispId= 0;
		m_eventMap[current+1].szName= NULL;
		m_eventMap[current+1].pfnEventFunc= (DHEVTFUNCCONTROL) NULL;
	}

    void Clean() {
        _destroy();
        _bigger();
    }

private:
	void _bigger() {
		if( !m_eventMap ) {
			m_eventMap= new DHtmlEventMapEntry[32];
			m_size= 32;
			return;
		}
		
		DHtmlEventMapEntry * eventMap= new DHtmlEventMapEntry[m_size+32];
		memcpy( eventMap, m_eventMap, m_size*sizeof(DHtmlEventMapEntry) );
		m_size+= 32;

		delete[] m_eventMap;
		m_eventMap= eventMap;
	}

	long _get_next() {
		if( m_eventMap && m_size>0 ) {
			for( long i= m_size-1; i>=0; i-- ) {
				if( m_eventMap[i].nType==ELEM::END ) return i;
			}
			return -1;
		}
		_bigger();
		return 0;
	}

    void _destroy() {
		if( m_eventMap ) {
			long i= 0;
			while( m_eventMap[i].nType!=DHTMLEVENTMAPENTRY_END ) free((char*)m_eventMap[i++].szName);
			delete m_eventMap;
		}
        m_eventMap= NULL;
        m_size= -1;
    }
};


class CHtmlExDlg : public CDHtmlDialog
{
public:
	CHtmlExDlg(CWnd* pParent = NULL);

	int PropertyHandler(const char *param, const char *value, const char *extra, char **result);
    void LoadPage( const char * page, const char * targetFrame );

protected:
    void PostEvent(const char *type, const char *value=NULL, const char *extra=NULL);
    
protected:
	enum { IDD = IDR_HTML_DHTMLDLG, IDH = 0 };

	CDynDHtmlEventMap m_dynMap;
	HICON m_hIcon;

	BOOL m_acceptClose;
	BOOL m_allowNavigate;
	BOOL m_allowPostEvent;

	DWORD m_hostFlags; // Host Flags
	CString m_url; // Current URL

public:
	void (*m_cb_post_event)(long wnd, const char *type, const char *value, 
		const char *extra, void *data);
	void *m_cb_data;
	CString m_iconfile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	/* CHtmlExDlg events */
	HRESULT OnHtmlEvent(IHTMLElement *pElement);
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);
	HRESULT STDMETHODCALLTYPE ShowContextMenu(DWORD dwID, POINT * ppt, IUnknown * pcmdtReserved, IDispatch * pdispReserved );

	/* WND */
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT onTrayIcon(WPARAM id, LPARAM message);

	/* DLG events */
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);

	/* CDHtmlDialog */
	void _OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, 
		VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	virtual void OnBeforeNavigate(LPDISPATCH pDisp, LPCTSTR szUrl);
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);

	/* MFC */
	virtual const DHtmlEventMapEntry* GetDHtmlEventMap();
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

	//DECLARE_DHTML_EVENT_MAP()
};

