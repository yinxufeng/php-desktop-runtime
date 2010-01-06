// CDHtmlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CPDR.h"
#include "CDHtmlDlg.h"
#include <afxctl.h>


// CDHtmlDlg 对话框

IMPLEMENT_DYNCREATE(CDHtmlDlg, CDHtmlDialog)

BEGIN_DISPATCH_MAP(CDHtmlDlg, CDHtmlDialog)
	DISP_FUNCTION(CDHtmlDlg, "execute", ExecPHP, VT_BSTR,VTS_BSTR)
	DISP_FUNCTION(CDHtmlDlg, "call", CallPHPFunction, VT_BSTR, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()

BEGIN_MESSAGE_MAP(CDHtmlDlg, CDHtmlDialog)
	ON_MESSAGE(WM_TRAY_ICON, OnTrayIcon)
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

CDHtmlDlg::CDHtmlDlg(UINT nIDTemplate/*=CDHtmlDlg::IDD*/,CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(nIDTemplate, CDHtmlDlg::IDH, pParent)
{
	EnableAutomation() ;
}

CDHtmlDlg::~CDHtmlDlg()
{}

LPCTSTR CDHtmlDlg::GetDialogTemplate()
{
	return m_lpszTemplateName ;
}

void CDHtmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDHtmlDlg::OnInitDialog()
{
	// 需要初始化 ole ，否则 dhtml 中无法复制
	OleInitialize(NULL) ;

	CDHtmlDialog::OnInitDialog() ;

	SetExternalDispatch(GetIDispatch(TRUE)) ;

	// 除非将焦点设置到控件，否则返回 TRUE
	return m_dynMap.OnEvent(ELEMENT_ID_DIALOG,GetDHtmlEventName(DLG_EVENT_ONINITDIALOG))==S_OK ;  
}

void CDHtmlDlg::SetElementEventHandle(long nEventType,char * pElemId,pdr_callback_handle * pEventHandle)
{
	CString strInputElementId(pElemId) ;
	CString strElementId ;
	CMapStringToPtr * pEventHandles = NULL ;
	int nFindBy = 0 ;

	if( strInputElementId.Left(3).MakeLower()=="id:" )
	{
		strElementId = strInputElementId.Mid(3) ;
		pEventHandles = &m_dynMap.m_arElementHandlesById ;
		nFindBy = DHTMLEVENTMAPENTRY_NAME ;
	}
	else if( strInputElementId.Left(8).MakeLower()=="tagname:" )
	{
		strElementId = strInputElementId.Mid(8) ;
		strElementId.MakeLower() ;
		pEventHandles = &m_dynMap.m_arElementHandlesByTagName ;
		nFindBy = DHTMLEVENTMAPENTRY_TAG ;
	}
	else if( strInputElementId.Left(6).MakeLower()=="class:" )
	{
		strElementId = strInputElementId.Mid(6) ;
		pEventHandles = &m_dynMap.m_arElementHandlesByClass ;
		nFindBy = DHTMLEVENTMAPENTRY_CLASS ;
	}
	else
	{
		strElementId = strInputElementId ;
		pEventHandles = &m_dynMap.m_arElementHandlesById ;
		nFindBy = DHTMLEVENTMAPENTRY_NAME ;
	}

	// HTML 事件
	if( strInputElementId.Left(1)!="@" )
	{
		m_dynMap.Add(
			nFindBy, nEventType, (char *)(LPCTSTR)strElementId
			, CDYNDHEM_FUNC(&CDHtmlDlg::OnHtmlEvent)
		) ;
	}

	CMapStringToPtr * strEventType ;
	if( !pEventHandles->Lookup(strElementId,(void *&)strEventType) )
	{
		strEventType = new CMapStringToPtr() ;
		pEventHandles->SetAt(strElementId,strEventType) ;
	}

	CString s = GetDHtmlEventName(nEventType) ;
	strEventType->SetAt( s, (void *)pEventHandle ) ;
}


const DHtmlEventMapEntry* CDHtmlDlg::GetDHtmlEventMap()
{
	TRACE0("GetDHtmlEventMap()\r\n") ;
	return m_dynMap.m_eventMap;
}


void CDHtmlDlg::ClearEventHandles()
{
	m_dynMap.Clean() ;
}


HRESULT CDHtmlDlg::OnHtmlEvent(IHTMLElement* pElement)
{
	if(!pElement)
	{
		return S_OK;
	}

	IHTMLEventObj * pEvent;
	CComBSTR bs_id, bs_type;
	
	if( GetEvent( &pEvent )!=S_OK )
	{
		return S_OK;
	}

	pElement->get_id(&bs_id);
	pEvent->get_type(&bs_type);
	return this->m_dynMap.OnEvent( CString(bs_id), CString(bs_type), NULL, pElement, pEvent ) ;
}

void CDHtmlDlg::OnOK()
{
	CDHtmlDialog::OnOK();
}
void CDHtmlDlg::OnCancel()
{
	CDHtmlDialog::OnCancel();
}

BOOL CDHtmlDlg::IsExternalDispatchSafe()
{
	return TRUE;
}

#define ascii2wbs(psAscII,bst) int nAscIILen = strlen(psAscII) ;\
	DWORD nWLen = MultiByteToWideChar(CP_ACP,NULL,(LPCTSTR)psAscII,nAscIILen+1,NULL,0) ;\
	WCHAR * __wp = new WCHAR [nWLen] ;\
	MultiByteToWideChar(CP_ACP,NULL,(LPCTSTR)psAscII,nAscIILen+1,__wp,nWLen) ;\
	bst = ::SysAllocString((const OLECHAR *)__wp) ;\
	delete [] __wp ;


BSTR CDHtmlDlg::CallPHPFunction(CString strFuncName,CString strArgsJson)
{
	TSRMLS_FETCH() ;

	// 函数名
	zval * pZvalFunc ;
	char * psFunc = (char*)(LPCTSTR)strFuncName ;
	MAKE_STD_ZVAL(pZvalFunc)
	ZVAL_STRINGL( pZvalFunc, psFunc, strlen(psFunc), 1 )

	// 参数
	char * psArg = (char*)(LPCTSTR)strArgsJson ;	
	zval *** pppArgs = new zval**[1] ;
	zval * pArg ;
	MAKE_STD_ZVAL(pArg)
	ZVAL_STRINGL( pArg, psArg, strlen(psArg), 1 )
	pppArgs[0] = &pArg ;


	zval * pRetVal;
	call_user_function_ex(
		EG(function_table)
		, NULL, pZvalFunc
		, &pRetVal
		, 1, pppArgs, 0, NULL TSRMLS_CC
	) ;

	BSTR bsRet ;
	if(pRetVal)
	{
		convert_to_string_ex(&pRetVal);
		char * psRet = Z_STRVAL_P(pRetVal) ;
		ascii2wbs(psRet,bsRet)

		efree(pRetVal) ;
	}

	else
	{
		bsRet = ::SysAllocString((const OLECHAR *)"") ;
	}


	efree(pZvalFunc) ;
	efree(pArg) ;
	delete [] pppArgs ;

	return bsRet ;
}
BSTR CDHtmlDlg::ExecPHP(CString strPHPCode)
{

	DWORD dwNum = ::WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)(LPCTSTR)strPHPCode,-1,NULL,0,NULL,FALSE) ;
	char * psPHPCode = new char[dwNum] ;
	WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)(LPCTSTR)strPHPCode,-1,psPHPCode,dwNum,NULL,FALSE) ;

	TSRMLS_FETCH() ;

	zval* pRetVal ;
	MAKE_STD_ZVAL(pRetVal);
	zend_eval_string((char *)(LPCTSTR)strPHPCode, pRetVal, "PDR.JavaScript: external.execute()" TSRMLS_CC) ;

	convert_to_string_ex(&pRetVal);
	char * psRet = Z_STRVAL_P(pRetVal) ;

	
	BSTR bsRet ;
	ascii2wbs(psRet,bsRet)


	efree(pRetVal) ;
	return  bsRet ;
}


