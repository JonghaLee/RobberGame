#include "CDirector.h"
#include "CTileManager.h"
#include "CObjectManager.h"

CDirector::CDirector(LPDIRECT3DDEVICE9 _device, HWND _hView, HWND _hControl, HINSTANCE _inst)
{
	//���°��� ���Ƽ� �ϴ� ����� ����......
	m_pD3Ddevice = _device;

	TileManager = NULL;
	ObjectManager = NULL;
	TileManager = new CTileManager(_device);
	ObjectManager = new CObjectManager(_device);
	
	ObjectManager->InitAllModel();
}
CDirector::~CDirector()
{
	
	if(ObjectManager != NULL){ delete ObjectManager; }
	if(TileManager != NULL){ delete TileManager; }
	
}

void CDirector::RenderAll(LPDIRECT3DDEVICE9 _device)
{
	if(NULL == _device) return;
	// �ĸ���۸� �Ķ������� �����.
	_device->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	// ������ ����
	if( SUCCEEDED(_device->BeginScene()) ){
		
		TileManager->RenderAllTile(_device);
		ObjectManager->UpdateAllModel();
		ObjectManager->RenderAllModel();

		// ������ ����
		_device->EndScene();
	}
	
	// �ĸ���۸� ���̴� ȭ������
	_device->Present( NULL, NULL, NULL, NULL);
}



LRESULT CALLBACK CDirector::ViewWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	switch(iMessage){
		case WM_CREATE:
			
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_LBUTTONDOWN:
			OnLButtonDown(hWnd, lParam);
			break;
		case WM_RBUTTONDOWN:
			ObjectManager->ModelMove(1, 2.0f, 0.0f, 0.0f);
			break;
		case WM_LBUTTONUP:
			break;
		case WM_MOUSEMOVE:
			break;
	}

	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

LRESULT CALLBACK CDirector::ControlWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{	
	switch(iMessage){
		case WM_CREATE:
			
			break;
		case WM_LBUTTONDOWN:
			
			break;
		case WM_COMMAND:

			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void CDirector::OnLButtonDown(HWND _hView, LPARAM _lparam)
{
	TileManager->OnLButtonDown(m_pD3Ddevice, _hView, _lparam);
	ObjectManager->AddModel(L"MD2/Ogros.md2");
}

