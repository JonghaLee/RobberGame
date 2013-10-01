#ifndef _CPICKING_H_
#define _CPICKING_H_

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "StructCollect.h"

class CPicking
{
public:
	CPicking();
	~CPicking();

	static CPicking& PickInst();
	
	bool CollisionCheckTriangle(D3DXVECTOR3 &PickPos, RAY &_ray, D3DXVECTOR3 &_v0, D3DXVECTOR3 &_v1, D3DXVECTOR3 &_v2);
	RAY CalcPickingRay(LPDIRECT3DDEVICE9 _device, HWND _hWnd, int x, int y);
private:
	
};

#endif