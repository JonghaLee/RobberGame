#ifndef _COBJECTMANAGER_H_
#define _COBJECTMANAGER_H_

#include <d3d9.h>

// ↓ 임시로 전역으로 만들어놨음. 나중에 FPS 클래스 가꼬오면 됨
static float g_fPostTime = 0.03f;
#define MAX_MODEL 10

class CModel;

class CObjectManager
{
public:
	CObjectManager(LPDIRECT3DDEVICE9 _pd3dDevice);
	~CObjectManager();

	bool InitAllModel();
	void UpdateAllModel();
	void RenderAllModel();

	bool AddModel(LPCTSTR _fileName);
	void ModelSetting(int _iValue, LPCTSTR _fileName);
	bool ModelMove(int _iValue, float _fX, float _fY, float _fZ);

	//bool InitAll건물();
	//void RenderAll건물();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
private:
	CModel *m_pModel[MAX_MODEL];//포인터배열

private:
	BOOL* m_bUpdate;
};

#endif