////////////////////////////////////////////////////////////////////////////
#define CreatePHPObject TSRMLS_FETCH() ;\
		zval * pEventParam = NULL ;\
		/*zval * pEventParam = new zval() ;*/\
		MAKE_STD_ZVAL(pEventParam) ;\
		object_init(pEventParam);\

void CDHtmlDlg::OnBeforeNavigate(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CreatePHPObject
	add_property_string(pEventParam, "url", (char *)szUrl, 1) ;
	this->m_dynMap.OnEvent( ELEMENT_ID_DIALOG,GetDHtmlEventName(DLG_EVENT_ONBEFORENAVIGATE), pEventParam ) ;
	efree(pEventParam) ;
	
	CDHtmlDialog::OnBeforeNavigate(pDisp, szUrl);
}

void CDHtmlDlg::OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnNavigateComplete(pDisp, szUrl);

	CreatePHPObject
	add_property_string(pEventParam, "url", (char *)szUrl, 1) ;
	this->m_dynMap.OnEvent( ELEMENT_ID_DIALOG,GetDHtmlEventName(DLG_EVENT_ONNAVIGATECOMPLETE), pEventParam ) ;
	efree(pEventParam) ;
}

void CDHtmlDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);

	CreatePHPObject
	add_property_string(pEventParam, "url", (char *)szUrl, 1) ;
	this->m_dynMap.OnEvent( ELEMENT_ID_DIALOG,GetDHtmlEventName(DLG_EVENT_ONDOCUMENTCOMPLETE), pEventParam ) ;
	efree(pEventParam) ;
}

