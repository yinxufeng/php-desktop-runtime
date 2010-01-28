#include "stdafx.h"
#include "CPDR.h"
#include <Aclapi.h>
#include "pdr_smem.h"



class   CShareRestrictedSD       
  {   
  public:   
  CShareRestrictedSD();   
  virtual   ~CShareRestrictedSD();   
  SECURITY_ATTRIBUTES* GetSA();   
  protected:   
  PVOID     ptr;   
  SECURITY_ATTRIBUTES sa;   
  SECURITY_DESCRIPTOR   sd;   
  };   
  //////////////////////////////////////////////////////////////////////   
  //   Construction/Destruction   
  //////////////////////////////////////////////////////////////////////   
  PVOID   BuildRestrictedSD(PSECURITY_DESCRIPTOR   pSD)   {   
    
        DWORD     dwAclLength;   
    
        PSID       psidEveryone   =   NULL;   
    
        PACL       pDACL       =   NULL;   
        BOOL       bResult   =   FALSE;   
    
        PACCESS_ALLOWED_ACE   pACE   =   NULL;   
    
        SID_IDENTIFIER_AUTHORITY   siaWorld   =   SECURITY_WORLD_SID_AUTHORITY     ;   
          
        SECURITY_INFORMATION   si   =   DACL_SECURITY_INFORMATION;   
          
        __try   {   
    
              //   initialize   the   security   descriptor   
              if   (!InitializeSecurityDescriptor(pSD,     
                          SECURITY_DESCRIPTOR_REVISION))   {   
                    printf("InitializeSecurityDescriptor()   failed   with   error   %d\n",   
                                GetLastError());   
                    __leave;   
              }   
    
              //   obtain   a   sid   for   the   Authenticated   Users   Group   
              if   (!AllocateAndInitializeSid(&siaWorld,   1,     
                          SECURITY_WORLD_RID,   0,   0,   0,   0,   0,   0,   0,     
                          &psidEveryone))   {   
                    printf("AllocateAndInitializeSid()   failed   with   error   %d\n",   
                                GetLastError());   
                    __leave;   
              }   
    
              //   NOTE:   
              //     
              //   The   Authenticated   Users   group   includes   all   user   accounts   that   
              //   have   been   successfully   authenticated   by   the   system.   If   access   
              //   must   be   restricted   to   a   specific   user   or   group   other   than     
              //   Authenticated   Users,   the   SID   can   be   constructed   using   the   
              //   LookupAccountSid()   API   based   on   a   user   or   group   name.   
    
              //   calculate   the   DACL   length   
              dwAclLength   =   sizeof(ACL)   
                          //   add   space   for   Authenticated   Users   group   ACE   
                          +   sizeof(ACCESS_ALLOWED_ACE)   -   sizeof(DWORD)   
                          +   GetLengthSid(psidEveryone);   
    
              //   allocate   memory   for   the   DACL   
              pDACL   =   (PACL)   HeapAlloc(GetProcessHeap(),   HEAP_ZERO_MEMORY,     
                          dwAclLength);   
              if   (!pDACL)   {   
                    printf("HeapAlloc()   failed   with   error   %d\n",   GetLastError());   
                    __leave;   
              }   
    
              //   initialize   the   DACL   
              if   (!InitializeAcl(pDACL,   dwAclLength,   ACL_REVISION))   {   
                    printf("InitializeAcl()   failed   with   error   %d\n",     
                                GetLastError());   
                    __leave;   
              }   
                
              //   add   the   Authenticated   Users   group   ACE   to   the   DACL   with   
              //   GENERIC_READ,   GENERIC_WRITE,   and   GENERIC_EXECUTE   access   
              if   (!AddAccessAllowedAce(pDACL,   ACL_REVISION,   
                          GENERIC_ALL,   
                          psidEveryone))   {   
                    printf("AddAccessAllowedAce()   failed   with   error   %d\n",   
                                GetLastError());   
                    __leave;   
              }   
    
              //   set   the   DACL   in   the   security   descriptor   
              if   (!SetSecurityDescriptorDacl(pSD,   TRUE,   pDACL,   FALSE))   {   
                    printf("SetSecurityDescriptorDacl()   failed   with   error   %d\n",   
                                GetLastError());   
                    __leave;   
              }   
    
              bResult   =   TRUE;   
              
        }   __finally   {   
    
              if   (psidEveryone)   FreeSid(psidEveryone);   
        }   
    
        if   (bResult   ==   FALSE)   {   
              if   (pDACL)   HeapFree(GetProcessHeap(),   0,   pDACL);   
              pDACL   =   NULL;   
        }   
    
        return   (PVOID)   pDACL;   
  }   
    
  //   The   following   function   frees   memory   allocated   in   the   
  //   BuildRestrictedSD()   function   
  VOID   FreeRestrictedSD(PVOID   ptr)   {   
    
        if   (ptr)   HeapFree(GetProcessHeap(),   0,   ptr);   
    
        return;   
  }   
    
    
  CShareRestrictedSD::CShareRestrictedSD()   
  {   
  ptr=NULL;   
  sa.nLength   =   sizeof(sa);   
  sa.lpSecurityDescriptor   =   &sd;   
  sa.bInheritHandle   =   FALSE;   
  //   build   a   restricted   security   descriptor   
  ptr   =   BuildRestrictedSD(&sd);   
  if   (!ptr)   {   
  TRACE("BuildRestrictedSD()   failed\n");   
  }   
  }   
    
  CShareRestrictedSD::~CShareRestrictedSD()   
  {   
  if(ptr){   
  FreeRestrictedSD(ptr);   
  }   
  }   
  SECURITY_ATTRIBUTES* CShareRestrictedSD::GetSA()   
  {   
  if(ptr){   
  return   &sa;   
  }   
  else   
  return   NULL;   
  }   


