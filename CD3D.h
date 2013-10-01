#ifndef _CD3D_H_
#define _CD3D_H_

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>

class CD3D
{
public:
	CD3D();
	~CD3D();
	LPDIRECT3DDEVICE9 InitD3D(HWND _hWnd);
	VOID Cleanup();
private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pd3dDevice;

	
};

#endif