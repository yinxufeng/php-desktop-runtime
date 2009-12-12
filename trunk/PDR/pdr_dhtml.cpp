#include "stdafx.h"
#include "CPDR.h"
#include "pdr_dhtml.h"
//#include "comutil.h"
//#pragma comment(lib,"comsupp.lib")
//#pragma comment(lib,"kernel32.lib")


ZEND_FUNCTION(pdr_dhtml_new)
{
	bool bWindowDecorator = true ;
	zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &bWindowDecorator ) ;

	CDHtmlDlg * pDlg = new CDHtmlDlg(bWindowDecorator?IDD_DHTMLDLG:IDD_DHTMLDLG_NOBORDER,NULL) ;

	int nResrc = _pdr_get_resrc_dhtml_dlg() ;
	ZEND_REGISTER_RESOURCE( return_value, pDlg, nResrc )
}

// 取得对话框指针
#define PDR_DHTML_GetDlgPtrFromResrc(type_spec,other_param) zval * hResrc ;\
	CDHtmlDlg *pDlg = NULL ;\
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, type_spec, &hResrc other_param ) == FAILURE )\
	{\
		RETURN_FALSE\
	}\
	ZEND_FETCH_RESOURCE(pDlg, CDHtmlDlg*, &hResrc, -1, resrc_name_pdr_dhtml_dlg, _pdr_get_resrc_dhtml_dlg()) ;\
	if(!pDlg)\
	{\
		zend_error(E_WARNING, "PDR DHTML: you was given a avalid dialog handle." );\
		RETURN_FALSE\
	}

ZEND_FUNCTION(pdr_dhtml_create)
{
	long nWindowShow = SW_SHOWNORMAL ;

	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("r|l", __PDR_RESRC_MPARAM(&nWindowShow))

	if( pDlg->GetSafeHwnd() )
	{
		zend_error(E_WARNING, "PDR DHTML: the dialog has created before." );
		RETURN_FALSE
	}

	// 创建
	if( pDlg->Create( pDlg->GetDialogTemplate() ) )
	{
		if(nWindowShow)
		{
			pDlg->ShowWindow(nWindowShow) ;
		}
		RETURN_TRUE
	}

	else
	{
		RETURN_FALSE
	}
	
}

ZEND_FUNCTION(pdr_dhtml_show)
{
	int nCmdShow = SW_SHOWNORMAL ;
	PDR_DHTML_GetDlgPtrFromResrc("r|l", __PDR_RESRC_MPARAM(&nCmdShow) )

	pDlg->ShowWindow(nCmdShow) ;
}

ZEND_FUNCTION(pdr_dhtml_domodal)
{
	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("r",)

	if( pDlg->GetSafeHwnd() )
	{
		zend_error(E_WARNING, "PDR DHTML: you have created for this dialog before." );
		RETURN_FALSE
	}

	int nResponse = pDlg->DoModal() ;
	if(nResponse==-1)
	{
		alert("创建对话框遇到错误：%d",::GetLastError())
	}

	RETURN_LONG( nResponse )
}

ZEND_FUNCTION(pdr_dhtml_get_hwnd)
{	
	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("r",)
	RETURN_LONG( (long)pDlg->GetSafeHwnd() )
}