LRESULT CDHtmlDlg::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	CString strMsg = GetDHtmlEventName(lParam) ;

	CreatePHPObject
	add_property_long(pEventParam,"id",wParam) ;
	add_property_long(pEventParam,"message",lParam) ;
	add_property_long(pEventParam, "wParam", wParam) ;
	add_property_long(pEventParam, "lParam", lParam) ;
	add_property_long(pEventParam, "hwnd", (long)this->GetSafeHwnd()) ;
	add_property_string(pEventParam, "event", (char *)(LPCTSTR)strMsg, 1) ;

	LRESULT nRet = (LRESULT)this->m_dynMap.OnEvent( ELEMENT_ID_DIALOG,strMsg, pEventParam ) ;

	efree(pEventParam) ;
	return nRet;
}

BOOL CDHtmlDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CString strEventName = GetDHtmlEventName(DLG_EVENT_ONCOMMAND) ;

	CreatePHPObject
	add_property_long(pEventParam, "id", wParam) ;
	add_property_long(pEventParam, "wParam", wParam) ;
	add_property_long(pEventParam, "lParam", lParam) ;
	add_property_long(pEventParam, "hwnd", (long)this->GetSafeHwnd()) ;
	add_property_string(pEventParam, "event", (char *)(LPCTSTR)strEventName, 1) ;

	BOOL bRet = this->m_dynMap.OnEvent( ELEMENT_ID_DIALOG,strEventName, pEventParam ) ;
	efree(pEventParam) ;

	return ( bRet || CDHtmlDialog::OnCommand(wParam, lParam) );
}

void CDHtmlDlg::OnClose()
{
	if( m_dynMap.OnEvent( ELEMENT_ID_DIALOG, GetDHtmlEventName(DLG_EVENT_ONCLOSE))!=S_FALSE )
	{
		CDialog::OnClose() ;

		OleUninitialize() ;
	}
}


BOOL CDHtmlDlg::PreTranslateMessage(MSG* pMsg)
{
	TSRMLS_FETCH() ;
	_make_msg_object_p(pMsg) ;
	LRESULT nRet = this->m_dynMap.OnEvent( ELEMENT_ID_DIALOG, GetDHtmlEventName(DLG_EVENT_PRETRANSLATEMSG), pZvalMsg ) ;
	efree(pZvalMsg) ;

	if( S_OK==nRet  )
	{
		return CDialog::PreTranslateMessage(pMsg);
	}

	// 消息已经被处理，不再继续处理
	else
	{
		return nRet ;
	}
}

LRESULT CDHtmlDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CreatePHPObject

	add_property_long(pEventParam, "hwnd", (long)GetSafeHwnd()) ;
	add_property_long(pEventParam, "message", (long)message) ;
	add_property_long(pEventParam, "wParam", (long)wParam) ;
	add_property_long(pEventParam, "lParam", (long)lParam) ;

	LRESULT nRet = this->m_dynMap.OnEvent( ELEMENT_ID_DIALOG, GetDHtmlEventName(DLG_EVENT_WINDOWPROC), pEventParam ) ;
	efree(pEventParam) ;

	if( S_OK == nRet )
	{
		return CDialog::WindowProc(message, wParam, lParam);
	}

	// 消息已经被处理，不再继续处理
	else
	{
		return nRet ;
	}

}

