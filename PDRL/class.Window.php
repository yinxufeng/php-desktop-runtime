<?php 
namespace PDRL ;


class Window
{
	public function Window($hWnd)
	{
		$this->AttachHwnd($hWnd) ;
	}
		
	/**
	 * 关联一个 windows 窗口句柄
	 *
	 * @access	public
	 * @return	void
	 */
	public function AttachHwnd($hWnd)
	{
		$this->hWnd = $hWnd ;
	}
	/**
	 * 返回 windows 窗口句柄
	 *
	 * @access	public
	 * @return	int
	 */
	public function GetHwnd()
	{
		return $this->hWnd ;
	}
	
	/**
	 * 取得窗口的 long 值
	 * 
	 * 参数 $nIdx 可以为下列常量：
	 *  GWL_EXSTYLE
	 *  GWL_STYLE
	 *  GWL_WNDPROC
	 *  GWL_HINSTANCE
	 *  GWL_ID
	 *  GWL_USERDATA
	 *  DWL_DLGPROC
	 *  DWL_MSGRESULT
	 *  DWL_USER
	 * 其意义参考 MSDN
	 *
	 * @access	public
	 * @param	$nIdx		int		long值类型
	 * @return	int
	 */
	public function GetLong($nIdx)
	{
		return pdr_window_get_long($this->GetHwnd(),$nIdx) ;
	}
	/**
	 * 设置窗口的 long 值
	 *
	 * @access	public
	 * @param	$nIdx		int		long值类型
	 * @param	$nLong		int		long值
	 * @return	int
	 */
	public function SetLong($nIdx,$nLong)
	{
		pdr_window_set_long($this->GetHwnd(),$nIdx,$nLong) ;
	}
	

	/**
	 * 为窗口设置样式，返回窗口原有样式
	 *
	 * @access	public
	 * @param	$nStyles			int		样式
	 * @param	$bExStyle=false		bool	是否为 扩展样式（GWL_EXSTYLE）
	 * @return	int
	 */
	public function SetStyle($nStyles,$bExStyle=false)
	{
		$nHWnd =$this->GetHwnd() ;
		$nIdx = $bExStyle? GWL_EXSTYLE: GWL_STYLE ;
		$nOriLong = pdr_window_get_long($nHWnd,$nIdx) ;
		return pdr_window_set_long($nHWnd, $nIdx, $nOriLong|$nStyles) ;
	}
	/**
	 * 返回窗口是否已经拥有给入的样式
	 *
	 * @access	public
	 * @param	$nStyles			int		样式
	 * @param	$bExStyle=false		bool	是否为 扩展样式（GWL_EXSTYLE）
	 * @return	bool
	 */
	public function HasStyle($nStyles,$bExStyle=false)
	{
		$nOriLong = pdr_window_get_long($this->GetHwnd(),$bExStyle? GWL_EXSTYLE: GWL_STYLE) ;
		return ($nOriLong&$nStyles) ;
	}
	/**
	 * 取消窗口样式，返回窗口原有样式
	 *
	 * @access	public
	 * @param	$nStyles			int		样式
	 * @param	$bExStyle=false		bool	是否为 扩展样式（GWL_EXSTYLE）
	 * @return	int
	 */
	public function RemoveStyle($nStyles,$bExStyle=false)
	{
		$nHWnd =$this->GetHwnd() ;
		$nIdx = $bExStyle? GWL_EXSTYLE: GWL_STYLE ;
		$nOriLong = pdr_window_get_long($nHWnd,$nIdx) ;
		$nNewLong = $nOriLong&~$nStyles ;
		return pdr_window_set_long($nHWnd, $nIdx, $nNewLong) ;
	}

	/**
	 * 取得窗口或屏幕上指定坐标位置的颜色值
	 *
	 * @access	public
	 * @param	$nX				int		坐标x
	 * @param	$nY				int		坐标y
	 * @param	$bScreen=false	bool	是否全屏幕
	 * @return	string
	 */
	public function GetPixel($nX,$nY,$bScreen=false)
	{
		return pdr_window_get_pixel(
			$nX, $nY,
			$bScreen?0:$this->GetHwnd()
		) ;
	}
	
	/**
	 * 以指定方式 显示（隐藏）窗口
	 *
	 * @access	public
	 * @param	$nShowWnd=SW_SHOW	int
	 * @return	void
	 */
	public function Show($nShowWnd=SW_SHOW)
	{
		pdr_window_show( $this->GetHwnd(), $nShowWnd ) ;
	}
	
