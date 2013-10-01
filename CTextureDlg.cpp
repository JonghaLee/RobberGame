#include "CTextureDlg.h"


CTextureDlg::CTextureDlg(HWND _hMain)
{
	m_hMain = _hMain;
	

	for(int i=0; i<MAX_TEXPATH; ++i){
		m_pTexPath[i] = new WCHAR[1024];
	}
	

}
CTextureDlg::~CTextureDlg()
{
	for(int i=0; i<MAX_TEXPATH; ++i){
		delete[] m_pTexPath[i];
	}
}

//
//	*실질적인 TextureDialog의 작업을 수행함*
//
//  이곳에서 값을 변경후, 메인 프로시저에게 이벤트를 알림
//	메인 프로시저 에서 값을 Get해서 사용함....
//
BOOL CTextureDlg::TextureDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage){
		case WM_INITDIALOG:

		return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case ID_ADDTEX:
					AddTexture(hDlg);
				break;
				case ID_DELTEX:
					
				break;

			}
			SendMessage( m_hMain , WM_COMMAND, wParam, lParam);
		return TRUE;

		case WM_HSCROLL:
			
		return TRUE;

		case WM_CLOSE:
			EndDialog(hDlg,0);
		return TRUE;
	}

	return FALSE;
}

void CTextureDlg::AddTexture(HWND _h)
{
	LPWSTR str = new WCHAR[1024];

	loadfile::GetPathName(_h, str);
	
	if(str != NULL) delete[] str;
}