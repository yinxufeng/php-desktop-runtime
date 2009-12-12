#pragma once
#include "Resource.h"
#include "CDynDHtmlEventMap.h"
#include "pdr_dhtml.h"



#define DLG_EVENT_START 0x0001
#define DLG_EVENT_ONINITDIALOG 0x0001
#define DLG_EVENT_ONBEFORENAVIGATE 0x0002
#define DLG_EVENT_ONNAVIGATECOMPLETE 0x0003
#define DLG_EVENT_ONDOCUMENTCOMPLETE 0x0004
#define DLG_EVENT_ONCOMMAND 0x0005
#define DLG_EVENT_END 0x0005


#define TRAY_EVENT_ONMOUSEMOVE		WM_MOUSEMOVE
#define TRAY_EVENT_ONLBUTTONDOWN	WM_LBUTTONDOWN
#define TRAY_EVENT_ONLBUTTONUP		WM_LBUTTONUP
#define TRAY_EVENT_ONLBUTTONDBLCLK	WM_LBUTTONDBLCLK
#define TRAY_EVENT_ONRBUTTONDOWN	WM_RBUTTONDOWN
#define TRAY_EVENT_ONRBUTTONUP		WM_RBUTTONUP
#define TRAY_EVENT_ONRBUTTONDBLCLK	WM_RBUTTONDBLCLK
#define TRAY_EVENT_ONMBUTTONDOWN	WM_MBUTTONDOWN
#define TRAY_EVENT_ONMBUTTONUP		WM_MBUTTONUP
#define TRAY_EVENT_ONMBUTTONDBLCLK	WM_MBUTTONDBLCLK


#define CDYNDHEM_FUNC(f) ((DHEVTFUNCCONTROL) (DHEVTFUNC)f)


#define ELEMENT_ID_DIALOG "@dialog" 
#define ELEMENT_ID_DOCUMENT "@document" 




// CDHtmlDlg 对话框

class CDHtmlDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDHtmlDlg)

public:
	CDHtmlDlg(UINT nIDTemplate=IDD_DHTMLDLG,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDHtmlDlg();
// 重写
	HRESULT CDHtmlDlg::OnHtmlEvent(IHTMLElement* pElement) ;
	virtual BOOL IsExternalDispatchSafe(); 

// 对话框数据
	enum { IDD = IDD_DHTMLDLG, IDD_NOBORDER = IDD_DHTMLDLG_NOBORDER, IDH = IDR_HTML_DHTMLDLG };

	LPCTSTR GetDialogTemplate() ;
	void SetElementEventHandle(long nEventType,char * pElemId,pdr_callback_handle * pEventHandle) ;
	BSTR CallPHPFunction(CString strFuncName,CString strArgsJson) ;
	BSTR ExecPHP(CString strPHPCode) ;


	virtual void OnOK();
	virtual void OnCancel();

	
	CDynDHtmlEventMap m_dynMap ;

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()


public:
	virtual void OnBeforeNavigate(LPDISPATCH pDisp, LPCTSTR szUrl) ;
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl) ;
	virtual void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl) ;
	
	afx_msg LRESULT OnTrayIcon(WPARAM id, LPARAM message);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};



CString GetDHtmlEventName(long nType) ;