	/**
	 * 隐藏窗口
	 *
	 * @access	public
	 * @return	void
	 */
	public function Hide()
	{
		pdr_window_show( $this->GetHwnd(), SW_HIDE ) ;
	}
	
	
	/**
	 * 取回窗口左上角和右下角的坐标
	 *
	 * @access	public
	 * @return	array($nLeft,$nTop,$nRight,$nBottom)
	 */
	public function GetRect()
	{
		return pdr_window_get_rect( $this->GetHwnd() ) ;
	}
	
	
	/**
	 * 取回窗口客户区的 左上角和右下角的坐标
	 *
	 * @access	public
	 * @param	$bRetScreenCoord=true	bool	是否转换换为屏幕坐标返回
	 * @return	array($nLeft,$nTop,$nRight,$nBottom)
	 */
	public function GetClientRect($bRetScreenCoord=true)
	{
		return pdr_window_get_client_rect( $this->GetHwnd(), $bRetScreenCoord ) ;
	}

	/**
	 * 取回窗口尺寸
	 *
	 * @access	public
	 * @param	$bOnlyClient=false	bool	是否返回客户区坐标
	 * @return	array($nWidth,$nHeight)
	 */
	public function GetSize($bOnlyClient=false)
	{
		$arrRect = $bOnlyClient?
			$this->GetClientRect() :
			$this->GetRect() ;
		
		if( $arrRect===false )
		{
			return array(-1,-1) ;
		}
		
		else 
		{
			list($nL,$nT,$nR,$nB) = $arrRect ;
			return array($nR-$nL,$nB-$nT) ;
		}
	}	
	
	/**
	 * 取回窗口的线程id
	 *
	 * @access	public
	 * @return	int
	 */
	public function GetThread()
	{
		return pdr_window_get_thread( $this->GetHwnd() ) ;
	}

	/**
	 * 取回窗口的进程id
	 *
	 * @access	public
	 * @return	int
	 */
	public function GetProcess()
	{
		return pdr_window_get_process( $this->GetHwnd() ) ;
	}
	
	/**
	 * 将窗口提前到前台
	 *
	 * @access	public
	 * @param	$bRestore=true		bool
	 * @return	void
	 */
	public function Foreground($bRestore=true)
	{
		pdr_window_foreground( $this->GetHwnd(), $bRestore ) ;
	}

	/**
	 * 设置parent窗口
	 *
	 * @access	public
	 * @param	$Parent		int,Window
	 * @return	int
	 */
	public function SetParent($Parent)
	{
		$nParentHwnd = ($Parent instanceof self)? $Parent->GetHwnd(): intval($Parent) ;
		return pdr_window_set_parent( $this->GetHwnd(), $nParentHwnd ) ;
	}
	/**
	 * 取得parent窗口
	 *
	 * @access	public
	 * @return	Window
	 */
	public function GetParent()
	{
		$nParentHwnd = pdr_window_get_parent( $this->GetHwnd() ) ;
		return new self($nParentHwnd) ;
	}

	/**
	 * 设置窗口是否可用
	 *
	 * @access	public
	 * @param	$bEnable=true	bool
	 * @return	void
	 */
	public function Enable($bEnable=true)
	{
		pdr_window_enable( $this->GetHwnd(), $bEnable ) ;
	}
	/**
	 * 返回窗口是否可用
	 *
	 * @access	public
	 * @return	bool
	 */
	public function IsEnabled()
	{
		return pdr_window_is_enabled( $this->GetHwnd() ) ;
	}

	/**
	 * 检查窗口是否有效
	 *
	 * @access	public
	 * @return	bool
	 */
	public function Exists()
	{
		return pdr_window_exists( $this->GetHwnd() ) ;
	}


	/**
	 * 销毁窗口
	 *
	 * @access	public
	 * @return	bool
	 */
	public function Destroy()
	{
		return pdr_window_destroy( $this->GetHwnd() ) ;
	}
	
	/**
	 * 向窗口发送windows 消息，并等待窗口对此消息的处理结果
	 *
	 * @access	public
	 * @param	$nMsg		int		消息类型
	 * @param	$wParam		int		消息参数
	 * @param	$lParam		int		消息参数（长整形）
	 * @return	int
	 */
	public function SendMessage($nMsg, $wParam, $lParam)
	{
		return pdr_window_send_msg( $this->GetHwnd(), $nMsg, $wParam, $lParam ) ;
	}
	