ZEND_FUNCTION(pdr_dhtml_set_handle)
{
	int argc = ZEND_NUM_ARGS();
	if (argc<4)
	{
		WRONG_PARAM_COUNT;
	}

	// 取得其他各项参数
	zval ***params = (zval ***)safe_emalloc(sizeof(zval **), argc, 0);
	if (zend_get_parameters_array_ex(argc, params) == FAILURE)
	{
		efree(params);
		RETURN_FALSE;
	}

	// 参数1 对话框指针
	CDHtmlDlg *pDlg = NULL ;
	ZEND_FETCH_RESOURCE(pDlg, CDHtmlDlg*, params[0], -1, resrc_name_pdr_dhtml_dlg, _pdr_get_resrc_dhtml_dlg()) ;
	if(!pDlg)
	{
		zend_error(E_WARNING, "PDR DHTML: you was given a avalid dialog handle." );
		RETURN_FALSE
	}

	// 参数2 事件类型
	if( Z_TYPE_PP(params[1])!=IS_LONG )
	{
		zend_error(E_WARNING, "PDR DHTML: parameter 2 must be a int as event type." );
		RETURN_FALSE
	}
	long nEventType = Z_LVAL_PP(params[1]) ;

	// 参数3 html id
	if( Z_TYPE_PP(params[2])!=IS_STRING )
	{
		zend_error(E_WARNING, "PDR DHTML: parameter 2 must be a string as dhtml element id." );
		RETURN_FALSE
	}
	char * psHtmlId = Z_STRVAL_PP(params[2]) ;

	// 参数4 callback
	char *name;
	if (!__zend_is_callable(*params[3], &name)) {
		php_error_docref1(NULL TSRMLS_CC, name, E_WARNING, "First argument is expected to be a valid callback");
		efree(name);
		RETURN_NULL();
	}
	efree(name);


	pdr_callback_handle * pEventHandle = new pdr_callback_handle() ;
	pEventHandle->pFuncTable = EG(function_table) ;
	pEventHandle->pFuncName = *params[3] ;
	pEventHandle->tsrm_ls = tsrm_ls;

	(*params[3])->__refcount ++ ;
	// ZVAL_ADDREF(*params[3]) ;

	if(argc>4)
	{
		pEventHandle->nArgc = argc-4;
		pEventHandle->ppArgs = new zval*[pEventHandle->nArgc] ;
		// params+4 ;
		
		// 增加参数的 引用计数
		for(int i=0;i<pEventHandle->nArgc;i++)
		{
			pEventHandle->ppArgs[i] = *(params[4+i]) ;
			pEventHandle->ppArgs[i]->__refcount ++ ;
		}
	}

	else
	{
		pEventHandle->ppArgs = NULL ;
		pEventHandle->nArgc = 0 ;
	}

	pDlg->SetElementEventHandle(nEventType,psHtmlId,pEventHandle) ;
	
	efree(params);
}

HRESULT __pdr_dhtml_onevent(pdr_callback_handle * pHandle,zval * pEventParam)
{
	//TSRMLS_FETCH() ;
	void *** tsrm_ls = pHandle->tsrm_ls ;

	// 参数
	zval ***pppArgs = NULL ;
	pppArgs = new zval**[pHandle->nArgc+1] ;
	pppArgs[0] = &pEventParam ;

	// 用户参数
	for(int i=1;i<=pHandle->nArgc;i++)
	{
		pppArgs[i] = &pHandle->ppArgs[i-1] ;
	}

	zval *retval_ptr;
	call_user_function_ex(
		pHandle->pFuncTable
		, NULL, pHandle->pFuncName
		, &retval_ptr
		, pHandle->nArgc+1, pppArgs
		, 0, NULL TSRMLS_CC) ;

	delete [] pppArgs ;

	switch( Z_TYPE_P(retval_ptr) )
	{
		case IS_LONG :
			return (HRESULT)Z_LVAL_P(retval_ptr) ;
		case IS_NULL :
			return S_OK ;
		default :
			zend_error(E_WARNING, "PDR DHTML: the dhtml event handle function(method) must return a int value." );
			return S_OK ;
	}
}


ZEND_FUNCTION(pdr_dhtml_set_html)
{	
	char* pzId = NULL, * pzHtml = NULL ;
	int nIdLen = 0, nHtmlLen = 0 ;

	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("rss",__PDR_RESRC_MPARAM(&pzId,&nIdLen,&pzHtml,&nHtmlLen))
	pDlg->SetElementHtml((LPCTSTR)pzId,CComBSTR(pzHtml)) ;
}

ZEND_FUNCTION(pdr_dhtml_set_text)
{
	char* pzId = NULL, * pzHtml = NULL ;
	int nIdLen = 0, nHtmlLen = 0 ;

	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("rss",__PDR_RESRC_MPARAM(&pzId,&nIdLen,&pzHtml,&nHtmlLen))
	pDlg->SetElementText((LPCTSTR)pzId,CComBSTR(pzHtml)) ;
}

