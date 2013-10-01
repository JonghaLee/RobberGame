#include "CTile.h"

CTile::CTile(LPDIRECT3DDEVICE9 _device, float _x, float _z)
{
	D3DXMatrixIdentity(&m_matWorld);
	
	m_pTex = NULL;
	m_pIB = NULL;
	m_pVB = NULL;
	
	
	_b = false;


	
	ZeroMemory(&m_matrl, sizeof(D3DMATERIAL9));

	CreateVB(_device, _x, _z);
	CreateIB(_device);
}
CTile::~CTile()
{
	if(m_pTex != NULL) m_pTex->Release();
	
	if(m_pIB != NULL) m_pIB->Release();
	if(m_pVB != NULL) m_pVB->Release();
}

HRESULT CTile::CreateVB(LPDIRECT3DDEVICE9 _device, float _x, float _z)
{
	if( FAILED( _device->CreateVertexBuffer(
												4 * sizeof(TILEVERTEX),
												0,
												D3DFVF_CUSTOMVERTEX,
												D3DPOOL_DEFAULT,
												&m_pVB,
												0
											)
			 )
	 )
	{
		MessageBox(NULL, L"CTile::CreateVertexBuffer Failed!!", NULL, MB_OK);
		return E_FAIL;
	}
	
	

	if( FAILED( m_pVB->Lock(0, sizeof(TILEVERTEX)*4, (void**)&m_pTileVertex,0) ) ){
		MessageBox(NULL, L"CTile::VB Lock Failed!!", NULL, MB_OK);
		return E_FAIL;
	}

	m_pTileVertex[0].vPos.x = _x;
	m_pTileVertex[0].vPos.y = 0;
	m_pTileVertex[0].vPos.z = _z;	
	D3DXVec3Normalize(&m_pTileVertex[0].vNormal, &m_pTileVertex[0].vPos);	

	m_pTileVertex[1].vPos.x = _x + SIZE_TILE_W;
	m_pTileVertex[1].vPos.y = 0;
	m_pTileVertex[1].vPos.z = _z;
	D3DXVec3Normalize(&m_pTileVertex[1].vNormal, &m_pTileVertex[1].vPos);

	m_pTileVertex[2].vPos.x = _x + SIZE_TILE_W;
	m_pTileVertex[2].vPos.y = 0;
	m_pTileVertex[2].vPos.z = _z - SIZE_TILE_H;
	D3DXVec3Normalize(&m_pTileVertex[2].vNormal, &m_pTileVertex[2].vPos);

	m_pTileVertex[3].vPos.x = _x;
	m_pTileVertex[3].vPos.y = 0;
	m_pTileVertex[3].vPos.z = _z - SIZE_TILE_H;
	D3DXVec3Normalize(&m_pTileVertex[3].vNormal, &m_pTileVertex[3].vPos);

	m_pVB->Unlock();

	return S_OK;
}
HRESULT CTile::CreateIB(LPDIRECT3DDEVICE9 _device)
{
	if( FAILED(_device->CreateIndexBuffer(
											sizeof(WORD) * 6,
											0,
											D3DFMT_INDEX16,
											D3DPOOL_DEFAULT,
											&m_pIB,
											0
										 )
			  )
	  )
	{
		MessageBox(NULL, L"CTile::CreateIB Failed!!", NULL, MB_OK);
		return E_FAIL;
	}

	WORD* indices = 0;

	if( FAILED( m_pIB->Lock(0, sizeof(WORD)*6, (void**)&indices, 0 ) ) ){
	
		MessageBox(NULL, L"CTile::IB Lock Failed!!", NULL, MB_OK);
		return E_FAIL;
	}
	
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;
	
	
	m_pIB->Unlock();

	return S_OK;
}

void CTile::Render(LPDIRECT3DDEVICE9 _device)
{
	_device->SetStreamSource( 0, m_pVB, 0, sizeof(TILEVERTEX) );
	_device->SetFVF( D3DFVF_CUSTOMVERTEX );
	_device->SetIndices( m_pIB );

	if(_b == true){
		_device->SetMaterial(&m_matrl);
	}else{
		ZeroMemory(&m_matrl, sizeof(D3DMATERIAL9));
		_device->SetMaterial(&m_matrl);
	}

	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);
	//_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);
}

void CTile::UpdateWorldMatrix(LPDIRECT3DDEVICE9 _device)
{
	_device->SetTransform(D3DTS_WORLD, &m_matWorld);


	
}

TILEVERTEX* CTile::GetTileVertex()
{
	return m_pTileVertex;
}

void CTile::SetMaterial()
{
	_b = true;

	m_matrl.Emissive.r = 1.0f;
	m_matrl.Emissive.g = 0.0f;
	m_matrl.Emissive.b = 0.0f;
	m_matrl.Emissive.a = 1.0f;
}