#include "stdafx.h"


ZEND_FUNCTION(pdr_rc_find)
{
	char *sName ;
	long nTypeId, nNameLen, nModuleHandle, nLang=0, nSubLang=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls|ll"
				, &nModuleHandle, &nTypeId
				, &sName, &nNameLen
				, &nLang, &nSubLang) == FAILURE )
	{
		RETURN_FALSE
	}

	if( !nLang && !nSubLang )
	{
		HRSRC hRsrc ;
		if( hRsrc=::FindResource((HMODULE)nModuleHandle,sName,MAKEINTRESOURCE(nTypeId)) )
		{
			RETURN_LONG((long)hRsrc)
		}

		else
		{
			RETURN_FALSE
		}
	}

	else
	{
		HRSRC hRsrc ;
		if( hRsrc=::FindResourceEx(
				(HMODULE)nModuleHandle
				, MAKEINTRESOURCE(nTypeId)
				, sName
				, MAKELANGID(nLang,nSubLang)
		))
		{
			RETURN_LONG((long)hRsrc)
		}

		else
		{
			RETURN_FALSE
		}
	}

}


ZEND_FUNCTION(pdr_rc_load)
{
	long nModuleHandle, nRsrcHandle ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &nModuleHandle, &nRsrcHandle ) == FAILURE )
	{
		RETURN_FALSE
	}

	HGLOBAL hGlobal = LoadResource((HMODULE)nModuleHandle,(HRSRC)nRsrcHandle) ;
	if(!hGlobal)
	{
		RETURN_FALSE
	}

	else
	{
		RETURN_LONG((long)hGlobal)
	}
}

ZEND_FUNCTION(pdr_rc_size)
{
	long nModuleHandle, nRsrcHandle ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &nModuleHandle, &nRsrcHandle ) == FAILURE )
	{
		RETURN_FALSE
	}

	DWORD nSize = SizeofResource((HMODULE)nModuleHandle,(HRSRC)nRsrcHandle) ;
	
	if(nSize)
	{
		RETURN_LONG((long)nSize)
	}
	else
	{
		RETURN_FALSE
	}
}

ZEND_FUNCTION(pdr_rc_lock)
{
	long nRsrcHandle ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nRsrcHandle ) == FAILURE )
	{
		RETURN_FALSE
	}

	void * pRsrc = LockResource((HRSRC)nRsrcHandle) ;
	
	if(pRsrc)
	{
		RETURN_LONG((long)pRsrc)
	}
	else
	{
		RETURN_FALSE
	}
}

ZEND_FUNCTION(pdr_rc_read)
{
	long nRsrcPtr, nPos, nLength ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &nRsrcPtr, &nPos, &nLength ) == FAILURE )
	{
		RETURN_FALSE
	}

	char * pBuffer = new char[nLength] ;
	char * pRsrc = (char *)nRsrcPtr ;
	pRsrc+= nPos ;

	::CopyMemory(pBuffer,pRsrc,nLength) ;

	zval * pzvRet ;
	MAKE_STD_ZVAL(pzvRet);
	ZVAL_STRINGL(pzvRet,pBuffer,nLength,1) ;

	delete[] pBuffer ;
	pRsrc = NULL ;

	RETURN_ZVAL(pzvRet,0,0)
}





// icon ////////////////////////////////


ZEND_FUNCTION(pdr_rc_filetype_icon)
{
	char * sFile ;
	long nFileLen ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &sFile, &nFileLen) == FAILURE )
	{
		RETURN_FALSE
	}

	SHFILEINFO sfi ;
	SHGetFileInfo(sFile, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES|SHGFI_ICON) ;

	RETURN_LONG((long)sfi.hIcon) ;
	/*zval * pzvRet ;
	MAKE_STD_ZVAL(pzvRet) ;
	object_init(pzvRet) ;
	add_property_long(pzvRet, "hIcon", (long)sfi.hIcon) ;
	add_property_long(pzvRet, "nRcIndex", (long)sfi.iIcon) ;
	add_property_long(pzvRet, "nAttributes", (long)sfi.dwAttributes) ;
	add_property_string(pzvRet, "sPath", sfi.szDisplayName,1) ;
	add_property_string(pzvRet, "sTypeName", sfi.szTypeName,1) ;

	RETURN_ZVAL(pzvRet,0,0) ;*/
}

