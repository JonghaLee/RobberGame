#include "WinBase.h"

#include "CD3D.h"
#include "CDirector.h"
#include "EnumCollect.h"
#include "CObjectDlg.h"
#include "CTextureDlg.h"

int WinClass::MessageLoop(HINSTANCE hInstance)// �޽���ť
{
	MSG Message;

	m_WC().MyRegisterClass(hInstance);
	if(m_WC().InitInstance(hInstance, SW_SHOWNORMAL)){

		while(true){
			if(PeekMessage(&Message,NULL,0,0,PM_REMOVE)){
				if(Message.message == WM_QUIT) break;
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}else{
				if(m_WC().g_pd3dDevice != NULL){
					m_WC().Director->RenderAll(m_WC().g_pd3dDevice);
				}				
			}
		}
	}

	m_WC().MyD3D->Cleanup();
	
	delete m_WC().ObjectDlg;
	delete m_WC().TextureDlg;
	delete m_WC().MyD3D;
	delete m_WC().Director;

	return Message.wParam;
}

WinClass& WinClass::m_WC(void){	// Instance
	static WinClass winClass;
	return winClass;
}

WinClass::WinClass()
{
	m_lpszTitle = TEXT("WinBase");

	m_WC().MyD3D = NULL;
	m_WC().Director = NULL;
	
	m_WC().g_pd3dDevice = NULL;
	m_WC().TextureDlg = NULL;
}

WinClass::~WinClass()
{
	
}

ATOM WinClass::MyRegisterClass(HINSTANCE hInstance)// ������ Ŭ���� ���
{
	WNDCLASS WndClass;
	
	//���� ������
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);	//��� ����
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);	//�ּ�ȭ�Ǿ��� ��� ��µ� ������ ����
	WndClass.hInstance=hInstance;				//������ Ŭ������ ����ϴ� ���α׷��� ��ȣ
	WndClass.lpfnWndProc=(WNDPROC)WndProc;		//�޽��� ó�� �Լ� ����
	WndClass.lpszClassName=m_lpszTitle;			//������ Ŭ������ �̸�
	WndClass.lpszMenuName=NULL;					//���α׷��� ����� �޴� ����
	WndClass.style=CS_HREDRAW | CS_VREDRAW;		//����,���� ũ�Ⱑ ���Ұ�� ������ �ٽ� �׸�

	RegisterClass(&WndClass);
	
	//VIEW ������
	WndClass.lpfnWndProc = (WNDPROC)ViewWndProc;
	WndClass.lpszClassName = TEXT("View");
	RegisterClass(&WndClass);

	//control ������(����)
	WndClass.lpfnWndProc = (WNDPROC)ControlWndProc;
	WndClass.lpszClassName = TEXT("Control");
	RegisterClass(&WndClass);

	return 0;
}
BOOL WinClass::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	m_Inst = hInstance;

	HWND hWnd;

	hWnd=CreateWindow(m_lpszTitle,			//�����ϰ��� �ϴ� �������� Ŭ������ �����ϴ� ���ڿ�
					  m_lpszTitle,			//�������� Ÿ��Ʋ �ٿ� ��Ÿ�� ���ڿ�
					  WS_OVERLAPPEDWINDOW,	//������� �ϴ� �������� ���¸� �����ϴ� �μ�(Ÿ��Ʋ��,��ũ�ѹ� ���� ���)
					  100,					//X
					  100,					//Y
					  1000,		//WIDTH
					  700,		//HEIGHT
					  NULL,					//�θ� �����찡 ���� ��� �θ� �������� �ڵ�
					  (HMENU)NULL,			//�����쿡�� ����� �޴��� �ڵ�
					  hInstance,			//������ ����� ��ü, �� ���α׷��� �ڵ� ����
					  NULL
						);
	//������ ����
	m_WC().CreateWnd(hWnd);

	m_WC().MyD3D = new CD3D();
	m_WC().g_pd3dDevice = MyD3D->InitD3D(m_hViewWnd);
	
	m_WC().TextureDlg = new CTextureDlg(hWnd);
	m_WC().ObjectDlg = new CObjectDlg(hWnd);

	if(m_WC().g_pd3dDevice == NULL){
		return false;
	}
	
	// Dialog ����
	m_WC().CreateDlg();

	m_WC().Director = new CDirector(m_WC().g_pd3dDevice, m_WC().m_hViewWnd, m_WC().m_hControlWnd, m_WC().m_Inst);


	m_WC().g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_WC().g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	m_WC().g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	m_WC().g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );	
	
	
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj;

	/// �������    
    D3DXMatrixIdentity( &matWorld );							/// ��������� ����������� ����
    
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );		/// ����̽��� ������� ����

    /// ������� ����
    D3DXVECTOR3 vEyePt( 0.0f, 50.0f, -250.0f);
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    /// �������� ��� ����
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 10000.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );



	ShowWindow(hWnd,nCmdShow);

	return true;
}

