#ifndef _CTILE_H_
#define _CTILE_H_

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>

#include "TileInfo.h"

class CTile
{
public:
	CTile(LPDIRECT3DDEVICE9 _device, float _x, float _z);
	~CTile();
	
	//테스트용
	void SetMaterial();


	HRESULT CreateVB(LPDIRECT3DDEVICE9 _device, float _x, float _z);
	HRESULT CreateIB(LPDIRECT3DDEVICE9 _device);
	
	void UpdateWorldMatrix(LPDIRECT3DDEVICE9 _device);
	void Render(LPDIRECT3DDEVICE9 _device);
	TILEVERTEX* GetTileVertex();

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	
	TILEVERTEX *m_pTileVertex;

	//텍스쳐 정보
	LPDIRECT3DTEXTURE9 m_pTex;
	
	D3DMATERIAL9 m_matrl;

	D3DXMATRIXA16 m_matWorld;
	
	//테스트용
	bool _b;
};

#endif