void _pdr_smem_handle::WriteMemSize(unsigned long nSize)
{
	char szMemData[4] ;
	szMemData[0] = (nSize>>24) & 255 ;
	szMemData[1] = (nSize>>16) & 255 ;
	szMemData[2] = (nSize>>8) & 255 ;
	szMemData[3] = (nSize>>0) & 255 ;
	memcpy( pAccessPtr, szMemData, 4 ) ;
}
unsigned long _pdr_smem_handle::ReadMemSize()
{
	char szMemDataLen[4] ;
	memcpy( szMemDataLen, pAccessPtr, 4 ) ;
	return (szMemDataLen[0]<<24)
			|(szMemDataLen[1]<<16)
			|(szMemDataLen[2]<<8)
			|(szMemDataLen[3]<<0) ;
}
void _pdr_smem_handle::Write(char * pData,unsigned long nDataLen,unsigned long nSeek)
{
	char* pWriteAt = pAccessPtr + nSeek + 4 ;
	memcpy( pWriteAt, pData, nDataLen ) ;

	WriteMemSize(nDataLen+nSeek) ;
}
void _pdr_smem_handle::Read(char * pData,unsigned long nDataLen,unsigned long nSeek)
{
	char* pReadAt = pAccessPtr + nSeek + 4 ;
	memcpy( pAccessPtr, pReadAt, nDataLen ) ;
}
bool _pdr_smem_handle::Open(HANDLE hMemFile,unsigned long nSize)
{
	hMappingFile = hMemFile ;
	pAccessPtr = (char *)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,0,nSize) ;
	return pAccessPtr!=NULL ;
}
void _pdr_smem_handle::Release(bool bForceRelease)
{
	if( pAccessPtr )
	{
		if( ::UnmapViewOfFile(pAccessPtr) )
		{
			pAccessPtr = NULL ;
		}
		else
		{
			set_last_error ;
		}
	}

	if( hMappingFile && (bForceRelease||bAutoRelease) )
	{
		::CloseHandle(hMappingFile) ;
		hMappingFile = NULL ;
	}
}