struct TIconRec
{
	BYTE Width;
	BYTE Height;
	WORD Colors;
	WORD Reserved1;
	WORD Reserved2;
	long DIBSize;
	long DIBOffset;
};
struct TCursorOrIcon
{
	WORD Reserved;
	WORD wType;
	WORD Count;
};



static void InitBmpInfoHeader(HBITMAP Bitmap,
BITMAPINFOHEADER& BI, int nBit)
{
int Bytes;
DIBSECTION DS;

DS.dsBmih.biSize = 0;
Bytes = GetObject(Bitmap, sizeof(
DS), &DS);
if (Bytes>=sizeof(DS.dsBm)+sizeof(
DS.dsBmih) && DS.dsBmih.biSize
>=sizeof(DS.dsBmih))
memcpy(&BI, &DS.dsBmih, sizeof(
BITMAPINFOHEADER));
else {
memset(&BI, 0, sizeof(BI));
BI.biSize = sizeof(BI);
BI.biWidth = DS.dsBm.bmWidth;
BI.biHeight = DS.dsBm.bmHeight;
BI.biBitCount = DS.dsBm.bmPlanes
* DS.dsBm.bmBitsPixel;
}
if (nBit!=0) BI.biBitCount = nBit;
if (BI.biBitCount <= 8)
BI.biClrUsed = 1<<BI.biBitCount;
BI.biPlanes = 1;
if (BI.biClrImportant > BI.biClrUsed)
BI.biClrImportant = BI.biClrUsed;
if (BI.biSizeImage == 0)
BI.biSizeImage = ((BI.biWidth*BI.
biBitCount+31) / 32) * 4 * BI
.biHeight;
}
static void GetDIBSizes(HBITMAP Bitmap,
DWORD& InfoSize, DWORD& ImageSize,
int nBit)
{
BITMAPINFOHEADER BI;

InitBmpInfoHeader(Bitmap, BI, nBit);
InfoSize = sizeof(BITMAPINFOHEADER);
if (BI.biBitCount > 8) {
if (BI.biCompression&BI_BITFIELDS)
InfoSize += 12;
}
else
InfoSize += sizeof(RGBQUAD) * (BI
.biClrUsed!=0 ? BI.biClrUsed :
(1 << BI.biBitCount));
ImageSize = BI.biSizeImage;
}

static void GetDIB(HBITMAP Bitmap,
BITMAPINFO* BmpInfo, void* Bits, int nBit)
{
HDC DC;

DC = CreateCompatibleDC(NULL);
InitBmpInfoHeader(Bitmap, BmpInfo
->bmiHeader, nBit);
GetDIBits(DC, Bitmap, 0, BmpInfo
->bmiHeader.biHeight, Bits,
BmpInfo, DIB_RGB_COLORS);
DeleteDC(DC);
}

