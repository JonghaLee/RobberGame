#include "CObjectManager.h"

#include "Model.h"

#include <iostream>
using namespace std;

CObjectManager::CObjectManager(LPDIRECT3DDEVICE9 _pd3dDevice)
{
	m_pd3dDevice = _pd3dDevice;
	m_bUpdate = new BOOL;
	*m_bUpdate = FALSE;
	//m_pModel = NULL;
	//m_pModel = new CModel();
	memset(m_pModel, NULL, sizeof(m_pModel));

}

CObjectManager::~CObjectManager()
{
	if(m_bUpdate != NULL)
	{
		delete m_bUpdate;
		m_bUpdate = NULL;
	}

	for(int i=0; i<MAX_MODEL; ++i)
	{
		if(m_pModel[i] != NULL)
		delete m_pModel[i];
	}
}

bool CObjectManager::InitAllModel()
{
	/*if(m_pModel != NULL)
		delete m_pModel;*/

	//�ϴ��� �𵨰�ü 10�� ��� ����.
	/*for(int i=0; i<MAX_MODEL; ++i)
	{
		if(m_pModel[i] == NULL)
			m_pModel[i] = new CModel();
	}*/

	/*m_pModel->CreateModel(m_pd3dDevice, m_bUpdate, &g_fPostTime);
	m_pModel->CreateMD2(L"MD2/Ogros.md2");
	m_pModel->Rotate(0.0f, 0.0f, 0.0f);
	m_pModel->CreateMeshVertex();
	m_pModel->CreateMeshIndex();

	m_pModel->VertexGeometry();
	m_pModel->IndexGeometry();*/

	return true;
}

void CObjectManager::UpdateAllModel()
{
	//D3DXMATRIXA16 matWorld;
	//D3DXMatrixIdentity(&matWorld);
	//
	//static float x = 0.0f;
	//x += 10.0f;
	//
	//D3DXMatrixTranslation(&matWorld, x, 0.0f, 0.0f);

	//m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// �ִϸ��̼� �� ���������� ������Ʈ �Ǿ��ϴ°͵�.
//	m_pModel->Animation(CMD2Model::ATTACK_NOACTION, 1);
	//g_fPostTime+=0.01f;
	for(int i=0; i<MAX_MODEL; ++i)
	{
		if(m_pModel[i] != NULL)
		{
			m_pModel[i]->Animation(CMD2Model::STAND, 1);
			//m_pd3dDevice->SetTransform(D3DTS_WORLD, m_pModel[i]->GetWorldTransform() );
			//// TODO : 0930. ����. ������������ ������� ������ �ǰ� ����� �Ѵ�.
		}

	}
	
}

void CObjectManager::RenderAllModel()
{
	//m_pModel->Render();
	for(int i=0; i<MAX_MODEL; ++i)
	{
		if(m_pModel[i] != NULL)
			//m_pModel[i]->Animation(CMD2Model::STAND, 1);
			m_pModel[i]->Render();
	}

}

bool CObjectManager::AddModel(LPCTSTR _fileName)
{
	for(int i=0; i<MAX_MODEL; ++i)
	{
		if(m_pModel[i] == NULL)			// 1. �ش��ϴ� �ڸ��� ������
		{
				// 2. ����
			ModelSetting(i, _fileName);
			
			cout<<"���� �Ϸ�"<<endl;
			return true;
		}
	}

	cout<<"���� ����"<<endl;
	return false;
}

void CObjectManager::ModelSetting(int _iValue, LPCTSTR _fileName)
{
	//static float i = 0.0f;
	m_pModel[_iValue] = new CModel();
	m_pModel[_iValue]->CreateModel(m_pd3dDevice, m_bUpdate, &g_fPostTime);
	m_pModel[_iValue]->CreateMD2(_fileName);
	m_pModel[_iValue]->Rotate(0.0f, 0.0f, 0.0f);
	//m_pModel[_iValue]->Move(i, 0.0f, 0.0f);
	m_pModel[_iValue]->CreateMeshVertex();
	m_pModel[_iValue]->CreateMeshIndex();

	m_pModel[_iValue]->VertexGeometry();
	m_pModel[_iValue]->IndexGeometry();

	//i+= 20.0f;
}

bool CObjectManager::ModelMove(int _iValue, float _fX, float _fY, float _fZ)
{
	D3DXMATRIXA16 *temp = NULL;
	HRESULT hr;

	if(m_pModel[_iValue] != NULL)
	{
		m_pModel[_iValue]->Move(_fX, _fY, _fZ);
		//D3DXMATRIXA16 mTemp;
		//mTemp = *(m_pModel[_iValue]->GetWorldTransform());
		//m_pd3dDevice->SetTransform(D3DTS_WORLD, &mTemp );
		hr = m_pd3dDevice->SetTransform(D3DTS_WORLD, m_pModel[_iValue]->GetWorldTransform() );
		
		//m_pModel[_iValue]->Scale(2.0f, 2.0f, 2.0f);
		
		

		//m_pModel[_iValue]->GetWorldTransform();
		cout<<"Model Move �Ϸ�"<<endl;
		return true;
	}
	else
	{
		cout<<"Model Move ����"<<endl;
		return false;
	}

	
}