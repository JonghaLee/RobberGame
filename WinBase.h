#ifndef _WINCLASS_H_
#define _WINCLASS_H_

#define ID_TOOLBAR 100

#pragma comment(lib,"Comctl32")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <iostream>
#include <commctrl.h>

#include "resource.h"

class CD3D;
class CDirector;
class CTextureDlg;
class CObjectDlg;

class WinClass
{
public:
	static int MessageLoop(HINSTANCE hInstance = NULL);		// 메시지큐
	
	void CreateWnd(HWND _hMain);
	void CreateToolBar(HWND _hMain);
	

	void CreateDlg();

private:
	static WinClass& m_WC(void);	// Instance
	LPCTSTR m_lpszTitle;
	
	CD3D* MyD3D;
	CDirector* Director;
	CTextureDlg* TextureDlg;
	CObjectDlg* ObjectDlg;

	HINSTANCE m_Inst;

	HWND m_hViewWnd,
		 m_hControlWnd;
	
	HWND m_hTextureDlg;

	HWND m_hToolBar;
private:
	WinClass();
	~WinClass();

	LPDIRECT3DDEVICE9	g_pd3dDevice;

	ATOM MyRegisterClass(HINSTANCE hInstance);	// 윈도우 클래스 등록
	BOOL InitInstance(HINSTANCE, int);
	
	//메인 프로시저
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


	//	윈도우 프로시저
	//	
	//	가짜 프로시저(실질적인 작업은 Director에서 함)
	static LRESULT CALLBACK ViewWndProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK ControlWndProc(HWND, UINT, WPARAM, LPARAM);
	
	static BOOL CALLBACK TextureDlgProc(HWND, UINT, WPARAM, LPARAM);
	static BOOL CALLBACK ObjDlgProc(HWND, UINT, WPARAM, LPARAM);
};

#endif