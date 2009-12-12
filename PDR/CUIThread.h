#pragma once
#include "pdr_thread.h"


// CUIThread
class CUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CUIThread)

private:
	pdr_window_thread_handle* m_pInitHandle ;
	pdr_window_thread_handle* m_pExitHandle ;

protected:
	CUIThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CUIThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void SetEventHandle(pdr_window_thread_handle* pInitHandle,pdr_window_thread_handle* pExitHandle)
	{
		m_pInitHandle = pInitHandle ;
		m_pExitHandle = pExitHandle ;
	}
protected:
	DECLARE_MESSAGE_MAP()
};