ZEND_FUNCTION(pdr_dhtml_set_value)
{
	char* pzId = NULL, * pzHtml = NULL ;
	int nIdLen = 0, nHtmlLen = 0 ;

	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("rss",__PDR_RESRC_MPARAM(&pzId,&nIdLen,&pzHtml,&nHtmlLen))

	VARIANT var ;
	var.vt = VT_BSTR ;
	var.bstrVal = CComBSTR(pzHtml) ;
	pDlg->SetElementProperty((LPCTSTR)pzId,DISPID_A_VALUE,&var) ;
}

ZEND_FUNCTION(pdr_dhtml_get_html)
{	
	char* pzId = NULL ;
	int nIdLen = 0 ;

	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("rs",__PDR_RESRC_MPARAM(&pzId,&nIdLen))

	CString strHtml( pDlg->GetElementHtml((LPCTSTR)pzId) ) ;
	RETURN_STRING( (char *)(LPCTSTR)strHtml,1) ;
}

ZEND_FUNCTION(pdr_dhtml_get_text)
{
	char* pzId = NULL, * pzHtml = NULL ;
	int nIdLen = 0, nHtmlLen = 0 ;

	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("rs",__PDR_RESRC_MPARAM(&pzId,&nIdLen,&pzHtml,&nHtmlLen))
		
	CString strText( pDlg->GetElementText((LPCTSTR)pzId) ) ;
	RETURN_STRING( (char *)(LPCTSTR)strText,1) ;
}

ZEND_FUNCTION(pdr_dhtml_get_value)
{
	char* pzId = NULL, * pzHtml = NULL ;
	int nIdLen = 0, nHtmlLen = 0 ;

	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("rs",__PDR_RESRC_MPARAM(&pzId,&nIdLen,&pzHtml,&nHtmlLen))

	VARIANT var = pDlg->GetElementProperty((LPCTSTR)pzId,DISPID_A_VALUE) ;
	CString strValue(var.bstrVal) ;
	RETURN_STRING( (char *)(LPCTSTR)strValue,1) ;
}

ZEND_FUNCTION(pdr_dhtml_ok)
{
	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("r",)

	if( !pDlg->GetSafeHwnd() )
	{
		zend_error(E_WARNING, "PDR DHTML: you must create this dialog first." );\
		RETURN_FALSE
	}

	pDlg->OnOK() ;
	RETURN_TRUE
}
ZEND_FUNCTION(pdr_dhtml_cancel)
{
	// 取得对话框指针
	PDR_DHTML_GetDlgPtrFromResrc("r",)

	if( !pDlg->GetSafeHwnd() )
	{
		zend_error(E_WARNING, "PDR DHTML: you must create this dialog first." );\
		RETURN_FALSE
	}

	pDlg->OnCancel() ;
	RETURN_TRUE
}

ZEND_FUNCTION(pdr_dhtml_navigate)
{
	// 取得各项参数
	char* pUrl = NULL ;
	char* pTargetFrame = NULL ;
	char* pHeaders = NULL ;
	char* pPostData = NULL ;
	int nUrlLen = 0 ;
	int nTargetFrameLen = 0 ;
	int nHeadersLen = 0 ;
	int nPostDataLen = 0 ;
	int nFlag = 0 ;

	PDR_DHTML_GetDlgPtrFromResrc("rs|lsssl",__PDR_RESRC_MPARAM(
			&pUrl, &nUrlLen
			, &nFlag
			, &pTargetFrame, &nTargetFrameLen
			, &pHeaders, &nHeadersLen
			, &pPostData, &nPostDataLen
	))
	
	if( !pDlg->GetSafeHwnd() )
	{
		zend_error(E_WARNING, "PDR DHTML: you must create this dialog first." );\
		RETURN_FALSE
	}


	pDlg->Navigate(pUrl,nFlag
			, nTargetFrameLen?pTargetFrame:NULL
			, nHeadersLen?pHeaders:NULL
			, nPostDataLen?pPostData:NULL
			, nPostDataLen
	) ;

	RETURN_TRUE
}

