#define	_pdr_define_const \
	/*	窗口属性 */\
	REGISTER_LONG_CONSTANT("GWL_EXSTYLE", GWL_EXSTYLE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("GWL_STYLE", GWL_STYLE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("GWL_HINSTANCE", GWL_HINSTANCE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("GWL_HWNDPARENT", GWL_HWNDPARENT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("GWL_USERDATA", GWL_USERDATA, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("GWL_ID", GWL_ID, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DWL_MSGRESULT", DWL_MSGRESULT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DWL_USER", DWL_USER, CONST_CS | CONST_PERSISTENT);\
 \
	/* 窗口样式 */\
	REGISTER_LONG_CONSTANT("WS_OVERLAPPED", WS_OVERLAPPED, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_POPUP", WS_POPUP, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_CHILD", WS_CHILD, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_MINIMIZE", WS_MINIMIZE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_VISIBLE", WS_VISIBLE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_DISABLED", WS_DISABLED, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_CLIPSIBLINGS", WS_CLIPSIBLINGS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_CLIPCHILDREN", WS_CLIPCHILDREN, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_MAXIMIZE", WS_MAXIMIZE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_BORDER", WS_BORDER, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_DLGFRAME", WS_DLGFRAME, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_VSCROLL", WS_VSCROLL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_HSCROLL", WS_HSCROLL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_SYSMENU", WS_SYSMENU, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_THICKFRAME", WS_THICKFRAME, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_GROUP", WS_GROUP, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_TABSTOP", WS_TABSTOP, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_MINIMIZEBOX", WS_MINIMIZEBOX, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_MAXIMIZEBOX", WS_MAXIMIZEBOX, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_CAPTION", WS_CAPTION, CONST_CS|CONST_PERSISTENT);\
 \
	/* 扩展窗口样式 */\
	REGISTER_LONG_CONSTANT("WS_EX_DLGMODALFRAME", WS_EX_DLGMODALFRAME, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_NOPARENTNOTIFY", WS_EX_NOPARENTNOTIFY, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_ACCEPTFILES", WS_EX_ACCEPTFILES, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_TRANSPARENT", WS_EX_TRANSPARENT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_MDICHILD", WS_EX_MDICHILD, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_TOOLWINDOW", WS_EX_TOOLWINDOW, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_WINDOWEDGE", WS_EX_WINDOWEDGE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_CONTEXTHELP", WS_EX_CONTEXTHELP, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_RIGHT", WS_EX_RIGHT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_LEFT", WS_EX_LEFT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_RTLREADING", WS_EX_RTLREADING, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_LTRREADING", WS_EX_LTRREADING, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_LEFTSCROLLBAR", WS_EX_LEFTSCROLLBAR, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_RIGHTSCROLLBAR", WS_EX_RIGHTSCROLLBAR, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_CONTROLPARENT", WS_EX_CONTROLPARENT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_STATICEDGE", WS_EX_STATICEDGE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_APPWINDOW", WS_EX_APPWINDOW, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_OVERLAPPEDWINDOW", WS_EX_OVERLAPPEDWINDOW, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_LAYERED", WS_EX_LAYERED, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_NOACTIVATE", WS_EX_NOACTIVATE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_CLIENTEDGE", WS_EX_CLIENTEDGE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("WS_EX_TOPMOST", WS_EX_TOPMOST, CONST_CS | CONST_PERSISTENT);\
 \
	/* 对话框样式 */\
	REGISTER_LONG_CONSTANT("DS_ABSALIGN", DS_ABSALIGN, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_SYSMODAL", DS_SYSMODAL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_LOCALEDIT", DS_LOCALEDIT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_SETFONT", DS_SETFONT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_MODALFRAME", DS_MODALFRAME, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_NOIDLEMSG", DS_NOIDLEMSG, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_SETFOREGROUND", DS_SETFOREGROUND, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_3DLOOK", DS_3DLOOK, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_FIXEDSYS", DS_FIXEDSYS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_NOFAILCREATE", DS_NOFAILCREATE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_CONTROL", DS_CONTROL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_CENTER", DS_CENTER, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_CENTERMOUSE", DS_CENTERMOUSE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_CONTEXTHELP", DS_CONTEXTHELP, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_SHELLFONT", DS_SHELLFONT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DS_USEPIXELS", 0x8000L, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DM_GETDEFID", DM_GETDEFID, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DM_SETDEFID", DM_SETDEFID, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DM_REPOSITION", DM_REPOSITION, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DC_HASDEFID", DC_HASDEFID, CONST_CS|CONST_PERSISTENT);\
 \
	/* 对话框按钮 */\
	REGISTER_LONG_CONSTANT("IDOK", IDOK, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("IDCANCEL", IDCANCEL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("IDABORT", IDCANCEL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("IDIGNORE", IDCANCEL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("IDNO", IDCANCEL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("IDYES", IDCANCEL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("IDRETRY", IDCANCEL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("MB_ABORTRETRYIGNORE", MB_ABORTRETRYIGNORE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("MB_OK", MB_OK, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("MB_OKCANCEL", MB_OKCANCEL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("MB_RETRYCANCEL", MB_RETRYCANCEL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("MB_YESNO", MB_YESNO, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("MB_YESNOCANCEL", MB_YESNOCANCEL, CONST_CS|CONST_PERSISTENT);\
 \
	/*	窗口状态 */\
	REGISTER_LONG_CONSTANT("SW_HIDE", SW_HIDE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_SHOWNORMAL", SW_SHOWNORMAL, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_NORMAL", SW_NORMAL, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_SHOWMINIMIZED", SW_SHOWMINIMIZED, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_SHOWMAXIMIZED", SW_SHOWMAXIMIZED, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_MAXIMIZE", SW_MAXIMIZE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_SHOWNOACTIVATE", SW_SHOWNOACTIVATE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_SHOW", SW_SHOW, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_MINIMIZE", SW_MINIMIZE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_SHOWMINNOACTIVE", SW_SHOWMINNOACTIVE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_SHOWNA", SW_SHOWNA, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_RESTORE", SW_RESTORE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_SHOWDEFAULT", SW_SHOWDEFAULT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_FORCEMINIMIZE", SW_FORCEMINIMIZE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SW_MAX", SW_MAX, CONST_CS | CONST_PERSISTENT);\
 \
	/* 窗口位置 */\
	REGISTER_LONG_CONSTANT("SWP_ASYNCWINDOWPOS", SWP_ASYNCWINDOWPOS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_DEFERERASE", SWP_DEFERERASE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_DRAWFRAME", SWP_DRAWFRAME, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_FRAMECHANGED", SWP_FRAMECHANGED, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_HIDEWINDOW", SWP_HIDEWINDOW, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_NOACTIVATE", SWP_NOACTIVATE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_NOCOPYBITS", SWP_NOCOPYBITS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_NOMOVE", SWP_NOMOVE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_NOOWNERZORDER", SWP_NOOWNERZORDER, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_NOREDRAW", SWP_NOREDRAW, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_NOREPOSITION", SWP_NOREPOSITION, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_NOSENDCHANGING", SWP_NOSENDCHANGING, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_NOSIZE", SWP_NOSIZE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_NOZORDER", SWP_NOZORDER, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("SWP_SHOWWINDOW", SWP_SHOWWINDOW, CONST_CS|CONST_PERSISTENT);\
 \
	REGISTER_LONG_CONSTANT("HWND_BOTTOM", (long)HWND_BOTTOM, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("HWND_NOTOPMOST", (long)HWND_NOTOPMOST, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("HWND_TOP", (long)HWND_TOP, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("HWND_TOPMOST", (long)HWND_TOPMOST, CONST_CS|CONST_PERSISTENT);\
 \
	/* HRESULT */\
	REGISTER_LONG_CONSTANT("S_OK", (long)S_OK, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("S_FALSE", (long)S_FALSE, CONST_CS|CONST_PERSISTENT);\
 \
	/*	DHTML 事件 */\
	REGISTER_LONG_CONSTANT("DHTML_ONACTIVATE", DISPID_HTMLELEMENTEVENTS_ONACTIVATE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONAFTERUPDATE", DISPID_HTMLELEMENTEVENTS_ONAFTERUPDATE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONBEFOREACTIVATE", DISPID_HTMLELEMENTEVENTS_ONBEFOREACTIVATE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONBEFORECOPY", DISPID_HTMLELEMENTEVENTS_ONBEFORECOPY, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONBEFORECUT", DISPID_HTMLELEMENTEVENTS_ONBEFORECUT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONBEFOREDEACTIVATE", DISPID_HTMLELEMENTEVENTS_ONBEFOREDEACTIVATE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONBEFOREEDITFOCUS", DISPID_HTMLELEMENTEVENTS_ONBEFOREEDITFOCUS, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONBEFOREPASTE", DISPID_HTMLELEMENTEVENTS_ONBEFOREPASTE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONBEFOREUPDATE", DISPID_HTMLELEMENTEVENTS_ONBEFOREUPDATE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONBLUR", DISPID_HTMLELEMENTEVENTS_ONBLUR, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONCELLCHANGE", DISPID_HTMLELEMENTEVENTS_ONCELLCHANGE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONCLICK", DISPID_HTMLELEMENTEVENTS_ONCLICK, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONCONTEXTMENU", DISPID_HTMLELEMENTEVENTS_ONCONTEXTMENU, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONCONTROLSELECT", DISPID_HTMLELEMENTEVENTS_ONCONTROLSELECT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONCOPY", DISPID_HTMLELEMENTEVENTS_ONCOPY, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONCUT", DISPID_HTMLELEMENTEVENTS_ONCUT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDATAAVAILABLE", DISPID_HTMLELEMENTEVENTS_ONDATAAVAILABLE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDATASETCHANGED", DISPID_HTMLELEMENTEVENTS_ONDATASETCHANGED, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDATASETCOMPLETE", DISPID_HTMLELEMENTEVENTS_ONDATASETCOMPLETE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDBLCLICK", DISPID_HTMLELEMENTEVENTS_ONDBLCLICK, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDEACTIVATE", DISPID_HTMLELEMENTEVENTS_ONDEACTIVATE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDRAG", DISPID_HTMLELEMENTEVENTS_ONDRAG, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDRAGEND", DISPID_HTMLELEMENTEVENTS_ONDRAGEND, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDRAGENTER", DISPID_HTMLELEMENTEVENTS_ONDRAGENTER, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDRAGLEAVE", DISPID_HTMLELEMENTEVENTS_ONDRAGLEAVE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDRAGOVER", DISPID_HTMLELEMENTEVENTS_ONDRAGOVER, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDRAGSTART", DISPID_HTMLELEMENTEVENTS_ONDRAGSTART, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONDROP", DISPID_HTMLELEMENTEVENTS_ONDROP, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONERRORUPDATE", DISPID_HTMLELEMENTEVENTS_ONERRORUPDATE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONFILTERCHANGE", DISPID_HTMLELEMENTEVENTS_ONFILTERCHANGE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONFOCUS", DISPID_HTMLELEMENTEVENTS_ONFOCUS, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONFOCUSIN", DISPID_HTMLELEMENTEVENTS_ONFOCUSIN, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONFOCUSOUT", DISPID_HTMLELEMENTEVENTS_ONFOCUSOUT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONHELP", DISPID_HTMLELEMENTEVENTS_ONHELP, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONKEYDOWN", DISPID_HTMLELEMENTEVENTS_ONKEYDOWN, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONKEYPRESS", DISPID_HTMLELEMENTEVENTS_ONKEYPRESS, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONKEYUP", DISPID_HTMLELEMENTEVENTS_ONKEYUP, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONLAYOUTCOMPLETE", DISPID_HTMLELEMENTEVENTS_ONLAYOUTCOMPLETE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONLOSECAPTURE", DISPID_HTMLELEMENTEVENTS_ONLOSECAPTURE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEDOWN", DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEENTER", DISPID_HTMLELEMENTEVENTS_ONMOUSEENTER, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSELEAVE", DISPID_HTMLELEMENTEVENTS_ONMOUSELEAVE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEMOVE", DISPID_HTMLELEMENTEVENTS_ONMOUSEMOVE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEOUT", DISPID_HTMLELEMENTEVENTS_ONMOUSEOUT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEOVER", DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEUP", DISPID_HTMLELEMENTEVENTS_ONMOUSEUP, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEWHEEL", DISPID_HTMLELEMENTEVENTS_ONMOUSEWHEEL, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOVE", DISPID_HTMLELEMENTEVENTS_ONMOVE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOVEEND", DISPID_HTMLELEMENTEVENTS_ONMOVEEND, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONMOVESTART", DISPID_HTMLELEMENTEVENTS_ONMOVESTART, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONPAGE", DISPID_HTMLELEMENTEVENTS_ONPAGE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONPASTE", DISPID_HTMLELEMENTEVENTS_ONPASTE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONPROPERTYCHANGE", DISPID_HTMLELEMENTEVENTS_ONPROPERTYCHANGE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONREADYSTATECHANGE", DISPID_HTMLELEMENTEVENTS_ONREADYSTATECHANGE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONRESIZE", DISPID_HTMLELEMENTEVENTS_ONRESIZE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONRESIZEEND", DISPID_HTMLELEMENTEVENTS_ONRESIZEEND, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONRESIZESTART", DISPID_HTMLELEMENTEVENTS_ONRESIZESTART, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONROWENTER", DISPID_HTMLELEMENTEVENTS_ONROWENTER, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONROWEXIT", DISPID_HTMLELEMENTEVENTS_ONROWEXIT, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONROWSDELETE", DISPID_HTMLELEMENTEVENTS_ONROWSDELETE, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONROWSINSERTED", DISPID_HTMLELEMENTEVENTS_ONROWSINSERTED, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONSCROLL", DISPID_HTMLELEMENTEVENTS_ONSCROLL, CONST_CS | CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("DHTML_ONSELECTSTART", DISPID_HTMLELEMENTEVENTS_ONSELECTSTART, CONST_CS | CONST_PERSISTENT);\
 	\
	/* DHTML Dialog 事件 */\
	REGISTER_LONG_CONSTANT("DHTML_ONINITDIALOG", DLG_EVENT_ONINITDIALOG, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("DHTML_ONBEFORENAVIGATE", DLG_EVENT_ONBEFORENAVIGATE, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("DHTML_ONNAVIGATECOMPLETE", DLG_EVENT_ONNAVIGATECOMPLETE, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("DHTML_ONDOCUMENTCOMPLETE", DLG_EVENT_ONDOCUMENTCOMPLETE, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("DHTML_ONCOMMAND", DLG_EVENT_ONCOMMAND, CONST_CS|CONST_PERSISTENT) ;\
 \
	/* 托盘图标事件 */\
	REGISTER_LONG_CONSTANT("TRAY_ONMOUSEMOVE", TRAY_EVENT_ONMOUSEMOVE, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TRAY_ONLBUTTONDOWN", TRAY_EVENT_ONLBUTTONDOWN, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TRAY_ONLBUTTONUP", TRAY_EVENT_ONLBUTTONUP, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TRAY_ONLBUTTONDBLCLK", TRAY_EVENT_ONLBUTTONDBLCLK, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TRAY_ONRBUTTONDOWN", TRAY_EVENT_ONRBUTTONDOWN, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TRAY_ONRBUTTONUP", TRAY_EVENT_ONRBUTTONUP, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TRAY_ONRBUTTONDBLCLK", TRAY_EVENT_ONRBUTTONDBLCLK, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TRAY_ONMBUTTONDOWN", TRAY_EVENT_ONMBUTTONDOWN, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TRAY_ONMBUTTONUP", TRAY_EVENT_ONMBUTTONUP, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TRAY_ONMBUTTONDBLCLK", TRAY_EVENT_ONMBUTTONDBLCLK, CONST_CS|CONST_PERSISTENT) ;\
 \
	/* 菜单 FLAG */\
	REGISTER_LONG_CONSTANT("MF_CHECKED", MF_CHECKED, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_UNCHECKED", MF_UNCHECKED, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_DISABLED", MF_DISABLED, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_ENABLED", MF_ENABLED, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_GRAYED", MF_GRAYED, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_MENUBARBREAK", MF_MENUBARBREAK, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_MENUBREAK", MF_MENUBREAK, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_OWNERDRAW", MF_OWNERDRAW, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_POPUP", MF_POPUP, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_SEPARATOR", MF_SEPARATOR, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_STRING", MF_STRING, CONST_CS|CONST_PERSISTENT) ;\
 \
	REGISTER_LONG_CONSTANT("MF_BYCOMMAND", MF_BYCOMMAND, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("MF_BYPOSITION", MF_BYPOSITION, CONST_CS|CONST_PERSISTENT) ;\
 \
	/* 菜单 弹出 */\
	REGISTER_LONG_CONSTANT("TPM_CENTERALIGN", TPM_CENTERALIGN, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_LEFTALIGN", TPM_LEFTALIGN, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_RIGHTALIGN", TPM_RIGHTALIGN, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_BOTTOMALIGN", TPM_BOTTOMALIGN, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_TOPALIGN", TPM_TOPALIGN, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_VCENTERALIGN", TPM_VCENTERALIGN, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_NONOTIFY", TPM_NONOTIFY, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_RETURNCMD", TPM_RETURNCMD, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_LEFTBUTTON", TPM_LEFTBUTTON, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_RIGHTBUTTON", TPM_RIGHTBUTTON, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_HORNEGANIMATION", TPM_HORNEGANIMATION, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_HORPOSANIMATION", TPM_HORPOSANIMATION, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_NOANIMATION", TPM_NOANIMATION, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_VERNEGANIMATION", TPM_VERNEGANIMATION, CONST_CS|CONST_PERSISTENT) ;\
	REGISTER_LONG_CONSTANT("TPM_VERPOSANIMATION", TPM_VERPOSANIMATION, CONST_CS|CONST_PERSISTENT) ;\
\
	/* 系统目录 */\
	REGISTER_LONG_CONSTANT("CSIDL_FLAG_CREATE", CSIDL_FLAG_CREATE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_ADMINTOOLS", CSIDL_ADMINTOOLS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_ALTSTARTUP", CSIDL_ALTSTARTUP, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_APPDATA", CSIDL_APPDATA, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_BITBUCKET", CSIDL_BITBUCKET, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_CDBURN_AREA", CSIDL_CDBURN_AREA, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_ADMINTOOLS", CSIDL_COMMON_ADMINTOOLS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_ALTSTARTUP", CSIDL_COMMON_ALTSTARTUP, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_APPDATA", CSIDL_COMMON_APPDATA, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_DESKTOPDIRECTORY", CSIDL_COMMON_DESKTOPDIRECTORY, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_DOCUMENTS", CSIDL_COMMON_DOCUMENTS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_FAVORITES", CSIDL_COMMON_FAVORITES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_MUSIC", CSIDL_COMMON_MUSIC, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_PICTURES", CSIDL_COMMON_PICTURES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_PROGRAMS", CSIDL_COMMON_PROGRAMS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_STARTMENU", CSIDL_COMMON_STARTMENU, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_STARTUP", CSIDL_COMMON_STARTUP, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_TEMPLATES", CSIDL_COMMON_TEMPLATES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COMMON_VIDEO", CSIDL_COMMON_VIDEO, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_CONTROLS", CSIDL_CONTROLS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_COOKIES", CSIDL_COOKIES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_DESKTOP", CSIDL_DESKTOP, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_DESKTOPDIRECTORY", CSIDL_DESKTOPDIRECTORY, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_DRIVES", CSIDL_DRIVES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_FAVORITES", CSIDL_FAVORITES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_FONTS", CSIDL_FONTS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_HISTORY", CSIDL_HISTORY, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_INTERNET", CSIDL_INTERNET, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_INTERNET_CACHE", CSIDL_INTERNET_CACHE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_LOCAL_APPDATA", CSIDL_LOCAL_APPDATA, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_MYDOCUMENTS", CSIDL_MYDOCUMENTS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_MYMUSIC", CSIDL_MYMUSIC, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_MYPICTURES", CSIDL_MYPICTURES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_MYVIDEO", CSIDL_MYVIDEO, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_NETHOOD", CSIDL_NETHOOD, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_NETWORK", CSIDL_NETWORK, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_PERSONAL", CSIDL_PERSONAL, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_PRINTERS", CSIDL_PRINTERS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_PRINTHOOD", CSIDL_PRINTHOOD, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_PROFILE", CSIDL_PROFILE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_PROFILES", 0x003e, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_PROGRAM_FILES", CSIDL_PROGRAM_FILES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_PROGRAM_FILES_COMMON", CSIDL_PROGRAM_FILES_COMMON, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_PROGRAMS", CSIDL_PROGRAMS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_RECENT", CSIDL_RECENT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_SENDTO", CSIDL_SENDTO, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_STARTMENU", CSIDL_STARTMENU, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_STARTUP", CSIDL_STARTUP, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_SYSTEM", CSIDL_SYSTEM, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_TEMPLATES", CSIDL_TEMPLATES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("CSIDL_WINDOWS", CSIDL_WINDOWS, CONST_CS|CONST_PERSISTENT);\
\
	/* CFileDialog(OPENFILENAME)常量 */\
	REGISTER_LONG_CONSTANT("OFN_ALLOWMULTISELECT", OFN_ALLOWMULTISELECT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_CREATEPROMPT", OFN_CREATEPROMPT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_DONTADDTORECENT", OFN_DONTADDTORECENT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_ENABLEHOOK", OFN_ENABLEHOOK, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_ENABLEINCLUDENOTIFY", OFN_ENABLEINCLUDENOTIFY, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_ENABLESIZING", OFN_ENABLESIZING, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_ENABLETEMPLATE", OFN_ENABLETEMPLATE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_ENABLETEMPLATEHANDLE", OFN_ENABLETEMPLATEHANDLE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_EXPLORER", OFN_EXPLORER, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_EXTENSIONDIFFERENT", OFN_EXTENSIONDIFFERENT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_FILEMUSTEXIST", OFN_FILEMUSTEXIST, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_FORCESHOWHIDDEN", OFN_FORCESHOWHIDDEN, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_HIDEREADONLY", OFN_HIDEREADONLY, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_LONGNAMES", OFN_LONGNAMES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_NOCHANGEDIR", OFN_NOCHANGEDIR, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_NODEREFERENCELINKS", OFN_NODEREFERENCELINKS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_NOLONGNAMES", OFN_NOLONGNAMES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_NONETWORKBUTTON", OFN_NONETWORKBUTTON, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_NOREADONLYRETURN", OFN_NOREADONLYRETURN, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_NOTESTFILECREATE", OFN_NOTESTFILECREATE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_NOVALIDATE", OFN_NOVALIDATE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_OVERWRITEPROMPT", OFN_OVERWRITEPROMPT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_PATHMUSTEXIST", OFN_PATHMUSTEXIST, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_READONLY", OFN_READONLY, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_SHAREAWARE", OFN_SHAREAWARE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("OFN_SHOWHELP", OFN_SHOWHELP, CONST_CS|CONST_PERSISTENT);\
\
	/* SHBrowseForFolder常量(BROWSEINFO) */\
	REGISTER_LONG_CONSTANT("BIF_BROWSEFORCOMPUTER", BIF_BROWSEFORCOMPUTER, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_BROWSEFORPRINTER", BIF_BROWSEFORPRINTER, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_BROWSEINCLUDEFILES", BIF_BROWSEINCLUDEFILES, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_BROWSEINCLUDEURLS", BIF_BROWSEINCLUDEURLS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_DONTGOBELOWDOMAIN", BIF_DONTGOBELOWDOMAIN, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_EDITBOX", BIF_EDITBOX, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_NEWDIALOGSTYLE", BIF_NEWDIALOGSTYLE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_NONEWFOLDERBUTTON", BIF_NONEWFOLDERBUTTON, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_NOTRANSLATETARGETS", BIF_NOTRANSLATETARGETS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_RETURNFSANCESTORS", BIF_RETURNFSANCESTORS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_RETURNONLYFSDIRS", BIF_RETURNONLYFSDIRS, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_SHAREABLE", BIF_SHAREABLE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_STATUSTEXT", BIF_STATUSTEXT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_UAHINT", BIF_UAHINT, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_USENEWUI", BIF_USENEWUI, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("BIF_VALIDATE", BIF_VALIDATE, CONST_CS|CONST_PERSISTENT);\
\
	/* 管道常量 */\
	REGISTER_LONG_CONSTANT("STD_INPUT_HANDLE", STD_INPUT_HANDLE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("STD_OUTPUT_HANDLE", STD_OUTPUT_HANDLE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("STD_ERROR_HANDLE", STD_ERROR_HANDLE, CONST_CS|CONST_PERSISTENT);\
\
	/* 管道常量 */\
	REGISTER_LONG_CONSTANT("HKEY_CLASSES_ROOT", 0x80000000, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("HKEY_CURRENT_USER", 0x80000001, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("HKEY_LOCAL_MACHINE", 0x80000002, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("HKEY_USERS", 0x80000003, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("HKEY_CURRENT_CONFIG", 0x80000005, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("HKEY_PERFORMANCE_DATA", 0x80000004, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("HKEY_PERFORMANCE_TEXT", 0x80000050, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("HKEY_PERFORMANCE_NLSTEXT", 0x80000060, CONST_CS|CONST_PERSISTENT);\
\
	REGISTER_LONG_CONSTANT("REG_NONE", REG_NONE, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_SZ", REG_SZ, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_EXPAND_SZ", REG_EXPAND_SZ, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_BINARY", REG_BINARY, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_DWORD", REG_DWORD, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_DWORD_LITTLE_ENDIAN", REG_DWORD_LITTLE_ENDIAN, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_DWORD_BIG_ENDIAN", REG_DWORD_BIG_ENDIAN, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_LINK", REG_LINK, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_MULTI_SZ", REG_MULTI_SZ, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_RESOURCE_LIST", REG_RESOURCE_LIST, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_FULL_RESOURCE_DESCRIPTOR", REG_FULL_RESOURCE_DESCRIPTOR, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_RESOURCE_REQUIREMENTS_LIST", REG_RESOURCE_REQUIREMENTS_LIST, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_QWORD", REG_QWORD, CONST_CS|CONST_PERSISTENT);\
	REGISTER_LONG_CONSTANT("REG_QWORD_LITTLE_ENDIAN", REG_QWORD_LITTLE_ENDIAN, CONST_CS|CONST_PERSISTENT);\




