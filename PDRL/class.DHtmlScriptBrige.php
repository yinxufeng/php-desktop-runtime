<?php 
namespace PDRL ;

class DHtmlScriptBrige
{
	/**
	 * Description
	 *
	 * @access	public
	 * @param	$aDHtmlDlg	DHtmlDialog
	 * @return	void
	 */
	public function DHtmlScriptBrige(DHtmlDialog $aDHtmlDlg)
	{
		$this->aDHtmlDlg = $aDHtmlDlg ;
		
		$sResrcId = strval($aDHtmlDlg->GetHandle()) ;
		self::$arrGlobalInstances[$sResrcId] = $this ;
	}
	
	/**
	 * Description
	 *
	 * @access	public
	 * @param	$sFuncName
	 * @param	$arrArgvs=array()	array
	 * @return	mixed
	 */
	public function __call($sFuncName,array $arrArgvs=array())
	{
		$sEncodeArgs = json_encode($arrArgvs) ;
		$sEncodeArgs = substr($sEncodeArgs,1,strlen($sEncodeArgs)-2) ;
		 
		echo $sEncodeArgs = json_encode( array($sFuncName,$sEncodeArgs) ) ;
		return pdr_dhtml_call( $this->aDHtmlDlg->GetHandle(), '__PDRLInterface_InvokeFromHost', $sEncodeArgs ) ;
	}
	
	/**
	 * Description
	 *
	 * @access	public
	 * @param	$sVarName
	 * @param	$sValue
	 * @return	void
	 */
	public function __set($sVarName,$sValue)
	{
		$sEncodeArgvs = json_encode( array($sVarName, json_encode($sValue)) ) ;
		pdr_dhtml_call( $this->aDHtmlDlg->GetHandle(), '__PDRLInterface_SetVariableFromHost', $sEncodeArgvs ) ;
	}
	
	/**
	 * Description
	 *
	 * @access	public
	 * @param	$sVarName
	 * @return	mixed
	 */
	public function __get($sVarName)
	{
		$sValue = pdr_dhtml_call( $this->aDHtmlDlg->GetHandle(), '__PDRLInterface_GetVariableFromHost', $sVarName ) ;
		return ($sValue===null)? null: json_decode($sValue) ;
	}

	/**
	 * Description
	 *
	 * @access	public
	 * @param	$Parameter
	 * @return	void
	 */
	public function __isset($sVarName)
	{
		return pdr_dhtml_call( $this->aDHtmlDlg->GetHandle(), '__PDRLInterface_IssetVariableFromHost', $sVarName ) ;
	}
	
	/**
	 * Description
	 *
	 * @access	public
	 * @param	$Parameter
	 * @return	void
	 */
	public function __unset($sVarName)
	{
		return pdr_dhtml_call( $this->aDHtmlDlg->GetHandle(), '__PDRLInterface_UnsetVariableFromHost', $sVarName ) ;
	}
	
	/**
	 * 执行一段 js脚本
	 *
	 * @access	public
	 * @param	$sScript		string
	 * @return	bool
	 */
	public function ExecuteScript($sScript)
	{
		return pdr_dhtml_exec_script($this->aDHtmlDlg->GetHandle(),$sScript) ;
	}
	
	/**
	 * Description
	 *
	 * @access	public
	 * @return	void
	 */
	public function OnDocumentComplete()
	{
		$this->ExecuteScript(self::$sJSInitCode) ;		
	}

	/**
	 * Description
	 *
	 * @access	public
	 * @static
	 * @return	mixed
	 */
	static public function OnInvokeFromJavaScript($sEncodeArgvs)
	{
		$arrArgvs = json_decode($sEncodeArgvs) ;
		$sResrcId = $arrArgvs[0] ;
		$sFuncName = $arrArgvs[1] ;

		if( !isset(self::$arrGlobalInstances[$sResrcId]) )
		{
			MsgBox("指定的DHtml窗口对象不存在：{$sResrcId}") ;
			return ;
		}
		$aDHtmlWnd = self::$arrGlobalInstances[$sResrcId] ;
		
		if( !$callback=$aDHtmlWnd->GetScriptFunctionCallback($sFuncName) )
		{
			MsgBox("函数{$sFuncName}对应的回调函数丢失。") ;
			return ;
		}

		return call_user_func_array($callback,array_slice($arrArgvs,2)) ;
	}

