#include "Core.h"
#include "..\Scene\SceneManager.h"
#include "..\Core\Timer.h"
#include "PathManager.h"
#include "SourceManager.h"
#include "Texture.h"
#include "InputManager.h"
#include "TfManager.h"

DEFINE_SINGLETON(CCore)
bool CCore::loop_ = true;

CCore::CCore()
{
	// �޸� �� üũ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// �޸� �� �Ͼ ������ �̵� �����ִ� �Լ�
	// _CrtSetBreakAlloc(215);
}
CCore::~CCore()
{
	// ��� ������ �ı�
	CSceneManager::DestroyInst();

	// �ʽð� �ı�
	CTimer::DestroyInst();

	// source ������ �ı�
	CSourceManager::DestroyInst();

	//  path ������ �ı�
	CPathManager::DestroyInst();

	// ī�޶� ������ �ı�
	// CCameraManager::DestroyInst();

	// �Է� ������ �ı�
	CInputManager::DestroyInst();

	// Tf ������ �ı�
	CTfManager::DestroyInst();

	ReleaseDC(hWnd_, hdc_);

	// GDi plus ����
	Gdi_End();
}

bool CCore::Init(HINSTANCE _hInst)
{
	map_edit_mode_ = false;

	pose_ = MY_POSE(0.f, 0.f);

	hInst_ = _hInst;

	// â Ŭ������ �����.
	MyRegisterClass();

	// �ػ� ����
	wnd_rs_.w = 1280;
	wnd_rs_.h = 720;

	// �ν��Ͻ� �ڵ��� ���� ��, â�� �����.
	if (!Create())
		return false;

	// HDC ����
	hdc_ = GetDC(hWnd_);

	// GDI plus �ʱ�ȭ
	Gdi_Init();

	// Ÿ�̸� �ʱ�ȭ
	if (!CTimer::Instance()->Init())
		return false;

	// PathManager �ʱ�ȭ
	if (!CPathManager::Instance()->Init())
		return false;

	// SourceManager �ʱ�ȭ
	if (!CSourceManager::Instance()->Init(hInst_, hdc_))
		return false;

	// â ������ �ʱ�ȭ
	if (!CSceneManager::Instance()->Init())
		return false;

	// ī�޶� ������ ����
	// if (!CCameraManager::Instance()->Init())
	// 	return false;
	
	if (!CInputManager::Instance()->Init(hWnd_))
		return false;

	// Tf ������ �ʱ�ȭ
	if (!CTfManager::Instance()->Init())
		return false;

	return true;
}

int CCore::Run()
{
	MSG msg;

	while (loop_)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Logic();
		}
	}
	return (int)msg.wParam;
}

void CCore::Logic()
{
	// Ÿ�̸� dt ���
	CTimer::Instance()->Update();
	float delta_time = CTimer::Instance()->GetDeltaTime();
	
	Input(delta_time);
	Update(delta_time);
	LateUpdate(delta_time);
	Collision(delta_time);
	Render(delta_time);

	/*MY_POSE WndProc_tf = CTfManager::Instance()->GetTf("WndProc");
	MY_POSE TileSetProc_tf = CTfManager::Instance()->GetTf("TileSetProc");
	cout << "WndProc_tf - TileSetProc_tf: (" << TileSetProc_tf.x << ", " << TileSetProc_tf.y << ")" << endl;*/
}

void CCore::Input(float _time)
{
	CInputManager::Instance()->KeyBoardInput(_time);
	CInputManager::Instance()->MouseInput(_time);
	// CSceneManager::Instance()->Input(_time);
}

void CCore::Update(float _time)
{
	CSceneManager::Instance()->Update(_time);
	// CCameraManager::Instance()->Update(_time);
}

void CCore::LateUpdate(float _time)
{
	CSceneManager::Instance()->LateUpdate(_time);
}

void CCore::Collision(float _time)
{
	CSceneManager::Instance()->Collision(_time);
}

void CCore::Render(float _time)
{
	RECT map_edit_size{ 0, 0, 0, 0 };
	int mx = 0;
	int my = 0;
	if (map_edit_mode_)
	{
		GetClientRect(ChildHwnd_[0], &map_edit_size);
		cout << "map_edit_size: (" << map_edit_size.left << ", " << map_edit_size.top << ", " << map_edit_size.right << ", " << map_edit_size.bottom << ")\n";
		mx = map_edit_size.right + 5;
		my = map_edit_size.top;
	}
	// ���� ���۸�
	
	// �� ��� ��ȭ�� �׸���
	CTexture* back_buffer = CSourceManager::Instance()->GetBackBuffer();

	RECT rectView;
	GetClientRect(hWnd_, &rectView);

	HDC hdc = CreateCompatibleDC(hdc_);
	HBITMAP hDoubleBufferImage = CreateCompatibleBitmap(hdc_, rectView.right, rectView.bottom);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdc, hDoubleBufferImage);

	Graphics graphics(hdc);
	graphics.DrawImage(back_buffer->GetImg(), 0, 0, back_buffer->GetWidth(), back_buffer->GetHeight());

	// ������ �׸���	
	CSceneManager::Instance()->Render(hdc, _time);

	// ��� ��ȭ���� �׷����� �ű��
	BitBlt(hdc_, 0, 0, back_buffer->GetWidth() - mx, back_buffer->GetHeight() - my, hdc, 0, 0, SRCCOPY);

	SelectObject(hdc, hOldBitmap);
	DeleteDC(hdc);
	DeleteObject(hDoubleBufferImage);
	DeleteObject(hOldBitmap);
	SAFE_RELEASE(back_buffer);
}


