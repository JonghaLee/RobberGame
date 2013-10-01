#include "CTileManager.h"

#include "CTile.h"
#include "CPicking.h"

CTileManager::CTileManager(LPDIRECT3DDEVICE9 _device)
{
	m_vPickPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CreateBaseTile(_device);

	m_TileListIter = m_TileList.begin();
}
CTileManager::~CTileManager()
{
	for(m_TileListIter = m_TileList.begin(); m_TileListIter != m_TileList.end(); ++m_TileListIter){
		delete (*m_TileListIter);
		//m_TileList.clear();
	}

	
}

void CTileManager::RenderAllTile(LPDIRECT3DDEVICE9 _device)
{
	//타일이 생성 되었는지 확인
	if(m_TileList.empty()) return;
	
	for(m_TileListIter = m_TileList.begin(); m_TileListIter != m_TileList.end(); ++m_TileListIter){
		(*m_TileListIter)->UpdateWorldMatrix(_device);
		(*m_TileListIter)->Render(_device);
	}
}

void CTileManager::CreateBaseTile(LPDIRECT3DDEVICE9 _device)
{
	int halfTotalCount_width = int(TOTALCOUNT_TILE_W*0.5);
	int halfTotalCount_height = int(TOTALCOUNT_TILE_H*0.5);

	int startx = -(halfTotalCount_width) * SIZE_TILE_W;
	int startz = (halfTotalCount_height) * SIZE_TILE_H;
	
	int endx = halfTotalCount_width * SIZE_TILE_W;
	int endz = -halfTotalCount_height * SIZE_TILE_H;

	for(int z = startz; z > endz; z -= SIZE_TILE_H){
		for(int x = startx; x < endx; x += SIZE_TILE_W){
			m_TileList.push_back( new CTile(_device, (float)x, (float)z) );
		}
	}
}

void CTileManager::OnLButtonDown(LPDIRECT3DDEVICE9 _device, HWND _hView, LPARAM _lParam)
{
	LONG mouseX = LOWORD(_lParam);
	LONG mouseY = HIWORD(_lParam);

	RAY ray;
	ray = CPicking::PickInst().CalcPickingRay(_device, _hView, mouseX, mouseY);
	//ray = Picking->CalcPickingRay(_device, _hView, mouseX, mouseY);
	
	bool bCollision = false;
	
	
	//타일리스트를 돌려서 반직선과 만나는 iterator를 멈춘다.
	for(m_TileListIter = m_TileList.begin(); m_TileListIter != m_TileList.end(); ++m_TileListIter){
		TILEVERTEX *tileVertex = ( (*m_TileListIter)->GetTileVertex() );
		
		//위쪽 삼각형
		bCollision = CPicking::PickInst().CollisionCheckTriangle(m_vPickPos,
													  ray,
													  tileVertex[0].vPos,
													  tileVertex[1].vPos,
													  tileVertex[2].vPos
												     );
		
		if(bCollision == true) break;
		
		//아래쪽 삼각형
		bCollision = CPicking::PickInst().CollisionCheckTriangle(m_vPickPos,
													  ray,
													  tileVertex[0].vPos,
													  tileVertex[2].vPos,
													  tileVertex[3].vPos
												     );

		if(bCollision == true) break;
		
	}

	/*printf("%f,%f,%f\n", (*m_TileListIter)->GetTileVertex()->vPos.x,
						 (*m_TileListIter)->GetTileVertex()->vPos.y,
						 (*m_TileListIter)->GetTileVertex()->vPos.z
			);*/
	
	if(bCollision == true){
		(*m_TileListIter)->SetMaterial();
	}
	
	
}