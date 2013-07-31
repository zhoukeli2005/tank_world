#include "Screen.h"
#include "resource.h"
#include "GameObject.h"

using namespace engine;

static LPCSTR winClassName = "TankWorldWinClass";
static LPCSTR winTitle = "坦克世界 - 等你来战";

Screen::Screen(HINSTANCE hinstance, int width, int height)
:m_hinstance(hinstance)
,m_hwnd(NULL)
,m_pdevice9(NULL)
,m_pd3d9(NULL)
,m_root(NULL)
,m_width(width)
,m_height(height)
,m_near(1)
,m_far(10000)
{
	iCreateWindow();

	m_root = new GameObject(this);
}

void Screen::iUpdate()
{
	if(!m_root || !m_pdevice9) {
		return;
	}

	m_pdevice9->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f), 1.0f, 0 );

	m_pdevice9->BeginScene();

	m_root->Draw();

	m_pdevice9->EndScene();
	m_pdevice9->Present(NULL, NULL, NULL, NULL);

	iEnterFrame();
}

void Screen::iCreateWindow()
{
	WNDCLASSEX winClass;
	memset(&winClass, 0, sizeof(WNDCLASSEX));


	winClass.lpszClassName = winClassName;
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = m_hinstance;
	winClass.hIcon	       = LoadIcon(m_hinstance, (LPCTSTR)IDI_DIRECTX_ICON);
	winClass.hIconSm	   = LoadIcon(m_hinstance, (LPCTSTR)IDI_DIRECTX_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = sizeof(Screen *);

	if( !RegisterClassEx(&winClass) ) {
		Warning("RegisterClassEx Failed!");
		return;
	}

	m_hwnd = CreateWindowEx( 
		NULL, winClassName, winTitle,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, m_width, m_height, NULL, NULL, m_hinstance, NULL 
	);

	if( m_hwnd == NULL ) {
		Warning("CreateWindowEx Failed!");
		return;
	}

	SetWindowLong(m_hwnd, 0, (LONG)(this));

	ShowWindow(m_hwnd, 1);
	UpdateWindow(m_hwnd);

	iInitD3D9();
}

void Screen::Loop()
{
	MSG uMsg;
	memset(&uMsg,0,sizeof(uMsg));

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else
			iUpdate();
	}

	iDestroy();

	UnregisterClass( winClassName, m_hinstance );
}

void Screen::iInitD3D9()
{
		m_pd3d9 = Direct3DCreate9( D3D_SDK_VERSION );

		D3DDISPLAYMODE d3ddm;

		m_pd3d9->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof(d3dpp) );

		d3dpp.Windowed               = TRUE;
		d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat       = d3ddm.Format;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

		m_pd3d9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pdevice9 );

		m_pdevice9->SetRenderState( D3DRS_LIGHTING, TRUE );			// 使用光照
		m_pdevice9->SetRenderState( D3DRS_SPECULARENABLE, TRUE );	// 使用反射

		m_pdevice9->SetRenderState( D3DRS_ZENABLE, TRUE );

		m_pdevice9->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

		D3DXMATRIX pmatrix;
		D3DXMatrixPerspectiveFovLH(&pmatrix, D3DXToRadian( 45.0f ), m_width / m_height, m_near, m_far);
		m_pdevice9->SetTransform(D3DTS_PROJECTION, &pmatrix);		
}

void Screen::iDestroy()
{
	if( m_pdevice9 != NULL )
		m_pdevice9->Release();

	if( m_pd3d9 != NULL )
		m_pd3d9->Release();
}

LRESULT CALLBACK Screen::WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static POINT ptLastMousePosit;
	static POINT ptCurrentMousePosit;
	static bool bMousing;

	Screen * screen = (Screen *)GetWindowLong(hWnd, 0);

	switch( msg )
	{	
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;

			default:
				if(screen) {
					screen->OnKeyDown(wParam);
				}
			}
		}
		break;

	case WM_KEYUP:
		{
			if(screen) {
				screen->OnKeyUp(wParam);
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if(screen) {
				screen->OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if(screen) {
				screen->OnLButtonUp(LOWORD(lParam), HIWORD(lParam));
			}
		}
		break;

	case WM_RBUTTONDOWN:
		{
			if(screen) {
				screen->OnRButtonDown(LOWORD(lParam), HIWORD(lParam));
			}
		}
		break;

	case WM_RBUTTONUP:
		{
			if(screen) {
				screen->OnRButtonUp(LOWORD(lParam), HIWORD(lParam));
			}
		}
		break;

	case WM_MOUSEMOVE:
		{
			if(screen) {
				screen->OnMouseMove(LOWORD(lParam), HIWORD(lParam));
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if(screen) {
				screen->OnMouseWheel(LOWORD(lParam), HIWORD(lParam), (short)HIWORD(wParam) / WHEEL_DELTA);
			}
		}
		break;;

	case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

	default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}