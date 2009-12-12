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

#ifndef __dhtml_api_H
# define __dhtml_api_H

#define WM_DHTML_EVENT WM_USER+1
#define WM_DHTML_EVENT_ACK WM_USER+2
#define WM_WIN32STD_TRAY_ICON WM_USER+3

#ifdef PHP_DHTML_EXPORT
#define PHP_DHTML_API __declspec(dllexport)
#else
#define PHP_DHTML_API
#endif

typedef struct _dhtml_event {
	long id;
	char type [256];
	char value[1024];
	char *extra;
	char **result; /* NULL for set */
} dhtml_event;

#ifdef __cplusplus
extern "C" {
#endif

PHP_DHTML_API DWORD dhtml_create(HANDLE *th_handle, HWND *wnd);
PHP_DHTML_API dhtml_event * dhtml_get_event( long wait );
PHP_DHTML_API int dhtml_post_message(DWORD winThreadId, const char *type, const char *value, const char *extra, char **return_value);
PHP_DHTML_API int dhtml_is_closed(HANDLE th_handle);
PHP_DHTML_API void dhtml_free_event( dhtml_event * evt );
PHP_DHTML_API int dhtml_set(DWORD winThreadId, const char *type, const char *value, const char *extra);
PHP_DHTML_API char * dhtml_get(DWORD winThreadId, const char *type, const char *value, const char *extra);

PHP_DHTML_API void dhtml_get_destop_size( long *x, long *y );

#ifdef __cplusplus
}
#endif

#endif