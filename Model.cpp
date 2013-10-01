#include "Model.h"
#include <iostream>
using namespace std;

CModel::CModel()
{
	m_pD3dDevice = NULL;
	m_pVerBuff = NULL;
	m_pIdxBuff = NULL;

	memset(m_pTexture, NULL, sizeof(m_pTexture) );

	m_pMD2 = NULL;
	m_pbUpdate = NULL;
	
	m_vScale.x = 1.0f;
	m_vScale.y = 1.0f;
	m_vScale.z = 1.0f;

	m_vAngle.x = NULL;
	m_vAngle.y = NULL;
	m_vAngle.z = NULL;

	m_vMove.x = NULL;
	m_vMove.y = NULL;
	m_vMove.z = NULL;

	D3DXMatrixIdentity(&m_mScale);
	D3DXMatrixIdentity(&m_mRotate);
	D3DXMatrixIdentity(&m_mRotateX);
	D3DXMatrixIdentity(&m_mRotateY);
	D3DXMatrixIdentity(&m_mRotateZ);
	D3DXMatrixIdentity(&m_mTrans);
	D3DXMatrixIdentity(&m_mTransform);

	m_mMaterial.Ambient = D3DXCOLOR( D3DCOLOR_XRGB(255, 255, 255) );
	m_mMaterial.Diffuse = D3DXCOLOR( D3DCOLOR_XRGB(255, 255, 255) );
	m_mMaterial.Specular = D3DXCOLOR( D3DCOLOR_XRGB(255, 255, 255) );
	m_mMaterial.Emissive = D3DXCOLOR( D3DCOLOR_XRGB(0, 0, 0) );
	m_mMaterial.Power = 5.0f;


	m_iVertexSize = NULL;
	m_iIndexSize = NULL;
	m_iVertexNum = NULL;
	m_iIndexNum = NULL;
	m_iStructSize = NULL;

	m_iFvf = NULL;
	m_pTime = NULL;

	m_iMeshCnt = 0;

	//m_bNowDrawing = false;
	cout<<"CModel()"<<endl;

}

CModel::~CModel()
{
	if(m_pMD2)		delete[] m_pMD2;
	cout<<"~CModel()"<<endl;
}

VOID CModel::CreateModel(LPDIRECT3DDEVICE9 _device, LPBOOL _bUpdate, PFLOAT _pfTime, int _iMcnt)
{
	m_pD3dDevice = _device;
	m_iMeshCnt = _iMcnt;
	m_pbUpdate = _bUpdate;
	m_pTime = _pfTime;

	m_pMD2 = new CMD2Model[m_iMeshCnt];
	m_iFvf = MD2_FVF;
	m_iStructSize = sizeof(VERTEXLIST);

	cout<<"CreateModel"<<endl;
}

VOID CModel::CreateMD2(LPCTSTR _fileName)
{
	m_pMD2[0].LoadMD2(_fileName);

	cout<<"CreateMD2"<<endl;
}

VOID CModel::Move(float _x, float _y, float _z)
{
	m_vMove.x += _x;
	m_vMove.y += _y;
	m_vMove.z += _z;

	D3DXMatrixTranslation(&m_mTrans, m_vMove.x, m_vMove.y, m_vMove.z);

	ModelTransformMatrix();
	cout<<"Move"<<endl;
}

VOID CModel::Rotate(float _x, float _y, float _z)
{
	if(_x)
	{
		m_vAngle.x += _x;
		(m_vAngle.x > 360) ? m_vAngle.x = 0 : NULL;
		D3DXMatrixRotationX(&m_mRotateX, m_vAngle.x);
	}
	if(_y)
	{
		m_vAngle.y += _y;
		(m_vAngle.y > 360) ? m_vAngle.y = 0 : NULL;
		D3DXMatrixRotationY(&m_mRotateY, m_vAngle.y);
	}
	if(_z)
	{
		m_vAngle.z += _z;
		(m_vAngle.z > 360) ? m_vAngle.z = 0 : NULL;
		D3DXMatrixRotationZ(&m_mRotateZ, m_vAngle.z);
	}

	m_mRotate = m_mRotateX * m_mRotateY * m_mRotateZ;

	ModelTransformMatrix();
	cout<<"Rotate"<<endl;
}

