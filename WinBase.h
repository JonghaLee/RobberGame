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
	static int MessageLoop(HINSTANCE hInstance = NULL);		// �޽���ť
	
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

	ATOM MyRegisterClass(HINSTANCE hInstance);	// ������ Ŭ���� ���
	BOOL InitInstance(HINSTANCE, int);
	
	//���� ���ν���
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


	//	������ ���ν���
	//	
	//	��¥ ���ν���(�������� �۾��� Director���� ��)
	static LRESULT CALLBACK ViewWndProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK ControlWndProc(HWND, UINT, WPARAM, LPARAM);
	
	static BOOL CALLBACK TextureDlgProc(HWND, UINT, WPARAM, LPARAM);
	static BOOL CALLBACK ObjDlgProc(HWND, UINT, WPARAM, LPARAM);
};

#endif