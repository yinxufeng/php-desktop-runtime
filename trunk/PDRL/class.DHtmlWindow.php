<?php 
namespace PDRL ;


class DHtmlWindow
{
	const DLG_ELEMENT_ID = '@dialog' ;

	public function DHtmlWindow($bWndDecorator=true)
	{
		$this->hDlg = pdr_dhtml_new($bWndDecorator) ;

		pdr_dhtml_set_handle($this->hDlg,DHTML_ONINITDIALOG,'@dialog',array($this,'OnInitWindow')) ;
		pdr_dhtml_set_handle($this->hDlg,DHTML_ONBEFORENAVIGATE,'@dialog',array($this,'OnBeforeNavigate')) ;
		pdr_dhtml_set_handle($this->hDlg,DHTML_ONNAVIGATECOMPLETE,'@dialog',array($this,'OnNavigateComplete')) ;
		pdr_dhtml_set_handle($this->hDlg,DHTML_ONDOCUMENTCOMPLETE,'@dialog',array($this,'OnDocumentComplete')) ;
				
		$this->aScriptBrige = new DHtmlScriptBrige($this) ;
	}
	
	/**
	 * Description
	 *
	 * @access	public
	 * @return	DHtmlScriptBrige
	 */
	public function GetScriptBrige()
	{
		return $this->aScriptBrige ;
	}
	
	/**
	 * 创建窗口
	 *
	 * @access	public
	 * @param	$nShow=SW_SHOWNORMAL	int
	 * @return	void
	 */
	public function Create($nShow=SW_SHOWNORMAL)
	{
		pdr_dhtml_create($this->hDlg,$nShow) ;
	}
	
	/**
	 * 取回窗口
	 *
	 * @access	public
	 * @return	Window
	 */
	public function GetWindow()
	{
		$aWin = new Window($this->GetHwnd()) ;
		return $aWin->Exists()? $aWin: NULL ; 
	}

	/**
	 * 事件：窗口初始化完成
	 *
	 * @access	public
	 * @param	$aEventOb	object	事件数据
	 * @return	int
	 */
	public function OnInitWindow($aEventOb)
	{}
	/**
	 * 事件：打开网页之前
	 *
	 * @access	public
	 * @param	$aEventOb	object	事件数据
	 * @return	int
	 */
	public function OnBeforeNavigate($aEventOb)
	{}
	/**
	 * 事件：网页已经读取完成
	 *
	 * @access	public
	 * @param	$aEventOb	object	事件数据
	 * @return	int
	 */
	public function OnNavigateComplete($aEventOb)
	{}
	/**
	 * 事件：网页文档已经完成
	 *
	 * @access	public
	 * @param	$aEventOb	object	事件数据
	 * @return	int
	 */
	public function OnDocumentComplete($aEventOb)
	{
		$this->GetScriptBrige()->OnDocumentComplete() ;
	}

	/**
	 * 取回 dhtml dialog 句柄
	 *
	 * @access	public
	 * @return	resource
	 */
	public function GetHandle()
	{
		return $this->hDlg ;
	}
	
	/**
	 * 取回窗口的windows句柄
	 *
	 * @access	public
	 * @return	resource
	 */
	public function GetHwnd()
	{
		return pdr_dhtml_get_hwnd($this->GetHandle()) ;
	}

	/**
	 * 设置可拖拽窗口的 html 元素
	 *
	 * @access	public
	 * @param	$sElementName	string	元素id
	 * @return	void
	 */
	public function SetDraggable($sElementName)
	{
		pdr_dhtml_set_handle($this->hDlg,DHTML_ONSELECTSTART,$sElementName,array($this,'OnDraggingStart')) ;
	}

