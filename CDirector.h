#ifndef _CDIRECTOR_H_
#define _CDIRECTOR_H_

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>



#include "resource.h"

class CTileManager;
class CObjectManager;

class CDirector
{
public:
	CDirector(LPDIRECT3DDEVICE9 _device, HWND _hView, HWND _hControl, HINSTANCE _inst);
	~CDirector();

	void OnLButtonDown(HWND _hView, LPARAM _lparam);
	
	void RenderAll(LPDIRECT3DDEVICE9 _device);
	

	/* 프로시저 */
	LRESULT CALLBACK ViewWndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK ControlWndProc(HWND, UINT, WPARAM, LPARAM);

private:

	LPDIRECT3DDEVICE9 m_pD3Ddevice;

	HWND m_hView, m_hControl;
	HWND m_hTextureDlg, m_hObjectDlg;


	CTileManager *TileManager;
	CObjectManager *ObjectManager;
	
};

#endif