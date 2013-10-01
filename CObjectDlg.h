#ifndef _COBJECTDLG_H_
#define _COBJECTDLG_H_

#include <windows.h>
#include <iostream>

#include "resource.h"

class CObjectDlg
{
public:
	CObjectDlg(HWND _hMain);
	~CObjectDlg();
private:

	HWND m_hMain;
};

#endif