void CDHtmlDlg::DoSysCommand(int nId)
{
	OnSysCommand(nId,SC_HOTKEY) ;
}

void CDHtmlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDHtmlDialog::OnSysCommand(nID, lParam);
}






CString GetDHtmlEventName(long nType)
{
	switch(nType)
	{
		case DISPID_HTMLELEMENTEVENTS_ONACTIVATE : return CString("activate") ;
		case DISPID_HTMLELEMENTEVENTS_ONAFTERUPDATE : return CString("afterupdate") ;
		case DISPID_HTMLELEMENTEVENTS_ONBEFOREACTIVATE : return CString("beforeactivate") ;
		case DISPID_HTMLELEMENTEVENTS_ONBEFORECOPY : return CString("beforecopy") ;
		case DISPID_HTMLELEMENTEVENTS_ONBEFORECUT : return CString("beforecut") ;
		case DISPID_HTMLELEMENTEVENTS_ONBEFOREDEACTIVATE : return CString("beforedeactivate") ;
		case DISPID_HTMLELEMENTEVENTS_ONBEFOREEDITFOCUS : return CString("beforeeditfocus") ;
		case DISPID_HTMLELEMENTEVENTS_ONBEFOREPASTE : return CString("beforepaste") ;
		case DISPID_HTMLELEMENTEVENTS_ONBEFOREUPDATE : return CString("beforeupdate") ;
		case DISPID_HTMLELEMENTEVENTS_ONBLUR : return CString("blur") ;
		case DISPID_HTMLELEMENTEVENTS_ONCELLCHANGE : return CString("cellchange") ;
		case DISPID_HTMLELEMENTEVENTS_ONCLICK : return CString("click") ;
		case DISPID_HTMLELEMENTEVENTS_ONCONTEXTMENU : return CString("contextmenu") ;
		case DISPID_HTMLELEMENTEVENTS_ONCONTROLSELECT : return CString("controlselect") ;
		case DISPID_HTMLELEMENTEVENTS_ONCOPY : return CString("copy") ;
		case DISPID_HTMLELEMENTEVENTS_ONCUT : return CString("cut") ;
		case DISPID_HTMLELEMENTEVENTS_ONDATAAVAILABLE : return CString("dataavailable") ;
		case DISPID_HTMLELEMENTEVENTS_ONDATASETCHANGED : return CString("datasetchanged") ;
		case DISPID_HTMLELEMENTEVENTS_ONDATASETCOMPLETE : return CString("datasetcomplete") ;
		case DISPID_HTMLELEMENTEVENTS_ONDBLCLICK : return CString("dblclick") ;
		case DISPID_HTMLELEMENTEVENTS_ONDEACTIVATE : return CString("deactivate") ;
		case DISPID_HTMLELEMENTEVENTS_ONDRAG : return CString("drag") ;
		case DISPID_HTMLELEMENTEVENTS_ONDRAGEND : return CString("dragend") ;
		case DISPID_HTMLELEMENTEVENTS_ONDRAGENTER : return CString("dragenter") ;
		case DISPID_HTMLELEMENTEVENTS_ONDRAGLEAVE : return CString("dragleave") ;
		case DISPID_HTMLELEMENTEVENTS_ONDRAGOVER : return CString("dragover") ;
		case DISPID_HTMLELEMENTEVENTS_ONDRAGSTART : return CString("dragstart") ;
		case DISPID_HTMLELEMENTEVENTS_ONDROP : return CString("drop") ;
		case DISPID_HTMLELEMENTEVENTS_ONERRORUPDATE : return CString("errorupdate") ;
		case DISPID_HTMLELEMENTEVENTS_ONFILTERCHANGE : return CString("filterchange") ;
		case DISPID_HTMLELEMENTEVENTS_ONFOCUS : return CString("focus") ;
		case DISPID_HTMLELEMENTEVENTS_ONFOCUSIN : return CString("focusin") ;
		case DISPID_HTMLELEMENTEVENTS_ONFOCUSOUT : return CString("focusout") ;
		case DISPID_HTMLELEMENTEVENTS_ONHELP : return CString("help") ;
		case DISPID_HTMLELEMENTEVENTS_ONKEYDOWN : return CString("keydown") ;
		case DISPID_HTMLELEMENTEVENTS_ONKEYPRESS : return CString("keypress") ;
		case DISPID_HTMLELEMENTEVENTS_ONKEYUP : return CString("keyup") ;
		case DISPID_HTMLELEMENTEVENTS_ONLAYOUTCOMPLETE : return CString("layoutcomplete") ;
		case DISPID_HTMLELEMENTEVENTS_ONLOSECAPTURE : return CString("losecapture") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN : return CString("mousedown") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOUSEENTER : return CString("mouseenter") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOUSELEAVE : return CString("mouseleave") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOUSEMOVE : return CString("mousemove") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOUSEOUT : return CString("mouseout") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER : return CString("mouseover") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOUSEUP : return CString("mouseup") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOUSEWHEEL : return CString("mousewheel") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOVE : return CString("move") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOVEEND : return CString("moveend") ;
		case DISPID_HTMLELEMENTEVENTS_ONMOVESTART : return CString("movestart") ;
		case DISPID_HTMLELEMENTEVENTS_ONPAGE : return CString("page") ;
		case DISPID_HTMLELEMENTEVENTS_ONPASTE : return CString("paste") ;
		case DISPID_HTMLELEMENTEVENTS_ONPROPERTYCHANGE : return CString("propertychange") ;
		case DISPID_HTMLELEMENTEVENTS_ONREADYSTATECHANGE : return CString("readystatechange") ;
		case DISPID_HTMLELEMENTEVENTS_ONRESIZE : return CString("resize") ;
		case DISPID_HTMLELEMENTEVENTS_ONRESIZEEND : return CString("resizeend") ;
		case DISPID_HTMLELEMENTEVENTS_ONRESIZESTART : return CString("resizestart") ;
		case DISPID_HTMLELEMENTEVENTS_ONROWENTER : return CString("rowenter") ;
		case DISPID_HTMLELEMENTEVENTS_ONROWEXIT : return CString("rowexit") ;
		case DISPID_HTMLELEMENTEVENTS_ONROWSDELETE : return CString("rowsdelete") ;
		case DISPID_HTMLELEMENTEVENTS_ONROWSINSERTED : return CString("rowsinserted") ;
		case DISPID_HTMLELEMENTEVENTS_ONSCROLL : return CString("scroll") ;
		case DISPID_HTMLELEMENTEVENTS_ONSELECTSTART : return CString("selectstart") ;
			
		case DLG_EVENT_ONINITDIALOG: return CString("initdialog") ;
		case DLG_EVENT_ONBEFORENAVIGATE: return CString("beforenavigate") ;
		case DLG_EVENT_ONNAVIGATECOMPLETE: return CString("navigatecomplete") ;
		case DLG_EVENT_ONDOCUMENTCOMPLETE: return CString("documentcomplete") ;
		case DLG_EVENT_ONCOMMAND: return CString("command") ;
		case DLG_EVENT_ONCLOSE: return CString("close") ;

		case DLG_EVENT_PRETRANSLATEMSG: return CString("pretranslatemsg") ;
		case DLG_EVENT_WINDOWPROC: return CString("windowproc") ;

		case TRAY_EVENT_ONMOUSEMOVE: return CString("traymousemove") ;
		case TRAY_EVENT_ONLBUTTONDOWN: return CString("traylbuttondown") ;
		case TRAY_EVENT_ONLBUTTONUP: return CString("traylbuttonup") ;
		case TRAY_EVENT_ONLBUTTONDBLCLK: return CString("traylbuttondblclk") ;
		case TRAY_EVENT_ONRBUTTONDOWN: return CString("trayrbuttondown") ;
		case TRAY_EVENT_ONRBUTTONUP: return CString("trayrbuttonup") ;
		case TRAY_EVENT_ONRBUTTONDBLCLK: return CString("trayrbuttondblclk") ;
		case TRAY_EVENT_ONMBUTTONDOWN: return CString("traymbuttondblclk") ;
		case TRAY_EVENT_ONMBUTTONUP: return CString("traymbuttondown") ;
		case TRAY_EVENT_ONMBUTTONDBLCLK: return CString("traymbuttondblclk") ;


		default: return CString() ;
	}
}