ZEND_FUNCTION(pdr_dhtml_exec_script)
{
	// 取得各项参数
	char* psScript = NULL ;
	int nScriptLen = 0 ;
	HRESULT hr ;

	PDR_DHTML_GetDlgPtrFromResrc("rs",__PDR_RESRC_MPARAM(&psScript, &nScriptLen ))

	if( !pDlg->GetSafeHwnd() )
	{
		zend_error(E_WARNING, "PDR DHTML: you must create this dialog first." );
		RETURN_FALSE 
	}

	IHTMLDocument2* pDocument = NULL ;
	hr = pDlg->GetDHtmlDocument(&pDocument) ;
	if( hr!=S_OK || !pDocument )
	{
		zend_error(E_WARNING, "PDR DHTML: can't query the IHTMLDocument2 interface." );
		RETURN_FALSE 
	}

	IHTMLWindow2* pWindow = NULL ;
	hr = pDocument->get_parentWindow(&pWindow) ;
	if( hr!=S_OK || !pDocument )
	{
		zend_error(E_WARNING, "PDR DHTML: can't query the IHTMLWindow2 interface." );
		RETURN_FALSE 
	}

	VARIANT ret;
	ret.vt = VT_EMPTY;
	CComBSTR bstrScript(psScript) ;
	CComBSTR bstrLanguage = L"javascript";
	hr = pWindow->execScript(bstrScript, bstrLanguage,&ret) ;
	if( hr!=S_OK )
	{
		zend_error(E_WARNING, "PDR DHTML: can't execute script." );
		RETURN_FALSE 
	}

	RETURN_TRUE
}

ZEND_FUNCTION(pdr_dhtml_call)
{
	// 取得各项参数
	char* psFuncName = NULL ;
	int nFuncNameLen = 0 ;
	char* psEncodeArgs = NULL ;
	int nEncodeArgsLen = 0 ;
	PDR_DHTML_GetDlgPtrFromResrc("rs|s",__PDR_RESRC_MPARAM(&psFuncName, &nFuncNameLen, &psEncodeArgs,&nEncodeArgsLen ))

    HRESULT hResult ;
	CComPtr<IHTMLDocument2>	pDoc2 ;
	if( FAILED(pDlg->GetDHtmlDocument(&pDoc2)) )
	{
		zend_error(E_WARNING, "PDR DHTML: can't query the IHTMLDocument2 interface." );
		RETURN_NULL()
	}

	CComPtr<IDispatch> pDispScript;
    if( FAILED(pDoc2->get_Script(&pDispScript)) )
    {
		zend_error(E_WARNING, "PDR DHTML: can't query the IDispatch interface." );
        RETURN_NULL()
    }

	// 取得函数 id
	DISPID nFuncDispId = -1 ;
	CComBSTR bsobFuncName(psFuncName) ;	
    if( FAILED(pDispScript->GetIDsOfNames(IID_NULL, &bsobFuncName, 1, LOCALE_SYSTEM_DEFAULT,&nFuncDispId)) )
    {
		zend_error(E_WARNING, "PDR DHTML: can't found javascript function." );
        RETURN_NULL()
    }

	// 参数
	DISPPARAMS param;
	param.cArgs = 1;
	param.cNamedArgs = 0;
	param.rgdispidNamedArgs = NULL;

	param.rgvarg = new VARIANTARG[3];
	param.rgvarg[0].vt = VT_BSTR;
	param.rgvarg[0].bstrVal = CComBSTR( nEncodeArgsLen?psEncodeArgs:"" ).Detach();


	EXCEPINFO excepinfo ;
    excepinfo.wCode               = 1001;
    //excepinfo.wReserved           = 0;
    excepinfo.bstrSource          = 0;
    excepinfo.bstrDescription     = 0;
    excepinfo.bstrHelpFile        = 0;
    excepinfo.dwHelpContext       = 0;
    excepinfo.pvReserved          = 0;
    excepinfo.pfnDeferredFillIn   = 0;
    excepinfo.scode               = 0;

	UINT nArgErr = 0 ;

	VARIANT varResult ;
    varResult.vt = VT_VARIANT;
    hResult = pDispScript->Invoke(
					nFuncDispId
					, IID_NULL
					, LOCALE_USER_DEFAULT
					, DISPATCH_METHOD
					, &param
					, &varResult
					, &excepinfo
					, &nArgErr
	);

	CString strResult(varResult) ;
	RETURN_STRINGL( (char *)(LPCTSTR)strResult,strResult.GetLength(), 1 )
}

