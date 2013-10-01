#ifndef _LOADFILE_H_
#define _LOADFILE_H_

#include <windows.h>
#include <iostream>
#include <tchar.h>

namespace loadfile
{
	#define MAX_PATH_LEN 1024

	void GetPathName(HWND _hWnd, LPWSTR _pPath);
}

#endif