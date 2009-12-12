#include "StdAfx.h"

#include "CPDR.h"
#include "CDynDHtmlEventMap.h"
#include "pdr_dhtml.h"


#define SetEventPropertyLong(method,property) pEvent->method(&lVal) ;\
		add_property_long(pEventParam, property, lVal) ;

#define SetEventPropertyBool(method,property) pEvent->method(&bVal) ;\
		add_property_bool(pEventParam, property, bVal) ;

#define SetEventPropertyString(method,property) pEvent->method(&bsVal) ;\
		add_property_string(pEventParam, property, (char *)(LPCTSTR)CString(bsVal), 1) ;

#define LookupElementEventHandle(mapping) pTypeHandles = NULL ;\
		pEventHandle = NULL ;\
		if( mapping.Lookup(strName,(void *&)pTypeHandles) && pTypeHandles )\
		{ pTypeHandles->Lookup(strEventType,(void *&)pEventHandle) ; }

#define DeleteElementEventHandles(mapping) pos = NULL; \
		for( pos=mapping.GetStartPosition(); pos != NULL; )\
		{\
			mapping.GetNextAssoc( pos, key, (void *&)pTypeHandles ) ;\
			POSITION pos2 = NULL ;\
			for( pos2=pTypeHandles->GetStartPosition(); pos2 != NULL; )\
			{\
				pTypeHandles->GetNextAssoc( pos2, key, (void *&)pEventHandle ) ;\
				delete pEventHandle ;\
			}\
			delete pTypeHandles ;\
		}\
		mapping.RemoveAll() ;


CDynDHtmlEventMap::CDynDHtmlEventMap()
{
	m_eventMap= NULL;
	m_size= -1;
	_bigger();
	m_eventMap[0].nType= DHTMLEVENTMAPENTRY_END;
	m_eventMap[0].dispId= 0;
	m_eventMap[0].szName= NULL;
	m_eventMap[0].pfnEventFunc= (DHEVTFUNCCONTROL) NULL;
}

CDynDHtmlEventMap::~CDynDHtmlEventMap(void)
{
	_destroy();
}

void CDynDHtmlEventMap::_destroy()
{
	if( m_eventMap ) {
		long i= 0;
		while( m_eventMap[i].nType!=DHTMLEVENTMAPENTRY_END ) free((char*)m_eventMap[i++].szName);
		delete m_eventMap;
	}
    m_eventMap= NULL;
    m_size= -1;

	// 删除 事件
	POSITION pos = NULL ;
	CString key ;
	CMapStringToPtr * pTypeHandles = NULL ;
	pdr_callback_handle * pEventHandle = NULL ;

	DeleteElementEventHandles(m_arElementHandlesById)
	DeleteElementEventHandles(m_arElementHandlesByClass)
	DeleteElementEventHandles(m_arElementHandlesByTagName)
}


void CDynDHtmlEventMap::Add( long elem, long type, const char * id, DHEVTFUNCCONTROL function )
{
	long current= _get_next();
	if( !id || !id ) return;  // Error: bad id
	if( current==-1 ) return; // Error: Not found

	if( current+2>=m_size )
	{
		_bigger();
	}

	m_eventMap[current].nType= (DHtmlEventMapEntryType)elem;
	m_eventMap[current].dispId= type;
	m_eventMap[current].szName= _strdup(id);
	m_eventMap[current].pfnEventFunc= function;

	m_eventMap[current+1].nType= DHTMLEVENTMAPENTRY_END;
	m_eventMap[current+1].dispId= 0;
	m_eventMap[current+1].szName= NULL;
	m_eventMap[current+1].pfnEventFunc= (DHEVTFUNCCONTROL) NULL;
}
	
HRESULT CDynDHtmlEventMap::OnEvent(CString strId,CString strEventType,zval * pEventParam,IHTMLElement* pElement, IHTMLEventObj * pEvent)
{
	CMapStringToPtr * pTypeHandles = NULL;
	pdr_callback_handle * pEventHandle = NULL ;
	CString strName ;

	// 通过 id 查找 element event handle
	strName = strId ;
	LookupElementEventHandle(m_arElementHandlesById) ;

	// 通过 class 查找
	if( !pEventHandle && pElement )
	{
		CComBSTR bsType ;
		pElement->get_className(&bsType) ;
		strName = bsType ;
		LookupElementEventHandle(m_arElementHandlesByClass) ;
	}

	// 通过 tagName 查找 
	if(!pEventHandle && pElement )
	{
		CComBSTR bsClass ;
		pElement->get_tagName(&bsClass) ;
		strName = bsClass ;
		strName.MakeLower() ;
		LookupElementEventHandle(m_arElementHandlesByTagName) ;
	}

	// 均无法找到
	if(!pEventHandle)
	{
		return S_OK ;
	}

	// 调用 PHP 事件
	// ----------------------------------------------
	//TSRMLS_FETCH() ;
	void *** tsrm_ls = pEventHandle->tsrm_ls ;

	bool bCreateEventOb = false ;
	if(!pEventParam)
	{
		//pEventParam = new zval() ;
		MAKE_STD_ZVAL(pEventParam) ;
		object_init(pEventParam);
		bCreateEventOb = true ;
	}
	add_property_string(pEventParam, "elementId", (char *)(LPCTSTR)strId, 1) ;
	add_property_string(pEventParam, "event", (char *)(LPCTSTR)strEventType, 1) ;

	if( pEvent )
	{
		VARIANT_BOOL bVal = false ;
		long lVal = NULL ;
		BSTR bsVal  ;

		SetEventPropertyBool(get_altKey,"altKey") ;
		SetEventPropertyBool(get_ctrlKey,"ctrlKey") ;
		SetEventPropertyBool(get_shiftKey,"shiftKey") ;
		SetEventPropertyLong(get_keyCode,"keyCode") ;
		SetEventPropertyLong(get_button,"button") ;
		SetEventPropertyString(get_type,"type") ;
		SetEventPropertyString(get_qualifier,"qualifier") ;
		SetEventPropertyLong(get_reason,"reason") ;
		SetEventPropertyLong(get_x,"x") ;
		SetEventPropertyLong(get_y,"y") ;
		SetEventPropertyLong(get_clientX,"clientX") ;
		SetEventPropertyLong(get_clientY,"clientY") ;
		SetEventPropertyLong(get_offsetX,"offsetX") ;
		SetEventPropertyLong(get_offsetY,"offsetY") ;
		SetEventPropertyLong(get_screenX,"screenX") ;
		SetEventPropertyLong(get_screenY,"screenY") ;
	}

	if(pElement)
	{
		BSTR bsVal  ;

		pElement->get_className(&bsVal) ;
		add_property_string(pEventParam, "elementClass", (char *)(LPCTSTR)CString(bsVal), 1) ;

		pElement->get_tagName(&bsVal) ;
		add_property_string(pEventParam, "elementTag", (char *)(LPCTSTR)CString(bsVal), 1) ;
	}
	
	HRESULT bRet = __pdr_dhtml_onevent(pEventHandle,pEventParam) ;

	if(bCreateEventOb)
	{
		efree(pEventParam) ;
	}

	return bRet ;
}



void CDynDHtmlEventMap::_bigger()
{
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

long CDynDHtmlEventMap::_get_next()
{
	if( m_eventMap && m_size>0 ) {
		for( long i= m_size-1; i>=0; i-- ) {
			if( m_eventMap[i].nType==DHTMLEVENTMAPENTRY_END ) return i;
		}
		return -1;
	}
	_bigger();
	return 0;
}