ATOM CCore::MyRegisterClass()
{
	wcex_.cbSize = sizeof(WNDCLASSEX);

	wcex_.style = CS_HREDRAW | CS_VREDRAW;
	wcex_.lpfnWndProc = WndProc;
	wcex_.cbClsExtra = 0;
	wcex_.cbWndExtra = 0;
	wcex_.hInstance = hInst_;
	wcex_.hIcon = LoadIcon(hInst_, MAKEINTRESOURCE(IDI_ICON1));
	wcex_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex_.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex_.lpszMenuName = MAKEINTRESOURCEW(IDI_ICON1);
	wcex_.lpszClassName = L"AS21prac";
	wcex_.hIconSm = LoadIcon(wcex_.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassExW(&wcex_);

	wcex_.lpfnWndProc = TileSetProc;
	wcex_.lpszMenuName = NULL;
	wcex_.lpszClassName = L"TileSet";
	RegisterClassExW(&wcex_);

	return NULL;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		// ������ �ڵ� Init
		CSceneManager::Instance()->LoadHwnd(INGAME_SCENE, hWnd);
		CSceneManager::Instance()->LoadHwnd(MAP_EDIT_SCENE, hWnd);

		//// ������ ��ġ ������ ������Ʈ for Tf
		//RECT tmp_rect;
		//GetWindowRect(hWnd, &tmp_rect);

		//POINT tmp_point;
		//tmp_point = POINT{ tmp_rect.left, tmp_rect.top };
		//ScreenToClient(hWnd, &tmp_point);
		//cout << "tmp_point: (" << tmp_point.x << ", " << tmp_point.y << ")\n";

		//CTfManager::Instance()->LoadTf("WndProc", MY_POSE(tmp_rect.left, tmp_rect.top));

		break;

	case WM_MOUSEMOVE:
		CInputManager::Instance()->SetHwnd(hWnd);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



LRESULT CCore::TileSetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		// ������ �ڵ� Init
		CSceneManager::Instance()->LoadHwnd(ASSIST_SCENE, hWnd);

		// SetTimer(hWnd, 123, 20, NULL);
		// ������ ��ġ ������ ������Ʈ for Tf
		/*RECT tmp_rect;
		GetWindowRect(hWnd, &tmp_rect);

		POINT tmp_point;
		tmp_point = POINT{ tmp_rect.left, tmp_rect.top };
		ScreenToClient(hWnd, &tmp_point);
		cout << "tmp_point: (" << tmp_point.x << ", " << tmp_point.y << ")\n";*/

		// CTfManager::Instance()->LoadTf("TileSetProc", MY_POSE(tmp_rect.left, tmp_rect.top));
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// ������ �׸���
		CSceneManager::Instance()->pt_assist_scene_->Render(hdc, 0);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_MOUSEMOVE:
		CInputManager::Instance()->SetHwnd(hWnd);
		break;

	case WM_DESTROY:
		// KillTimer(hWnd ,123);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL CCore::Create()
{
	hWnd_ = CreateWindowW(L"AS21prac", L"AS21prac", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst_, nullptr);

	if (!hWnd_)
	{
		return FALSE;
	}

	// ���� ������ Ÿ��Ʋ�ٳ� �޴��� ������ ������ ũ�⸦ ���Ѵ�.
	RECT rc = { 0, 0, wnd_rs_.w, wnd_rs_.h };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);


	// ������ ������ ũ��� ������ Ŭ���̾�Ʈ ������ ũ�⸦ ���ϴ� ũ��� ��������Ѵ�.
	SetWindowPos(hWnd_, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

	ShowWindow(hWnd_, SW_SHOW);
	UpdateWindow(hWnd_);

	
	// child ������ ����
	RECT rectView;
	GetClientRect(hWnd_, &rectView);

	// �θ� ������ ����� ��� ������� ����
	// CCameraManager::Instance()->SetWorldSize(rectView.right, rectView.bottom);

	// ���� ���� ����
	int dx = 1;
	float size_coef = 0.4;
	float size_x = rectView.right * size_coef;
	float size_y = rectView.bottom;

	ChildHwnd_[0] = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("TileSet"),
		NULL,
		WS_CHILD | WS_VISIBLE,
		rectView.right - size_x + dx,
		0,
		size_x - dx,
		size_y,
		CCore::Instance()->GetHWnd(),
		NULL,
		CCore::Instance()->GetHInstance(),
		NULL
	);

	CTfManager::Instance()->LoadTf("TileSetProc", MY_POSE{ rectView.right - size_x + dx, 0.f });

	return TRUE;
}