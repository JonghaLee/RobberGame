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

	// 디바이스를 생성하기위한 D3D객체 생성
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

	D3DPRESENT_PARAMETERS d3dpp;		//디바이스 생성을 위한 구조체
	ZeroMemory(&d3dpp, sizeof(d3dpp));	//반드시 ZeroMemory()함수로 미리 구조체를 깨끗이 지워야 한다.

	d3dpp.Windowed			= TRUE;						//창모드로 생성
	d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;	//가장 효율적인 SWAP효과
	d3dpp.BackBufferCount	= D3DFMT_UNKNOWN;			//현재 바탕화면 모드에 맞춰서 후면버퍼를 생성

	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,		//디폴트 비디오카드를 사용(대부분은 비디오카드가 1개)
									  deviceType,				//HAL디바이스를 생성한다.(HW가속장치를 사용하겠다는 의미)
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