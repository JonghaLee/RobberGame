#ifndef _CFILELOAD_H_
#define _CFILELOAD_H_





	

	LPWSTR GetPathName(HWND _hWnd, LPWSTR _pPath)
	{
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		//WCHAR == TCHAR,  WCHAR* == LPWSTR
		/*TCHAR lpstrFile[MAX_PATH_LEN];
		TCHAR lpstrName[MAX_PATH_LEN];
		memset(lpstrFile, 0, MAX_PATH_LEN);
		memset(lpstrName, 0, MAX_PATH_LEN);	*/
		
		TCHAR lpstrFile[1024];
		memset(lpstrFile, 0, 1024);

		ofn.lStructSize = sizeof(OPENFILENAME);

		ofn.hwndOwner = _hWnd;
		ofn.lpstrFilter = L"BMP ����(*.bmp)\0*.bmp; \0��� ����(*.*)\0*.*\0";

		ofn.lpstrFile = lpstrFile;
		ofn.nMaxFile = 1024;
		//ofn.lpstrFileTitle = lpstrName;
		//ofn.nMaxFileTitle = MAX_PATH_LEN;

		ofn.lpstrInitialDir = L"Data";	//������ �ִ� �����
		ofn.Flags = OFN_NOCHANGEDIR;
		
		
		if(GetOpenFileName(&ofn)==0){
			return NULL;
		}else{
			_pPath = ofn.lpstrFile;
			return _pPath;
		}
	}

	





#endif