	/**
	 * Description
	 *
	 * @access	public
	 * @param	$sFunctionName
	 * @param	$Callback
	 * @return	void
	 */
	public function RegisterScriptFunction($sFunctionName,$Callback)
	{
		$this->arrScriptFunctions[$sFunctionName] = $Callback ;
		$sResrcId = strval($this->aDHtmlDlg->GetHandle()) ;

		// 创建 javascript 函数
		$sThisClassName = str_replace('\\','\\\\',__CLASS__) ;
		$sCreateScriptFunction = "{$sFunctionName} = function(){
	arrInArgvs = new Array('{$sResrcId}','{$sFunctionName}') ;
	for(var i=0;i<arguments.length;i++)
	{
		arrInArgvs.push(arguments[i]) ;
	}
	return external.call( '{$sThisClassName}::OnInvokeFromJavaScript', PDRL.json_encode(arrInArgvs) )
}" ;
		$this->ExecuteScript($sCreateScriptFunction) ;
	}

	/**
	 * Description
	 *
	 * @access	public
	 * @param	$sFunctionName
	 * @return	void
	 */
	public function UnregisterScriptFunction($sFunctionName)
	{
		unset($this->arrScriptFunctions[$sFunctionName]) ;
		$this->ExecuteScript("{$sFunctionName} = null ;") ;
	}
	
	/**
	 * Description
	 *
	 * @access	public
	 * @param	$sFunctionName
	 * @return	callback
	 */
	public function GetScriptFunctionCallback($sFunctionName)
	{
		return isset($this->arrScriptFunctions[$sFunctionName])?
					$this->arrScriptFunctions[$sFunctionName]: null ;
	}
	
	/**
	 * Description
	 * 
	 * @access	private
	 * @static
	 * @var		array
	 */
	static private $arrGlobalInstances = array() ;
	
	/**
	 * Description
	 * 
	 * @access	private
	 * @var		array
	 */
	private $arrScriptFunctions = array() ;
	
	/**
	 * Description
	 * 
	 * @access	private
	 * @static
	 * @var		string
	 */
	static private $sJSInitCode = "var PDRL = {} ;
PDRL.json_encode = function(arr) {
	sVarType=typeof arr ;
	if( sVarType!='object' && sVarType!='[object Array]' )
	{
		if(sVarType=='string')
		{
			return  '\"' + arr + '\"' ;
		}
		else
		{
			return arr.toString() ; 
		}
	}

    var parts = [];
    var is_list = (Object.prototype.toString.apply(arr) === '[object Array]');
    for(var key in arr) {
    	var value = arr[key];
        if(typeof value == 'object') {
            if(is_list) parts.push(PDRL.json_encode(value));
            else parts[key] = PDRL.json_encode(value);
        } else {
            var str = '';
            if(!is_list) str = '\"' + key + '\":';
            if(typeof value == 'number') str += value;
            else if(value === false) str += 'false';
            else if(value === true) str += 'true';
            else str += '\"' + value + '\"';
            parts.push(str);
        }
    }
    var json = parts.join(',');
    if(is_list) return '[' + json + ']';
    return '{' + json + '}';
}
PDRL.call = function(sFuncname)
{
	arrInArgvs = Array.apply(null,arguments) ;
	sArgvs = PDRL.json_encode(arrInArgvs.splice(0,1)) ;
	return external.call(sFuncname,'123') ;
}
__PDRLInterface_InvokeFromHost = function(sEncodeInArgvs)
{
	eval( 'var arrInArgvs = ' + sEncodeInArgvs ) ;
	var sFuncname = arrInArgvs[0] ;
	var sEncodeArgvs = arrInArgvs[1] ;
	eval( \"var Ret = \"+sFuncname+\"(\"+sEncodeArgvs+\")\" ) ;
	return Ret ;
}
__PDRLInterface_SetVariableFromHost = function(sEncodeInArgvs)
{
	eval( 'var arrInArgvs = ' + sEncodeInArgvs ) ;
	var sVarName = arrInArgvs[0] ;
	var sEncodeValue = arrInArgvs[1] ;
	
	eval( sVarName+'='+sEncodeValue ) ;
}
__PDRLInterface_GetVariableFromHost = function(sVarName)
{
	// 屏蔽错误
	eval( \"var RetVariable ; if(typeof(\"+sVarName+\")!=\'undefined\'){RetVariable=\" + sVarName + '}else{RetVariable=null}' ) ;
	
	// 不屏蔽错误（访问不存在的变量会触发ie脚本错误）
	// eval( 'var RetVariable = '+sVarName ) ;
	
	if(RetVariable!=null)
	{
		return PDRL.json_encode(RetVariable) ;
	} ;
}
__PDRLInterface_IssetVariableFromHost = function(sVarName)
{
	eval(\"var bRet = typeof(\"+sVarName+\")!='undefined'\") ;
	return bRet ;
}
__PDRLInterface_UnsetVariableFromHost = function(sVarName)
{
	eval( sVarName + '=null;' ) ;
}
" ;
	
	/**
	 * Description
	 * 
	 * @access	private
	 * @var		DHtmlDialog
	 */
	private $aDHtmlDlg ;
}
?>