void WinClass::CreateDlg()
{
	RECT rt;
	GetClientRect(m_WC().m_hControlWnd,&rt);

	if(!IsWindow(m_WC().m_hTextureDlg)){
		
		m_hTextureDlg = CreateDialog(m_WC().m_Inst, MAKEINTRESOURCE(IDD_TEX_DLG), m_WC().m_hControlWnd, TextureDlgProc);
		//ControlWindow ũ��� ���̾�α� ũ�� ����
		SetWindowPos(m_hTextureDlg, (HWND)0, 0, 0, rt.right, rt.bottom-30, SWP_NOMOVE);
		ShowWindow(m_hTextureDlg, SW_SHOW);
	}
}

LRESULT CALLBACK WinClass::WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage){
		case WM_CREATE:
			return 0;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case ID_ADDTEX:
						
				break;
				case ID_DELTEX:
					
				break;

			}
			
		return TRUE;
		case WM_DESTROY:
			PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

LRESULT CALLBACK WinClass::ViewWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return m_WC().Director->ViewWndProc(hWnd, iMessage, wParam, lParam);
}

LRESULT CALLBACK WinClass::ControlWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return m_WC().Director->ControlWndProc(hWnd, iMessage, wParam, lParam);
}

BOOL CALLBACK WinClass::TextureDlgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return m_WC().TextureDlg->TextureDlgProc(hWnd, iMessage, wParam, lParam);
}

BOOL CALLBACK WinClass::ObjDlgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//return m_WC().TextureDlg->TextureDlgProc(hWnd, iMessage, wParam, lParam);
	return true;
}

void WinClass::CreateWnd(HWND _hMain)
{
	RECT rt;
	GetClientRect(_hMain, &rt);

	//View ���ϵ� ������ ����
	m_WC().m_hViewWnd =  CreateWindow(TEXT("View"),NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
								0,30,800,rt.bottom,_hMain,0,m_WC().m_Inst,NULL);
	//���� ��Ʈ�� ���ϵ� ������ ����
	m_WC().m_hControlWnd = CreateWindow(TEXT("Control"),NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
								800,30,rt.right,rt.bottom-150,_hMain,0,m_WC().m_Inst,NULL);
}

void WinClass::CreateToolBar(HWND _hMain)
{
	TBBUTTON ToolBtn[8]={
		//bReserved, dwData, fsState, fsStyle, iBitmap, idCommand, iString
		{0,OBJECT,TBSTATE_ENABLED | TBSTATE_CHECKED,TBSTYLE_CHECKGROUP,0,0,0,0},
		{1,TERRAIN,TBSTATE_ENABLED,TBSTYLE_CHECKGROUP,0,0,0,1},
		//{2,12,TBSTATE_ENABLED,TBSTYLE_CHECKGROUP,0,0,0,2},
		//{3,13,TBSTATE_ENABLED,TBSTYLE_CHECKGROUP,0,0,0,3}
	};

	TCHAR *szString=L"TEXTURE\0OBJECT\0";
	
	INITCOMMONCONTROLSEX InitCtrlEx;
	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC = ICC_BAR_CLASSES;

	InitCommonControlsEx(&InitCtrlEx);
	m_WC().m_hToolBar=CreateToolbarEx(_hMain, WS_CHILD | WS_VISIBLE | WS_BORDER,
		//| TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
		ID_TOOLBAR, 2, m_WC().m_Inst, 0, ToolBtn, 2,
		16,16,16,16,sizeof(TBBUTTON));
	SendMessage(m_WC().m_hToolBar,TB_ADDSTRING,NULL,(LPARAM)szString);
	
	
	TBREPLACEBITMAP rb;
	int bx, by;
	rb.hInstOld = rb.hInstNew = HINST_COMMCTRL;
	bx = 60; by = 0;
	rb.nButtons = 3;
	
	SendMessage(m_WC().m_hToolBar, TB_REPLACEBITMAP, 0, (LPARAM)&rb);
	SendMessage(m_WC().m_hToolBar, TB_SETBITMAPSIZE, 0, (LPARAM)MAKELONG(bx,by));
	SendMessage(m_WC().m_hToolBar,TB_AUTOSIZE,0,0);
}