ZEND_FUNCTION(pdr_rc_save_icon)
{
	long nIconHanle ;
	char * sPath ;
	long nPathLen ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &nIconHanle,&sPath,&nPathLen) == FAILURE )
	{
		RETURN_FALSE
	}
	
	char * FileName = sPath ;
	HICON Icon = (HICON)nIconHanle ;

	BOOL ret;
	UINT nColor;
	long Length;
	void *MonoBits;
	void *ColorBits;
	DWORD dwWrite;
	HANDLE hFile;
	TIconRec List;
	ICONINFO IconInfo;
	TCursorOrIcon CI;
	DWORD MonoInfoSize;
	DWORD ColorInfoSize;
	DWORD MonoBitsSize;
	DWORD ColorBitsSize;
	BITMAPINFO *MonoInfo;
	BITMAPINFO *ColorInfo;

	int nBit = 32 ;
	switch(nBit)
	{
		case 0:
		case 1:
		case 4:
		case 8:
		case 16:
		case 24:
		case 32:
			break;
		default:
			RETURN_FALSE
	}

	hFile = CreateFile(FileName,
	GENERIC_WRITE, FILE_SHARE_READ,
	0, CREATE_ALWAYS, 0, 0);
	if (hFile==INVALID_HANDLE_VALUE)
	{
		RETURN_FALSE
	}

	memset(&CI, 0, sizeof(CI));
	memset(&List, 0, sizeof(List));
	GetIconInfo(Icon, &IconInfo);

	GetDIBSizes(IconInfo.hbmMask,
	MonoInfoSize, MonoBitsSize, 1);
	GetDIBSizes(IconInfo.hbmColor,
	ColorInfoSize, ColorBitsSize,
	nBit);
	MonoInfo = (BITMAPINFO*)malloc(
	MonoInfoSize);
	ColorInfo = (BITMAPINFO*)malloc(
	ColorInfoSize);
	MonoBits = malloc(MonoBitsSize);
	ColorBits = malloc(ColorBitsSize);
	GetDIB(IconInfo.hbmMask, MonoInfo,
	MonoBits, 1);
	GetDIB(IconInfo.hbmColor, ColorInfo,
	ColorBits, nBit);
	CI.wType = 0x10001; CI.Count = 1;
	ret &= WriteFile(hFile, &CI, sizeof(CI),
	&dwWrite, NULL);
	List.Width = ColorInfo->bmiHeader.biWidth;
	List.Height = ColorInfo->bmiHeader.biHeight;
	List.Colors = ColorInfo->bmiHeader.biPlanes
	* ColorInfo->bmiHeader.biBitCount;
	List.DIBSize = ColorInfoSize + ColorBitsSize
	+ MonoBitsSize;
	List.DIBOffset = sizeof(CI) + sizeof(List);
	ret &= WriteFile(hFile, &List, sizeof(List),
	&dwWrite, NULL);
	ColorInfo->bmiHeader.biHeight *= 2;
	ret &= WriteFile(hFile, ColorInfo,
	ColorInfoSize, &dwWrite, NULL);
	ret &= WriteFile(hFile, ColorBits,
	ColorBitsSize, &dwWrite, NULL);
	ret &= WriteFile(hFile, MonoBits,
	MonoBitsSize, &dwWrite, NULL);
	free(ColorInfo); free(MonoInfo);
	free(ColorBits); free(MonoBits);
	DeleteObject(IconInfo.hbmColor);
	DeleteObject(IconInfo.hbmMask);

	CloseHandle(hFile);
	//return ret;

/*
	PICTDESC picdesc;
	picdesc.cbSizeofstruct = sizeof(PICTDESC);
	picdesc.picType = PICTYPE_ICON ;            
	picdesc.icon.hicon = (HICON)nIconHanle ;

	IPicture* pPicture=NULL ;
	OleCreatePictureIndirect(&picdesc, IID_IPicture, TRUE,(VOID**)&pPicture) ;

	LPSTREAM pStream ;
	CreateStreamOnHGlobal(NULL,TRUE,&pStream) ;

	LONG size ;
	HRESULT hr=pPicture->SaveAsFile(pStream,TRUE,&size) ;

	CFile iconfile;
	iconfile.Open(sPath, CFile::modeCreate|CFile::modeWrite);
	LARGE_INTEGER li;
	li.HighPart =0;
	li.LowPart =0;
	ULARGE_INTEGER ulnewpos;
	pStream->Seek( li,STREAM_SEEK_SET,&ulnewpos);
	ULONG uReadCount = 1;
	while(uReadCount>0)
	{
		pStream->Read(sPath,sizeof(sPath),&uReadCount);
		if(uReadCount>0)
		{
			iconfile.Write(sPath,uReadCount);
		}
	}
	pStream->Release();
	iconfile.Close();*/
}