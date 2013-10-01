#include "CD3D.h"

CD3D::CD3D()
{

}
CD3D::~CD3D()
{
	
}
VOID CD3D::Cleanup()
{
	if(m_pd3dDevice != NULL){
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
}
LPDIRECT3DDEVICE9 CD3D::InitD3D(HWND _hWnd)
{
	D3DCAPS9 caps;
	D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;

	// ����̽��� �����ϱ����� D3D��ü ����
	if( NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION) ) ){
		MessageBox(NULL, L"Direct3DCreate9() Failed", MB_OK, NULL);
	}	
	
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ){
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}else{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dpp;		//����̽� ������ ���� ����ü
	ZeroMemory(&d3dpp, sizeof(d3dpp));	//�ݵ�� ZeroMemory()�Լ��� �̸� ����ü�� ������ ������ �Ѵ�.

	d3dpp.Windowed			= TRUE;						//â���� ����
	d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;	//���� ȿ������ SWAPȿ��
	d3dpp.BackBufferCount	= D3DFMT_UNKNOWN;			//���� ����ȭ�� ��忡 ���缭 �ĸ���۸� ����

	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,		//����Ʈ ����ī�带 ���(��κ��� ����ī�尡 1��)
									  deviceType,				//HAL����̽��� �����Ѵ�.(HW������ġ�� ����ϰڴٴ� �ǹ�)
									  _hWnd,
									  vp,
									  &d3dpp,
									  &m_pd3dDevice
									) 
			  )
	  )
	{
		MessageBox(NULL, L"CreateDevice() Failed", MB_OK, NULL);
	}
	
	m_pD3D->Release();

	return m_pd3dDevice;
}