	/**
	 * 事件：拖拽开始
	 *
	 * @access	public
	 * @param	$aEventOb	object	事件数据
	 * @return	int
	 */
	public function OnDraggingStart($aEventOb)
	{		
		$nWnd = pdr_dhtml_get_hwnd($this->hDlg) ;
		
		// 消息 WM_NCLBUTTONDOWN , 参数 HTCAPTION
		pdr_window_send_msg($nWnd,0x00A1,2) ;
		
		// 返回  S_FALSE 禁止 dhtml 响应此消息
		return S_FALSE ;
	}
	
	/**
	 * 打开网页
	 *
	 * @access	public
	 * @param	$sUrl				string
	 * @param	$nFlag=0			int
	 * @param	$sTargetFrame=''	string
	 * @param	$sHeaders=''		string
	 * @param	$sPostData=''		string
	 * @return	void
	 */
	public function Navigate($sUrl,$nFlag=0,$sTargetFrame='',$sHeaders='',$sPostData='')
	{
		pdr_dhtml_navigate($this->GetHandle(),$sUrl,$nFlag,$sTargetFrame,$sHeaders,$sPostData) ;		
	}
	
	/**
	 * 设置事件响应函数
	 *
	 * @access	public
	 * @param	$sElementId		string
	 * @param	$nEvent=0		int
	 * @param	$Callback		callback
	 * @return	void
	 */
	public function SetEventHandle($sElementId,$nEvent,$Callback) 
	{
		$arrApiArgs = array(
			$this->GetHandle()
			, &$nEvent
			, &$sElementId
			, &$Callback		
		) ;
		
		if(func_num_args()>3)
		{
			$arrFuncArgs = func_get_args() ;
			$arrApiArgs = array($arrApiArgs,array_slice($arrFuncArgs,3)) ;
		}
		
		call_user_func_array( 'pdr_dhtml_set_handle', $arrApiArgs ) ;
	}
	
	/**
	 * 取回 html元素的 innerHTML
	 *
	 * @access	public
	 * @param	$sElementId		string
	 * @return	string
	 */
	public function GetElementHtml($sElementId)
	{
		return pdr_dhtml_get_html($this->GetHandle(),$sElementId) ;
	} 
	/**
	 * 设置 html元素的 innerHTML
	 *
	 * @access	public
	 * @param	$sElementId		string
	 * @param	$sHtml			string
	 * @return	string
	 */
	public function SetElementHtml($sElementId,$sHtml)
	{
		return pdr_dhtml_set_html($this->GetHandle(),$sElementId,$sHtml) ;
	} 
	
	/**
	 * 取回 html元素的 value
	 *
	 * @access	public
	 * @param	$sElementId		string
	 * @return	string
	 */
	public function GetElementValue($sElementId)
	{
		return pdr_dhtml_get_value($this->GetHandle(),$sElementId) ;
	} 
	/**
	 * 设置 html元素的 value
	 *
	 * @access	public
	 * @param	$sElementId		string
	 * @param	$sValue			string
	 * @return	string
	 */
	public function SetElementValue($sElementId,$sValue)
	{
		return pdr_dhtml_set_value($this->GetHandle(),$sElementId,$sValue) ;
	} 
	
	/**
	 * 取回 html元素的 text
	 *
	 * @access	public
	 * @param	$sElementId		string
	 * @return	string
	 */
	public function GetElementText($sElementId)
	{
		return pdr_dhtml_get_text($this->GetHandle(),$sElementId) ;
	} 
	
	/**
	 * 设置 html元素的 text
	 *
	 * @access	public
	 * @param	$sElementId		string
	 * @param	$sText			string
	 * @return	string
	 */
	public function SetElementText($sElementId,$sText)
	{
		return pdr_dhtml_set_text($this->GetHandle(),$sElementId,$sText) ;
	} 
	
	/**
	 * Description
	 * 
	 * @access	private
	 * @var		resource
	 */
	private $hDlg ;
	
	/**
	 * Description
	 * 
	 * @access	private
	 * @var		DHtmlScriptBrige
	 */
	private $aScriptBrige ;
	
}


?>