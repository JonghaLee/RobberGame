#ifndef _CTEXTUREDLG_H_
#define _CTEXTUREDLG_H_

#include <windows.h>
#include <iostream>

#include "LoadFile.h"
#include "resource.h"

#define MAX_TEXPATH 30

class CTextureDlg
{
public:
	CTextureDlg(HWND _hMain);
	~CTextureDlg();
	
	void AddTexture(HWND _h);

	BOOL CALLBACK TextureDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
private:
	
	LPWSTR m_pTexPath[MAX_TEXPATH];

	HWND m_hMain;
	
};

#endif