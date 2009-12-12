/**
 * php_dhtml
 * Copyright (C) 2004-2005  Eric Colinet <ecolinet - gmail - com>
 * http://wildphp.free.fr/wiki/doku.php?id=php_dhtml:index
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include "stdafx.h"
#include "dhtml_api.h"
#include "DHtmlMfcThread.h"

#define DHTML_MB( str ) MessageBox( NULL, str, "dhtml", MB_OK )

extern "C" const char * win32_strerror()
{
	static char Buf[256];

	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, 
		GetLastError(), 0, Buf, 256, NULL);
	return Buf;
}


DWORD dhtml_create(HANDLE *th_handle, HWND *wnd)
{
	HANDLE event;
	DHtmlMfcThread * thread= new DHtmlMfcThread();
	event= CreateEvent(NULL, FALSE, FALSE, "DHTML Wnd Create Notification");

	thread->m_sync_event= event;
	thread->m_parent_thread_id= GetCurrentThreadId();

	if( !thread->CreateThread() ) return 0;
	WaitForSingleObject(event, INFINITE);
	
	if(th_handle) *th_handle= thread->m_hThread;
	if( wnd ) *wnd= thread->m_pMainWnd->GetSafeHwnd();
	return thread->m_nThreadID;
}


dhtml_event * dhtml_get_event( long wait )
{
	BOOL ret;
	MSG msg;

	if( wait )
		ret= GetMessage(&msg, (HWND)INVALID_HANDLE_VALUE, WM_DHTML_EVENT, WM_DHTML_EVENT);
	else
		ret= PeekMessage(&msg, (HWND)INVALID_HANDLE_VALUE, WM_DHTML_EVENT, WM_DHTML_EVENT, PM_REMOVE);
	if( ret==-1 ) {
		printf( "Error getting message %s\n", win32_strerror() );
		return 0;
	}

	return (dhtml_event*)msg.lParam;
}


int dhtml_post_message(DWORD winThreadId, const char *type, const char *value, const char *extra, char **return_value)
{
	if( !type || !*type ) return 0;
	dhtml_event * event= (dhtml_event *)malloc(sizeof(dhtml_event));

	event->id= (long)GetCurrentThreadId();

	strncpy( event->type, type, 254 );
	event->type[254]= 0;

	if( value && *value )
	{
		strncpy( event->value, value, 1023 );
		event->value[1023]= 0;
	}
	else
		event->value[0]= 0;

	if( extra && *extra ) // Not binary safe
	{
        int len= strlen(extra);
        event->extra= (char*)malloc(sizeof(char)*(len+1));
        if( event->extra ) {
		    strncpy( event->extra, extra, len );
			event->extra[len]= 0;
        }
	}
	else
		event->extra= NULL;

	event->result= return_value;

	::PostThreadMessage( winThreadId, WM_DHTML_EVENT, (WPARAM)GetCurrentThreadId(), (LPARAM)event );
	if( return_value ) {
		MSG msg;
		BOOL ret= GetMessage(&msg, (HWND)INVALID_HANDLE_VALUE, WM_DHTML_EVENT_ACK, WM_DHTML_EVENT_ACK);
		if( !ret ) return 0;
		return (int)msg.wParam; //  wParam is succes or error
	}
	return 1; // Ok
}


int dhtml_set(DWORD winThreadId, const char *type, const char *value, const char *extra) {
	return dhtml_post_message(winThreadId, type, value, extra, NULL);
}


char * dhtml_get(DWORD winThreadId, const char *type, const char *value, const char *extra) {
	char * ret;
	int res;
	res= dhtml_post_message(winThreadId, type, value, extra, &ret);
	if( !res ) return NULL;
	return ret;
}


void dhtml_free_event( dhtml_event * evt ) {
	if( !evt ) return;
	if( evt->extra ) free(evt->extra);
	free(evt);
}


int dhtml_is_closed(HANDLE thread)
{
	DWORD exit_code;
	BOOL status;

	status= GetExitCodeThread(thread, &exit_code);

	if( !status ) {
		printf("GetExitCode of thread %d failed: %s\n", thread, win32_strerror());
		return 1;
	}

	if( exit_code==STILL_ACTIVE ) {
		return 0;
	}
	
	return 1;
}


void dhtml_get_destop_size( long *x, long *y ) {
	RECT rect;
	HWND desk= GetDesktopWindow();
	GetWindowRect(desk, &rect);
	if( x ) *x= rect.right;
	if( y ) *y= rect.bottom;
}
