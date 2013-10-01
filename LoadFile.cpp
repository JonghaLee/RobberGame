#include "LoadFile.h"

void loadfile::GetPathName(HWND _hWnd, LPWSTR _pPath)
	{
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		//WCHAR == TCHAR,  WCHAR* == LPWSTR

		TCHAR lpstrFile[MAX_PATH_LEN];
		//TCHAR lpstrName[MAX_PATH_LEN];
		memset(lpstrFile, 0, MAX_PATH_LEN);
		//memset(lpstrName, 0, MAX_PATH_LEN);
		

		ofn.lStructSize = sizeof(OPENFILENAME);

		ofn.hwndOwner = _hWnd;
		ofn.lpstrFilter = L"BMP ����(*.bmp)\0*.bmp; \0��� ����(*.*)\0*.*\0";

		ofn.lpstrFile = lpstrFile;
		ofn.nMaxFile = 1024;
		//ofn.lpstrFileTitle = lpstrName;
		//ofn.nMaxFileTitle = MAX_PATH_LEN;

		ofn.lpstrInitialDir = L"Data";	//������ �ִ� �����
		ofn.Flags = OFN_NOCHANGEDIR;
		
		
		if(GetOpenFileName(&ofn)!=0){
			wcscpy_s(_pPath, MAX_PATH_LEN, ofn.lpstrFile);
		}
	}