// PDRExe.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "sapi/embed/php_embed.h"


int _tmain(int argc, _TCHAR* argv[])
{

	PHP_EMBED_START_BLOCK(argc,argv)


	char * script = " print 'Hello World!';";
	zend_eval_string(script, NULL, "Simple Hello World App" TSRMLS_CC);

	PHP_EMBED_END_BLOCK();


	return 0;
}