ZEND_FUNCTION(pdr_dhtml_get_tags)
{
	// 取得各项参数
	char* psTagName = NULL ;
	int nTagNameLen = 0 ;
	HRESULT hr ;

	PDR_DHTML_GetDlgPtrFromResrc("rs",__PDR_RESRC_MPARAM(&psTagName, &nTagNameLen ))

	if( !pDlg->GetSafeHwnd() )
	{
		zend_error(E_WARNING, "PDR DHTML: you must create this dialog first." );
		RETURN_NULL()
	}
	
	IDispatch *pIDispatch = NULL;
	hr = pDlg->m_pBrowserApp->get_Document(&pIDispatch) ;
	if( hr!=S_OK || !pIDispatch )
	{
		zend_error(E_WARNING, "PDR DHTML: can't query the IDispatch interface." );
		RETURN_NULL()
	}

	IHTMLDocument3 * pIHTMLDocument3 = NULL ;
	hr = pIDispatch->QueryInterface(IID_IHTMLDocument3, (void **) &pIHTMLDocument3);
	if( hr!=S_OK || !pIHTMLDocument3 )
	{
		zend_error(E_WARNING, "PDR DHTML: can't query the IHTMLDocument3 interface." );
		RETURN_NULL()
	}

	CComBSTR bstrTagName(psTagName) ;
	IHTMLElementCollection * pIHTMLEleColl = NULL ;
	hr = pIHTMLDocument3->getElementsByTagName(bstrTagName,&pIHTMLEleColl) ;
	if( hr!=S_OK || !pIHTMLEleColl )
	{
		zend_error(E_WARNING, "PDR DHTML: can't query the IHTMLElementCollection interface." );
		RETURN_NULL()
	}

	RETURN_LONG((long)pIHTMLEleColl) ;
}

ZEND_FUNCTION(pdr_dhtml_el_collection_len)
{
	long nEleCollectionPtr = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nEleCollectionPtr ) == FAILURE )
	{
		RETURN_NULL()
	}
	IHTMLElementCollection * pIHTMLEleColl = (IHTMLElementCollection *)nEleCollectionPtr ;

	long nLen = 0 ;
	if( S_OK!=pIHTMLEleColl->get_length(&nLen) )
	{
		zend_error(E_WARNING, "PDR DHTML: can't get the IHTMLElementCollection length." );
		RETURN_NULL()
	}

	RETURN_LONG(nLen) ;
}

ZEND_FUNCTION(pdr_dhtml_el_collection_item)
{
	long nEleCollectionPtr = 0 ;
	long nEleIdx = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &nEleCollectionPtr, &nEleIdx ) == FAILURE )
	{
		RETURN_NULL()
	}
	IHTMLElementCollection * pIHTMLEleColl = (IHTMLElementCollection *)nEleCollectionPtr ;
	
	VARIANT vEleInd;
	vEleInd.lVal = nEleIdx ;
	vEleInd.vt = VT_I4  ;


	IDispatch * pIDispatch = NULL ;
	if( S_OK!=pIHTMLEleColl->item(vEleInd,vEleInd,&pIDispatch) || !pIDispatch )
	{
		zend_error(E_WARNING, "PDR DHTML: can't query the IDispatch interface." );
		RETURN_NULL()
	}
	
	IHTMLElement * pIHTMLEle = NULL ;
	HRESULT hr = pIDispatch->QueryInterface(IID_IHTMLElement,(void **)&pIHTMLEle) ;
	if( S_OK!=hr || !pIHTMLEle )
	{
		zend_error(E_WARNING, "PDR DHTML: can't query the IHTMLElement interface." );
		RETURN_NULL()
	}

	RETURN_LONG((long)pIHTMLEle) ;
}
