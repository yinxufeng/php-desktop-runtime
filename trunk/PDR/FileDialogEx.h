

/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx dialog

class CFileDialogEx : public CFileDialog
{
public:
	CFileDialogEx(
		BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL,
		DWORD dwSize = 0 ,
		LPCTSTR psTitle = NULL
	);

	LPCTSTR m_psTitle ;
	virtual void OnInitDone( );
};