VOID CModel::Scale(float _x, float _y, float _z)
{
	m_vScale.x += _x;
	m_vScale.y += _y;
	m_vScale.z += _z;

	D3DXMatrixScaling(&m_mScale, m_vScale.x, m_vScale.y, m_vScale.z);

	ModelTransformMatrix();
	cout<<"Scale"<<endl;
}

VOID CModel::ModelTransformMatrix()
{//이게 갱신이되야한다
	D3DXMatrixIdentity(&m_mTransform);
	m_mTransform = m_mScale * m_mRotate * m_mTrans;
	


	*m_pbUpdate = TRUE;
	cout<<"ModelTransformMatrix"<<endl;
}

VOID CModel::Animation(CMD2Model::MODELSTATE _type, bool _bPlayState, int _iSel)
{
	VOID *pVertices = NULL;
	VOID *pointer = NULL;

	pointer = m_pMD2[_iSel].Animate(_type, (_bPlayState ?  *m_pTime : 0.0f) );

	if( FAILED(m_pVerBuff->Lock(0, m_iVertexSize, (void**)&pVertices, 0)) )
		return;

	memcpy(pVertices, pointer, m_iVertexSize);

	m_pVerBuff->Unlock();

	//cout<<"Animation :: OK "<<endl;
	

}

VOID CModel::Render()
{
	// 버텍스 등록
	m_pD3dDevice->SetStreamSource(0, m_pVerBuff, 0, m_iStructSize);
	m_pD3dDevice->SetFVF(m_iFvf);

	//인덱스 등록
	m_pD3dDevice->SetIndices(m_pIdxBuff);
	m_pD3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexNum, 0, m_iIndexNum);

	//cout<<"Render :: OK"<<endl;

}

////////////////////////////////////////////

HRESULT CModel::CreateMeshVertex(int _iSel)
{
	m_iVertexSize = m_pMD2[_iSel].GetNumVertices() * sizeof(VERTEXLIST);
	m_iVertexNum = m_pMD2[_iSel].GetNumVertices();

	if( FAILED(m_pD3dDevice->CreateVertexBuffer(m_iVertexSize,
												0,
												m_iFvf,
												D3DPOOL_DEFAULT,
												&m_pVerBuff,
												NULL)) )
		return E_FAIL;

	cout<<"CreateMeshVertex"<<endl;
	return S_OK;
}

HRESULT CModel::CreateMeshIndex(int _iSel)
{
	m_iIndexSize = m_pMD2[_iSel].GetNumTriangles() * sizeof(INDEXLIST);
	m_iIndexNum = m_pMD2[_iSel].GetNumTriangles();

	if( FAILED(m_pD3dDevice->CreateIndexBuffer(m_iIndexSize,
											   0,
											   D3DFMT_INDEX16,
											   D3DPOOL_DEFAULT,
											   &m_pIdxBuff,
											   NULL)) )
	   return E_FAIL;

	cout<<"CreateMeshIndex"<<endl;
	return S_OK;
}

HRESULT CModel::VertexGeometry(int _iSel)
{
	VOID* pVertices = NULL;
	VOID* pointer = NULL;

	//이부분 좀 이상하지만 나중에 알아보자.
	//( ->ver 이부분. LPFRAME 리턴형이지만 굳이 포인터접근자를 붙여야 하나? 싶음
	pointer = m_pMD2[_iSel].GetListFrame(0)->ver;

	if( FAILED(m_pVerBuff->Lock(0,
								m_iVertexSize,
								(VOID**)&pVertices,
								0)) )
		return E_FAIL;

	memcpy(pVertices, pointer, m_iVertexSize);

	m_pVerBuff->Unlock();

	cout<<"VertexGeometry"<<endl;
	return S_OK;
}

HRESULT CModel::IndexGeometry(int _iSel)
{
	VOID* pIndices = NULL;
	LPWORD pointer = NULL;

	pointer = (LPWORD)m_pMD2[_iSel].GetListTriangle();

	if( FAILED(m_pIdxBuff->Lock(0,
								m_iIndexSize,
								(void**)&pIndices,
								0)) )
		return E_FAIL;

	memcpy(pIndices, pointer, m_iIndexSize);

	m_pIdxBuff->Unlock();	

	cout<<"IndexGeometry"<<endl;
	return S_OK;
}

HRESULT CModel::SetTexture(LPCTSTR _fileName, int _iSel)
{
	if( FAILED(D3DXCreateTextureFromFile(m_pD3dDevice, _fileName, &m_pTexture[_iSel])) )
		return E_FAIL;

	//m_pD3dDevice->SetTexture(0, m_pTexture[_iSel]);
	m_pD3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	cout<<"SetTexture"<<endl;
	return S_OK;
}

