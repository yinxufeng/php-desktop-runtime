

/////////////////////////////////////////////////////////////////////////////
// CFolderDialog dialog

class CFolderDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CFolderDialog)

public:
	static WNDPROC m_wndProc;
	virtual void OnInitDone( );
	CString* m_pPath;
	LPCTSTR m_psTitle ;
	CFolderDialog(CString* pPath,LPCTSTR psPath=NULL,LPCTSTR psTitle=NULL);

protected:
	//{{AFX_MSG(CFolderDialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

