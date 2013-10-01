#include "CPicking.h"
CPicking::CPicking()
{
}
CPicking::~CPicking()
{
}

CPicking& CPicking::PickInst()
{
	static CPicking Pick;
	return Pick;
}

RAY CPicking::CalcPickingRay(LPDIRECT3DDEVICE9 _device, HWND _hWnd, int x, int y)
{
	RAY ray;	
	
	RECT rt;
	GetClientRect(_hWnd, &rt);

	
	float px = 0.0f;
	float py = 0.0f;
	
	//get viewport 
	D3DVIEWPORT9 vp;
	_device->GetViewport(&vp);
	
	//get projection
	D3DXMATRIX proj;
	_device->GetTransform(D3DTS_PROJECTION, &proj);

	//px = ( ((2.0f*x) / vp.Width) - 1.0f ) / proj(0,0);
	//py = ( ((-2.0f*y) / vp.Height) + 1.0f ) / proj(1,1);
	
	px = ( ((2.0f*x) / vp.Width) - 1.0f );
	py = ( ((-2.0f*y) / vp.Height) + 1.0f );
	px /= proj._11;
	py /= proj._22;

	ray.origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray.direction = D3DXVECTOR3(px, py, 1.0f);
	
	D3DXMATRIX view;
	_device->GetTransform(D3DTS_VIEW, &view);
	
	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);	

	// 광선의 원점을 변환한다 w = 1
	D3DXVec3TransformCoord(&ray.origin, &ray.origin, &viewInverse);
	// 광선의 방향을 변환한다 w = 0
	D3DXVec3TransformNormal(&ray.direction, &ray.direction, &viewInverse);
	
	// 방향을 정규화
	D3DXVec3Normalize(&ray.direction, &ray.direction);

	return ray;
}

bool CPicking::CollisionCheckTriangle(D3DXVECTOR3 &PickPos, RAY &_ray, D3DXVECTOR3 &_v0, D3DXVECTOR3 &_v1, D3DXVECTOR3 &_v2)
{
	float t,u,v;
	t = u = v = 0.0f;

	D3DXVECTOR3 v_edge1 = _v1 - _v0;
	D3DXVECTOR3 v_edge2 = _v2 - _v0;

	D3DXVECTOR3 tvec, pvec, qvec;
	float det, inv_det;

	D3DXVec3Cross(&pvec, &_ray.direction, &v_edge2);
	
	det = D3DXVec3Dot(&v_edge1, &pvec);

	if(det > -0.0001f && det < 0.0001f) return false;

	inv_det = 1.0f / det;

	tvec = _ray.origin - _v0;

	u  = D3DXVec3Dot(&tvec, &pvec) * inv_det;
	if(u < 0.0001f || u > 1.0001f) return false;
	
	D3DXVec3Cross(&qvec, &tvec, &v_edge1);
	
	v = D3DXVec3Dot(&_ray.direction, &qvec) * inv_det;
	if(v < 0.0001f || (u+v) > 1.0001f ) return false;

	_ray.fT = D3DXVec3Dot(&v_edge2, &qvec) * inv_det;

	if(_ray.fT <= 0) return false;

	PickPos = _v0 + (u * (_v1 - _v0)) + (v * (_v2 - _v0));

	return true;
}