#ifndef _CTILEMANAGER_H_
#define _CTILEMANAGER_H_

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <list>


#include "StructCollect.h"

class CTile;


using namespace std;

class CTileManager
{
public:
	CTileManager(LPDIRECT3DDEVICE9 _device);
	~CTileManager();
	
	void CreateBaseTile(LPDIRECT3DDEVICE9 _device);
	
	void OnLButtonDown(LPDIRECT3DDEVICE9 _device, HWND _hView, LPARAM _lParam);

	

	void RenderAllTile(LPDIRECT3DDEVICE9 _device);
private:
	list<CTile*> m_TileList;
	list<CTile*>::iterator m_TileListIter;
	
	
	D3DXVECTOR3 m_vPickPos;
};

#endif