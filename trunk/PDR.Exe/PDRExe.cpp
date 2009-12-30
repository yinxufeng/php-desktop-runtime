// PDRExe.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "sapi/embed/php_embed.h"

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.VC80.CRT' version='8.0.50608.0' processorArchitecture='X86' publicKeyToken='1fc8b3b9a1e18e3b' language='*'\"")



DWORD WINAPI pdrexe_thread_callback(LPVOID pParam)
{
	PHP_EMBED_START_BLOCK(0,NULL)

	char * script = " print 'Hello World!';";
	zend_eval_string(script, NULL, "Simple Hello World App" TSRMLS_CC);

	PHP_EMBED_END_BLOCK();

	return 0 ;
}

int _tmain(int argc, _TCHAR* argv[])
{

	DWORD dwThreadId = 0 ;
	
	HANDLE hThread = CreateThread(NULL,0,pdrexe_thread_callback,0,0,&dwThreadId) ;
	hThread = CreateThread(NULL,0,pdrexe_thread_callback,0,0,&dwThreadId) ;
	if(!hThread)
	{
	}

	while(1)
	{
		Sleep(1000) ;
	}

	return 0;
}