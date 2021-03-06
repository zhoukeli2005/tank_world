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
,m_last_draw(0)
,m_width(width)
,m_height(height)
,m_near(1)
,m_far(10000)
,m_fps(0)
,m_fire_energy(0)
,m_fps_time(0)
,m_fps_count(0)
,m_remark_time(0)
{
	// create window & d3d
	iCreateWindow();

	// create font
	D3DXCreateFont(m_pdevice9,
		18, 18,	// height, width
		500,	// bold
		1,		// Mipmap
		0,		// 是否斜体
		DEFAULT_CHARSET, 
		OUT_TT_ONLY_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"Arial", 
		&m_font);

	m_root = new GameObject(this);
}

Screen::~Screen()
{

}

void Screen::iUpdate()
{
	if(!m_root || !m_pdevice9) {
		return;
	}
	
	int now = GetTickCount();

	if(now - m_last_draw > 10) {

		calculateFPS(now);
	
		m_pdevice9->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f), 1.0f, 0 );

		m_pdevice9->BeginScene();

		m_root->Draw();

		RECT rect;

		// draw fps

		char buf[32];
		memset(buf, 0, sizeof(buf));
		memcpy(buf, "FPS:", 4);
		itoa((int)m_fps, buf + 4, 10);

		rect.left = 10;
		rect.top = 10;
		rect.right = rect.left + 1000;
		rect.bottom = 100;
		m_font->DrawText(NULL, buf, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(0xFF, 0x00, 0x00));

		// draw introduction
		rect.left = 200;
		rect.top = 10;
		rect.right = rect.left + 1000;
		rect.bottom = 100;
		m_font->DrawText(NULL, "ASWD - 移动，J - 攻击，K - 蓄力攻击，空格 - 切换视角", 
			-1, &rect, DT_LEFT, D3DCOLOR_XRGB(0x00, 0x00, 0x00));

		rect.left = 200;
		rect.top = 30;
		rect.right = rect.left + 1000;
		rect.bottom = 200;
		m_font->DrawText(NULL, "鼠标点击并拖动 - 环顾，鼠标滑轮 - 缩放", 
			-1, &rect, DT_LEFT, D3DCOLOR_XRGB(0x00, 0x00, 0x00));

		// draw Fire Energy
		if(m_fire_energy > 0) {
			memset(buf, 0, sizeof(buf));
			const char * p = "蓄力:";
			int len = strlen(p);
			memcpy(buf, p, len);
			sprintf(buf + len, "%.2f", m_fire_energy);

			rect.left = m_width / 2 - 100;
			rect.top = m_height / 2;
			rect.right = rect.left + 1000;
			rect.bottom = rect.top + 100;
			m_font->DrawText(NULL, buf, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(0xFF, 0xFF, 0x00));
		}

		// draw Remark
		if(m_remark_time > 0) {
			if(now - m_remark_time < 3000) {
				rect.left = m_width / 2 + 100;
				rect.top = m_height / 2;
				rect.right = rect.left + 1000;
				rect.bottom = rect.top + 100;
				m_font->DrawText(NULL, m_remark, -1, &rect, DT_LEFT, D3DCOLOR_XRGB(0x00, 0xFF, 0xFF));
			} else {
				m_remark_time = 0;
			}
		}

		m_pdevice9->EndScene();
		m_pdevice9->Present(NULL, NULL, NULL, NULL);

		m_last_draw = GetTickCount();
	}

	iEnterFrame();
}

void Screen::iCreateWindow()
{
	WNDCLASSEX winClass;
	memset(&winClass, 0, sizeof(WNDCLASSEX));

	m_screen_width = GetSystemMetrics(SM_CXSCREEN);
	m_screen_height = GetSystemMetrics(SM_CYSCREEN);

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
		(m_screen_width - m_width) / 2, (m_screen_height - m_height) / 2, 
		m_width, m_height, NULL, NULL, m_hinstance, NULL 
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
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) ) { 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else {
			iUpdate();
		}
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
			/*D3DCREATE_SOFTWARE_VERTEXPROCESSING*/ D3DCREATE_HARDWARE_VERTEXPROCESSING,
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

void Screen::calculateFPS(int now)
{
	if(!m_fps_time) {
		m_fps_time = now;
	} else {
		m_fps_count++;
		if(now - m_fps_time >= 1000) {
			m_fps = m_fps_count;

			m_fps_count = 0;
			m_fps_time = 0;
		}
	}

}