	/**
	 * 向窗口发送windows 消息后立刻返回，不等待对消息的处理
	 *
	 * @access	public
	 * @param	$nMsg		int		消息类型
	 * @param	$wParam		int		消息参数
	 * @param	$lParam		int		消息参数（长整形）
	 * @return	int
	 */
	public function PostMessage($nMsg, $wParam, $lParam)
	{
		return pdr_window_post_msg( $this->GetHwnd(), $nMsg, $wParam, $lParam ) ;
	}

	
	/**
	 * 设置窗口的位置
	 *
	 * @access	public
	 * @param	$nX			int		坐标x
	 * @param	$nY			int		坐标y
	 * @param	$nFlags=0	int
	 * @return	void
	 */
	public function SetPos($nX,$nY,$nFlags=0)
	{
		$nFlags = $nFlags&~SWP_NOMOVE ;
		pdr_window_set_pos( $this->GetHwnd(), $nX,$nY, 0,0,0, $nFlags|SWP_NOSIZE|SWP_NOZORDER ) ;
	}
	/**
	 * 设置窗口的尺寸
	 *
	 * @access	public
	 * @param	$nW			int		宽
	 * @param	$nH			int		高
	 * @param	$nFlags=0	int
	 * @return	void
	 */
	public function SetSize($nW,$nH,$nFlags=0)
	{
		$nFlags = $nFlags&~SWP_NOSIZE ;
		pdr_window_set_pos( $this->GetHwnd(), 0,0, $nW,$nH ,0, $nFlags|SWP_NOMOVE|SWP_NOZORDER ) ;
	}
	/**
	 * 设置窗口顺序
	 *
	 * @access	public
	 * @param	$nInsertAfterHwnd	int		指定插入其下的窗口句柄
	 * @param	$nFlags=0	int
	 * @return	void
	 */
	public function SetOrder($nInsertAfterHwnd,$nFlags=0)
	{
		$nFlags = $nFlags&~SWP_NOZORDER ;
		pdr_window_set_pos( $this->GetHwnd(), 0,0,0,0 ,$nInsertAfterHwnd, $nFlags|SWP_NOMOVE|SWP_NOSIZE ) ;
	}

	/**
	 * 将窗口提到最上层（仍然低于置顶的窗口）
	 *
	 * @access	public
	 * @param	$nFlags=0	int
	 * @return	void
	 */
	public function Top($nFlags=0)
	{
		$nFlags = $nFlags&~SWP_NOZORDER ;
		pdr_window_set_pos( $this->GetHwnd(), 0,0,0,0 ,HWND_TOP, $nFlags|SWP_NOMOVE|SWP_NOSIZE ) ;
	}
	/**
	 * 将窗口放到最底层
	 *
	 * @access	public
	 * @param	$nFlags=0	int
	 * @return	void
	 */
	public function Bottom($nFlags=0)
	{
		$nFlags = $nFlags&~SWP_NOZORDER ;
		pdr_window_set_pos( $this->GetHwnd(), 0,0,0,0 ,HWND_BOTTOM, $nFlags|SWP_NOMOVE|SWP_NOSIZE ) ;
	}
	/**
	 * 设置（或取消）窗口置顶
	 *
	 * @access	public
	 * @param	$nFlags=0	int
	 * @return	void
	 */
	public function TopMost($bTopMost=true)
	{
		$nFlags = $nFlags&~SWP_NOZORDER ;
		pdr_window_set_pos( $this->GetHwnd(), 0,0,0,0 
				, $bTopMost? HWND_TOPMOST: HWND_NOTOPMOST
				, $nFlags|SWP_NOMOVE|SWP_NOSIZE
		) ;
	}

	/**
	 * 设置窗口的透明度
	 *
	 * @access	public
	 * @param	$nAlpha	int
	 * @return	void
	 */
	public function SetAlpha($nAlpha)
	{
		pdr_window_set_transparency($this->GetHwnd(),2,$nAlpha) ;
	}
	
	/**
	 * 设置窗口的透明颜色
	 *
	 * @access	public
	 * @param	$sColor		string
	 * @return	bool
	 */
	public function SetTransparency($sColor)
	{
		$arrResult = array() ;
		if( !preg_match('/^#?([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})$/i',$sColor,$arrResult) )
		{
			return false ;
		}
		
		pdr_window_set_transparency(
			$this->GetHwnd()
			, 1, 0
			, hexdec($arrResult[1])
			, hexdec($arrResult[2])
			, hexdec($arrResult[3])
		) ;
		
		return true ;
	}
		
	/**
	 * Description
	 * 
	 * @access	private
	 * @var		resource
	 */
	private $hWnd ;
}



function MsgBox($sMsg,$nType=MB_OK)
{
	return pdr_window_msg($sMsg,$nType) ;
}

?>