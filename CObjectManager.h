#ifndef _COBJECTMANAGER_H_
#define _COBJECTMANAGER_H_

#include <d3d9.h>

// �� �ӽ÷� �������� ��������. ���߿� FPS Ŭ���� �������� ��
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

	//bool InitAll�ǹ�();
	//void RenderAll�ǹ�();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
private:
	CModel *m_pModel[MAX_MODEL];//�����͹迭

private:
	BOOL* m_bUpdate;
};

#endif