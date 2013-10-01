#ifndef _CMODEL_H_
#define _CMODEL_H_

#include "MD2.h"
//#include <d3d9.h>
//#include <d3dx9.h>


#define MODEL_TEX_MAX (5)

class CMD2Model;

typedef class CModel
{
public:
	CModel();
	~CModel();

	HRESULT CreateMeshVertex(int _iSel=0);
	HRESULT CreateMeshIndex(int _iSel=0);
	HRESULT VertexGeometry(int _iSel=0);
	HRESULT IndexGeometry(int _iSel=0);
	HRESULT SetTexture(LPCTSTR _fileName, int _iSel=0);
	HRESULT CartoonTexture(BYTE _r, BYTE _g, BYTE _b, int _iLevel, int _iSel);

	LPDIRECT3DTEXTURE9* GetTexPointer(int _iSel);
	D3DMATERIAL9* GetMaterial();
	LPD3DXMATRIXA16 GetWorldTransform();

	VOID CreateModel(LPDIRECT3DDEVICE9 _device, LPBOOL _bUpdate, PFLOAT _pfTime, int _iMcnt=1);
	VOID CreateMD2(LPCTSTR _fileName);
	VOID Move(float _x, float _y, float _z);
	VOID Rotate(float _x, float _y, float _z);
	VOID Scale(float _x, float _y, float _z);
	VOID ModelTransformMatrix();
	VOID Animation(CMD2Model::MODELSTATE _type, bool _bPlayState, int _iSel=0);
	VOID Render();
	VOID VertexUnitRender(CMD2Model::MODELSTATE _type, LPD3DXVECTOR3 _pLight);

	VOID ModelReset();
	VOID ModelPositionReset();
	VOID ModelRotationReset();
	VOID ModelScaleReset();

public:
	// MD2 °´Ã¼
	LPMD2MODEL m_pMD2;
protected:
	LPDIRECT3DDEVICE9 m_pD3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVerBuff;
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;
	LPDIRECT3DTEXTURE9 m_pTexture[MODEL_TEX_MAX];

	LPBOOL m_pbUpdate;

	D3DXVECTOR3 m_vScale;
	D3DXVECTOR3 m_vAngle;
	D3DXVECTOR3 m_vMove;

	D3DXMATRIX m_mScale;
	D3DXMATRIX m_mRotate;
	D3DXMATRIX m_mRotateX;
	D3DXMATRIX m_mRotateY;
	D3DXMATRIX m_mRotateZ;
	D3DXMATRIX m_mTrans;
	D3DXMATRIXA16 m_mTransform;
	D3DMATERIAL9 m_mMaterial;

	UINT m_iVertexSize;
	UINT m_iIndexSize;
	UINT m_iVertexNum;
	UINT m_iIndexNum;
	UINT m_iStructSize;
	UINT m_iMeshCnt;

	DWORD m_iFvf;
	PFLOAT m_pTime;

private:
	//bool m_bNowDrawing;


} *LPMODEL;

#endif // _CMODEL_H_