ZEND_FUNCTION(pdr_smem_create)
{
	char * szName ;
	long nNameLen=0 ;
	long nSize=1024 ;
	long nProtect=PAGE_READWRITE ;
	bool bAutoRelease = true ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|llb", &szName,&nNameLen,&nSize,&nProtect,&bAutoRelease )==FAILURE )
	{
		RETURN_FALSE
	}

	///////////////////////////////////////////////////////////////////// 

	nSize+= 4 ;

	CShareRestrictedSD ShareRestrictedSD ;

	HANDLE hFile = INVALID_HANDLE_VALUE ;
	HANDLE hMapFile = CreateFileMapping(hFile,ShareRestrictedSD.GetSA(),nProtect,0,nSize,szName) ;
	DWORD dwErr = ::GetLastError() ;

	if ( !hMapFile||dwErr==ERROR_ALREADY_EXISTS )
	{
		_pdr_set_global_last_error(dwErr) ;
		RETURN_FALSE ;
	}

	pdr_smem_handle * pSMem = new pdr_smem_handle() ;
	if( !pSMem->Open(hMapFile,nSize) )
	{
		pSMem->Release() ;
		delete pSMem ;

		set_last_error ;
		RETURN_FALSE ;
	}

	pSMem->bAutoRelease = bAutoRelease ;
	pSMem->WriteMemSize(0) ;
	
	int nResrc = _pdr_get_resrc_smem() ;
	ZEND_REGISTER_RESOURCE( return_value, (void*)pSMem, nResrc )
}


ZEND_FUNCTION(pdr_smem_open)
{
	char * szName ;
	long nNameLen=0 ;
	long nSize=1024 ;
	long nProtect=PAGE_READWRITE ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &szName,&nNameLen,&nProtect,&nSize )==FAILURE )
	{
		RETURN_FALSE
	}

	nSize+= 4 ;

	HANDLE hMapFile = OpenFileMapping(nProtect,true,szName) ;
	if ( !hMapFile )
	{
		set_last_error ;
		RETURN_FALSE ;
	}

	pdr_smem_handle * pSMem = new pdr_smem_handle() ;
	if( !pSMem->Open(hMapFile,nSize) )
	{
		pSMem->Release() ;
		delete pSMem ;

		set_last_error ;
		RETURN_FALSE ;
	}

	pSMem->bAutoRelease = true ;

	int nResrc = _pdr_get_resrc_smem() ;
	ZEND_REGISTER_RESOURCE( return_value, (void*)pSMem, nResrc )
}

ZEND_FUNCTION(pdr_smem_get_size)
{
	PDR_GetShareMemoryHandleFromResrc("r",) ;
	RETURN_LONG(pSMemResrc->ReadMemSize()) ;
}

ZEND_FUNCTION(pdr_smem_release)
{
	PDR_GetShareMemoryHandleFromResrc("r",) ;
	pSMemResrc->Release(true) ;
}

ZEND_FUNCTION(pdr_smem_write)
{
	char * szData ;
	long nDataLen ;
	long nWriteLen=0 ;
	long nSeek=0 ;

	PDR_GetShareMemoryHandleFromResrc("rs|ll",__PDR_RESRC_MPARAM(&szData,&nDataLen,&nWriteLen,&nSeek)) ;
	
	if(!nWriteLen)
	{
		nWriteLen = nDataLen ;
	}

	pSMemResrc->Write(szData,nWriteLen,nSeek) ;
}

ZEND_FUNCTION(pdr_smem_read)
{
	long nReadLen=0 ;
	long nSeek=0 ;

	PDR_GetShareMemoryHandleFromResrc("r|ll",__PDR_RESRC_MPARAM(&nReadLen,&nSeek)) ;
	
	unsigned long nMemSize = pSMemResrc->ReadMemSize() ;
	if(!nReadLen)
	{
		nReadLen = nMemSize ;
	}

	if( nReadLen+nSeek > nMemSize )
	{
		RETURN_FALSE ;
	}

	char * pReadBuf = new char[nReadLen+1] ;

	pSMemResrc->Read(pReadBuf,nReadLen,nSeek) ;
	pReadBuf[nReadLen] = '\0' ;

	// ÒÔ×Ö·û´®·µ»Ø
	ZVAL_STRING(return_value,pReadBuf,1) ;

	delete [] pReadBuf ;
}
