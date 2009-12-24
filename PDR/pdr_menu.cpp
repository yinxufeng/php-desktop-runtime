#include "stdafx.h"
#include "CPDR.h"


ZEND_FUNCTION(pdr_menu_create)
{
	CMenu * pMenu = new CMenu() ;
	pMenu->CreatePopupMenu() ;

	int nResrc = _pdr_get_resrc_menu_icon() ;
	ZEND_REGISTER_RESOURCE( return_value, pMenu, nResrc )
}


// 取得菜单指针
#define _pdr_menu_getresrc(type_spec,other_param) zval * hResrc ;\
	CMenu * pMenu = NULL ;\
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, type_spec, &hResrc other_param ) == FAILURE )\
	{\
		RETURN_FALSE\
	}\
	ZEND_FETCH_RESOURCE(pMenu, CMenu*, &hResrc, -1, resrc_name_pdr_menu_icon, _pdr_get_resrc_menu_icon()) ;\
	if(!pMenu)\
	{\
		zend_error(E_WARNING, "PDR Popup Menu: you was given a avalid menu handle." );\
		RETURN_FALSE\
	}

ZEND_FUNCTION(pdr_menu_append)
{
	long nFlag=MF_STRING, nId=0 ;
	char * psText ;
	long nTextLen = 0 ; 

	_pdr_menu_getresrc("rls|l",__PDR_RESRC_MPARAM(&nId,&psText,&nTextLen,&nFlag))
	pMenu->AppendMenu(nFlag,nId,psText) ;
}

ZEND_FUNCTION(pdr_menu_insert)
{
	long nFlag=MF_STRING|MF_BYCOMMAND, nId=0, nPos ;
	char * psText ;
	long nTextLen = 0 ; 

	_pdr_menu_getresrc("rlls|l",__PDR_RESRC_MPARAM(&nPos,&nId,&psText,&nTextLen,&nFlag))
	pMenu->InsertMenu(nPos,nFlag,nId,psText) ;
}

ZEND_FUNCTION(pdr_menu_modify)
{
	long nFlag=MF_STRING|MF_BYCOMMAND, nId=0, nPos ;
	char * psText ;
	long nTextLen = 0 ; 

	_pdr_menu_getresrc("rlls|l",__PDR_RESRC_MPARAM(&nPos,&nId,&psText,&nTextLen,&nFlag))
	pMenu->ModifyMenu(nPos,nFlag,nId,psText) ;
}

ZEND_FUNCTION(pdr_menu_remove)
{
	long nPos,nPosFlag=MF_BYCOMMAND ;
	_pdr_menu_getresrc("rl|l",__PDR_RESRC_MPARAM(&nPos,&nPosFlag))
	pMenu->RemoveMenu(nPos,nPosFlag) ;
}

ZEND_FUNCTION(pdr_menu_enable)
{
	long nPos, nFlag=MF_ENABLED|MF_BYCOMMAND ;	// MF_DISABLED,MF_GRAYED,MF_ENABLED
	_pdr_menu_getresrc("rl|l",__PDR_RESRC_MPARAM(&nPos,&nFlag))
	pMenu->EnableMenuItem(nPos,nFlag) ;
}

ZEND_FUNCTION(pdr_menu_check)
{
	long nPos, nFlag=MF_CHECKED|MF_BYCOMMAND ; // MF_CHECKED,MF_UNCHECKED
	_pdr_menu_getresrc("rl|l",__PDR_RESRC_MPARAM(&nPos,&nFlag))
	pMenu->CheckMenuItem(nPos,nFlag) ;
}

ZEND_FUNCTION(pdr_menu_radio_check)
{
	long nFirstPos, nLastPos, nCheckedPos, nPosFlag=MF_BYCOMMAND ;
	_pdr_menu_getresrc("rlll|l",__PDR_RESRC_MPARAM(&nFirstPos,&nLastPos,&nCheckedPos,&nPosFlag))
	pMenu->CheckMenuRadioItem(nFirstPos,nLastPos,nCheckedPos,nPosFlag) ;
}

ZEND_FUNCTION(pdr_menu_popup)
{
	long nWnd, nX=-1, nY=-1, nFlag=TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL ;
	_pdr_menu_getresrc("rl|lll",__PDR_RESRC_MPARAM(&nWnd,&nX,&nY,&nFlag))

	if(nX<0||nY<0)
	{
		POINT pt ;
		if(::GetCursorPos(&pt))
		{
			if(nX<0)
			{
				nX = pt.x ;
			}
			if(nY<0)
			{
				nY = pt.y ;
			}
		}
	}

	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR Popup Menu: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}

	::SetForegroundWindow(hWnd) ;
	::TrackPopupMenu(pMenu->m_hMenu,nFlag,nX,nY,0,hWnd,0) ;
	::PostMessage(hWnd,WM_NULL,0,0); 
}

ZEND_FUNCTION(pdr_menu_get_hmenu)
{
	_pdr_menu_getresrc("r",)
	RETURN_LONG((long)pMenu->GetSafeHmenu()) ;
}


ZEND_FUNCTION(pdr_menu_item_count)
{
	_pdr_menu_getresrc("r",)
	RETURN_LONG(pMenu->GetMenuItemCount()) ;
}

ZEND_FUNCTION(pdr_menu_item_text)
{
	long nPos, nPosFlag=MF_BYCOMMAND ;
	_pdr_menu_getresrc("rl|l",__PDR_RESRC_MPARAM(&nPos,&nPosFlag))

	CString strText ;
	pMenu->GetMenuString(nPos,strText,nPosFlag) ;
	RETURN_STRING((char *)(LPCTSTR)strText,1) ;
}

ZEND_FUNCTION(pdr_menu_item_id)
{
	long nPos ;
	_pdr_menu_getresrc("rl",__PDR_RESRC_MPARAM(&nPos))
	RETURN_LONG(pMenu->GetMenuItemID(nPos)) ;
}
