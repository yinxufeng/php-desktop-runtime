#pragma once

class CDynDHtmlEventMap
{
public:
	CMapStringToPtr m_arElementHandlesById ;
	CMapStringToPtr m_arElementHandlesByClass ;
	CMapStringToPtr m_arElementHandlesByTagName ;
	DHtmlEventMapEntry * m_eventMap;
	long m_size;

	CDynDHtmlEventMap() ;

	~CDynDHtmlEventMap() ;

	void Add( long elem, long type, const char * id, DHEVTFUNCCONTROL function ) ;
	
	HRESULT OnEvent(CString strId,CString strEventType,zval * pEventParam=NULL,IHTMLElement* pElement=NULL, IHTMLEventObj * pEvent=NULL) ;

    void Clean() {
        _destroy();
        _bigger();
    }

private:
	void _bigger() ;
	long _get_next() ;
    void _destroy() ;
};