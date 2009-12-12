<?php 
namespace PDRL ;


class DHtmlDialog extends DHtmlWindow
{
	public function DHtmlDialog($bWndDecorator=true)
	{
		parent::DHtmlWindow($bWndDecorator) ;
	}
	
	/**
	 * 进入对话框  模态 状态
	 *
	 * @access	public
	 * @return	int
	 */
	public function DoModal()
	{
		$nResponse = pdr_dhtml_domodal($this->GetHandle()) ;
		return $nResponse==IDOK ;
	}
	
	/**
	 * 结束对话框 模态，DoModal() 返回 ID_OK
	 *
	 * @access	public
	 * @return	void
	 */
	public function EndModalOk() 
	{
		pdr_dhtml_ok($this->GetHandle()) ;
	}
		
	/**
	 * 结束对话框 模态，DoModal() 返回 ID_CANCEL
	 *
	 * @access	public
	 * @return	void
	 */
	public function EndModalCancel() 
	{
		pdr_dhtml_cancel($this->GetHandle()) ;
	}

}


?>