HRESULT CModel::CartoonTexture(BYTE _r, BYTE _g, BYTE _b, int _iLevel, int _iSel)
{
	HRESULT hr = 0;
	D3DXCOLOR color;

	BYTE rPlus = _r ? _r/_iLevel : NULL;
	BYTE gPlus = _g ? _g/_iLevel : NULL;
	BYTE bPlus = _b ? _b/_iLevel : NULL;
	BYTE color_r = 0, color_g = 0, color_b = 0;
	LPBYTE TexToon = (LPBYTE)malloc(sizeof(BYTE) * 3 * _iLevel);

	for(int i=0; i<_iLevel; i++)
	{
		TexToon[i*3+0] = color_r;	//빨강
		TexToon[i*3+1] = color_g;	//녹
		TexToon[i*3+2] = color_b;	//파

		color_r += rPlus;
		color_g += gPlus;
		color_b += bPlus;
	}

	hr = D3DXCreateTexture(m_pD3dDevice,
						   _iLevel-1, 1,
						   0, 0,
						   D3DFMT_X8R8G8B8,
						   D3DPOOL_MANAGED,
						   &m_pTexture[_iSel]);
	
	if(FAILED(hr))			return hr;

	m_pD3dDevice->SetSamplerState(_iSel, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pD3dDevice->SetSamplerState(_iSel, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pD3dDevice->SetSamplerState(_iSel, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pD3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DSURFACE_DESC TextureDesc;
	m_pTexture[_iSel]->GetLevelDesc(0, &TextureDesc);

	if( TextureDesc.Format != D3DFMT_X8R8G8B8 )
		return TRUE;

	//이건 뭔지 잘몰겠다..
	D3DLOCKED_RECT LockedRect;
	m_pTexture[_iSel]->LockRect(0, &LockedRect, 0, 0);

	DWORD* offset = (DWORD*)LockedRect.pBits;
	for(int j=0; j<_iLevel; j++)
	{
		color = D3DXCOLOR(D3DCOLOR_XRGB(TexToon[j*3], TexToon[j*3+1], TexToon[j*3+2]));
		offset[j] = (D3DCOLOR)color;
	}

	m_pTexture[_iSel]->UnlockRect(0);

	free(TexToon);

	cout<<"CartoonTexture"<<endl;
	
	return hr;
}


////////////////////////////////////////////

LPDIRECT3DTEXTURE9* CModel::GetTexPointer(int _iSel)
{
	cout<<"GetTexPointer"<<endl;
	return &m_pTexture[_iSel];
}

D3DMATERIAL9* CModel::GetMaterial()
{
	cout<<"GetMaterial"<<endl;
	return &m_mMaterial;
}

LPD3DXMATRIXA16 CModel::GetWorldTransform()
{
	cout<<"GetWorldTransform"<<endl;
	return &m_mTransform;
}

VOID CModel::ModelReset()
{
	ModelPositionReset();
	ModelRotationReset();
	ModelScaleReset();
}

VOID CModel::ModelPositionReset()
{
	m_vMove.x = 0.0f;
	m_vMove.y = 0.0f;
	m_vMove.z = 0.0f;

	D3DXMatrixTranslation(&m_mTrans, m_vMove.x, m_vMove.y, m_vMove.z);

	ModelTransformMatrix();
	cout<<"Move"<<endl;
}

VOID CModel::ModelRotationReset()
{
	
	m_vAngle.x = 0.0f;
	D3DXMatrixRotationX(&m_mRotateX, m_vAngle.x);

	m_vAngle.y = 0.0f;
	D3DXMatrixRotationY(&m_mRotateY, m_vAngle.y);

	m_vAngle.z = 0.0f;
	D3DXMatrixRotationZ(&m_mRotateZ, m_vAngle.z);
	

	m_mRotate = m_mRotateX * m_mRotateY * m_mRotateZ;

	ModelTransformMatrix();
	cout<<"Rotate"<<endl;

}

VOID CModel::ModelScaleReset()
{
	m_vScale.x = 1.0f;
	m_vScale.y = 1.0f;
	m_vScale.z = 1.0f;

	D3DXMatrixScaling(&m_mScale, m_vScale.x, m_vScale.y, m_vScale.z);

	ModelTransformMatrix();
	cout<<"Scale"<<endl;
}