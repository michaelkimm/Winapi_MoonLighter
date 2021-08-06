#include "Core.h"
#include "..\Scene\SceneManager.h"
#include "..\Core\Timer.h"
#include "PathManager.h"
#include "SourceManager.h"
#include "Texture.h"
#include "InputManager.h"
#include "TfManager.h"
#include "..\Scene\MapEditScene.h"
#include "..\Scene\AssistScene.h"

DEFINE_SINGLETON(CCore)
bool CCore::loop_ = true;

CCore::CCore()
{
	// 메모리 릭 체크
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// 메모리 릭 일어난 곳으로 이동 시켜주는 함수
	// _CrtSetBreakAlloc(235);
}
CCore::~CCore()
{
	// 장면 관리자 파괴
	CSceneManager::DestroyInst();

	// 초시계 파괴
	CTimer::DestroyInst();

	// source 관리자 파괴
	CSourceManager::DestroyInst();

	//  path 관리자 파괴
	CPathManager::DestroyInst();

	// 카메라 관리자 파괴
	// CCameraManager::DestroyInst();

	// 입력 관리자 파괴
	CInputManager::DestroyInst();

	// Tf 관리자 파괴
	CTfManager::DestroyInst();

	ReleaseDC(hWnd_, hdc_);
}

bool CCore::Init(HINSTANCE _hInst)
{
	map_edit_mode_ = false;

	pose_ = MY_POSE(0.f, 0.f);

	hInst_ = _hInst;

	// 창 클래스를 만든다.
	MyRegisterClass();

	// 해상도 설정
	wnd_rs_.w = 1280;
	wnd_rs_.h = 720;

	// 인스턴스 핸들을 저장 후, 창을 만든다.
	if (!Create())
		return false;

	// HDC 생성
	hdc_ = GetDC(hWnd_);

	// 타이머 초기화
	if (!CTimer::Instance()->Init())
		return false;

	// PathManager 초기화
	if (!CPathManager::Instance()->Init())
		return false;

	// SourceManager 초기화
	if (!CSourceManager::Instance()->Init(hInst_, hdc_))
		return false;

	// 창 관리자 초기화
	if (!CSceneManager::Instance()->Init())
		return false;

	// 카메라 관리자 생성
	// if (!CCameraManager::Instance()->Init())
	// 	return false;
	
	// 입력 관리자 생성
	if (!CInputManager::Instance()->Init(hWnd_))
		return false;

	// Tf 관리자 생성
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
	// 타이머 dt 얻기
	CTimer::Instance()->Update();
	float delta_time = CTimer::Instance()->GetDeltaTime();

	Input(delta_time);
	Update(delta_time);
	LateUpdate(delta_time);
	Collision(delta_time);
	Render(delta_time);
}

void CCore::Input(float _time)
{
	CInputManager::Instance()->KeyBoardInput(_time);
	CInputManager::Instance()->MouseInput(_time);
	CSceneManager::Instance()->Input(_time);
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
		// GetClientRect(ChildHwnd_[0], &map_edit_size);
		// cout << "map_edit_size: (" << map_edit_size.left << ", " << map_edit_size.top << ", " << map_edit_size.right << ", " << map_edit_size.bottom << ")\n";
		// mx = map_edit_size.right + 5;
		// my = map_edit_size.top;
	}
	// 더블 버퍼링
	
	// 빈 흰색 도화지 그리기
	CTexture* back_buffer = CSourceManager::Instance()->GetBackBuffer();
	// Rectangle(back_buffer->GetDC(), 0, 0, back_buffer->GetWidth(), back_buffer->GetHeight());

	

	// 컨텐츠 그리기
	// CSceneManager::Instance()->Render(back_buffer->GetDC(), _time);

	// 흰색 도화지에 그려진걸 옮기기
	// BitBlt(hdc_, 0, 0, back_buffer->GetWidth() - mx, back_buffer->GetHeight() - my, back_buffer->GetDC(), 0, 0, SRCCOPY);

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
	wcex_.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU2);
	wcex_.lpszClassName = L"AS21prac";
	wcex_.hIconSm = LoadIcon(wcex_.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassExW(&wcex_);

	wcex_.lpfnWndProc = ChildWndProc;
	// wcex_.lpszMenuName = NULL;
	wcex_.lpszMenuName = MAKEINTRESOURCEW(IDR_MAP_TOOL);
	wcex_.lpszClassName = _T("Child Window");
	RegisterClassExW(&wcex_);

	wcex_.lpfnWndProc = MapEditProc;
	wcex_.lpszMenuName = NULL;
	wcex_.lpszClassName = _T("MapEdit Window");
	RegisterClassExW(&wcex_);

	wcex_.lpfnWndProc = TileSetProc;
	wcex_.lpszMenuName = NULL;
	wcex_.lpszClassName = _T("TileSet Window");
	RegisterClassExW(&wcex_);

	return NULL;
}


BOOL CCore::Create()
{
	hWnd_ = CreateWindowW(L"AS21prac", L"AS21prac", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst_, nullptr);

	if (!hWnd_)
	{
		return FALSE;
	}

	// 실제 윈도우 타이틀바나 메뉴를 포함한 윈도우 크기를 구한다.
	RECT rc = { 0, 0, wnd_rs_.w, wnd_rs_.h };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);


	// 위에서 구해준 크기로 윈도우 클라이언트 영역의 크기를 원하는 크기로 맞춰줘야한다.
	SetWindowPos(hWnd_, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

	ShowWindow(hWnd_, SW_SHOW);
	UpdateWindow(hWnd_);


	// child 윈도우 생성
	RECT rectView;
	GetClientRect(hWnd_, &rectView);

	// 부모 윈도우 사이즈를 배경 사이즈로 설정
	// CCameraManager::Instance()->SetWorldSize(rectView.right, rectView.bottom);

	// 가로 알파 간견
	int dx = 1;
	float size_coef = 0.4;
	float size_x = rectView.right * size_coef;
	float size_y = rectView.bottom;

	/*ChildHwnd_[0] = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("MapEdit"),
		NULL,
		WS_CHILD | WS_VISIBLE,
		0,
		0,
		rectView.right - size_x,
		size_y,
		hWnd_,
		NULL,
		hInst_,
		NULL
	);

	ChildHwnd_[1] = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("TileSet"),
		NULL,
		WS_CHILD | WS_VISIBLE,
		rectView.right - size_x + dx,
		0,
		size_x - dx,
		size_y,
		hWnd_,
		NULL,
		hInst_,
		NULL
	);*/

	CTfManager::Instance()->LoadTf("TileSetProc", MY_POSE{ rectView.right - size_x + dx, 0.f });

	return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT	rect_view;
	static HWND hwndClient;
	CLIENTCREATESTRUCT clientcreate;
	MDICREATESTRUCT mdicreate;
	HWND hwndChild;

	int dx = 0;
	float size_coef = 1;
	float size_x = 0;
	float size_y = 0;
	MY_SIZE tmp_rect(0.f, 0.f);

	switch (message)
	{
	case WM_CREATE:
		// 윈도우 핸들 Init
		GetClientRect(hWnd, &rect_view);

		// 크기 조절하려고 tmp_rect 만들었지만, 무슨 이유에선지 안됌.
		tmp_rect = MY_SIZE(rect_view.right - rect_view.left, rect_view.bottom - rect_view.top);
		tmp_rect *= size_coef;

		CSceneManager::Instance()->LoadHwnd(INGAME_SCENE, hWnd);

		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hWnd), 0);
		clientcreate.idFirstChild = 100;
		hwndClient = CreateWindow(
			_T("MDICLIENT"),
			NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
			0, 0, rect_view.right, rect_view.bottom + 100,
			hWnd,
			NULL,
			CCore::Instance()->GetHInstance(),
			(LPSTR)&clientcreate);

		ShowWindow(hwndClient, SW_SHOW);

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILENEW:
			mdicreate.szClass = _T("Child Window");
			mdicreate.szTitle = _T("Child Window");
			mdicreate.hOwner = CCore::Instance()->GetHInstance();
			mdicreate.x = 0;
			mdicreate.y = 0;
			mdicreate.cx = rect_view.right;
			mdicreate.cy = rect_view.bottom; //  (int)tmp_rect.y;
			mdicreate.style = 0;
			mdicreate.lParam = 0;
			hwndChild = (HWND)SendMessage(hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			// cout << "bb\n"; 
			return 0;
		}
		break;

	case WM_MOUSEMOVE:
		// CInputManager::Instance()->SetHwnd(hWnd);
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


LRESULT CCore::ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rect_view;
	HWND tmp_hwnd;

	int dx = 1;
	float size_coef = 0.35;
	float size_x = 0;
	float size_y = 0;

	switch (message)
	{
	case WM_CREATE:
		// 윈도우 핸들 Init
		// CSceneManager::Instance()->LoadHwnd(MAP_EDIT_SCENE, hWnd);

		GetClientRect(hWnd, &rect_view);

		// SetTimer(hWnd, 124, 100, NULL);
		tmp_hwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			_T("MapEdit Window"),
			NULL,
			WS_CHILD | WS_VISIBLE,
			0,
			0,
			rect_view.right - rect_view.right * size_coef, // rect_view.right * size_coef,
			rect_view.bottom,
			hWnd,
			NULL,
			CCore::Instance()->GetHInstance(),
			NULL
		);
		CCore::Instance()->SetChildHwnd(0, tmp_hwnd);

		tmp_hwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			_T("TileSet Window"),
			NULL,
			WS_CHILD | WS_VISIBLE,
			rect_view.right - rect_view.right * size_coef + dx,
			0,
			rect_view.right,
			rect_view.bottom,
			hWnd,
			NULL,
			CCore::Instance()->GetHInstance(),
			NULL
		);
		CCore::Instance()->SetChildHwnd(1, tmp_hwnd);

		break;

	case WM_SIZE:
		GetClientRect(hWnd, &rect_view);
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_MOUSEMOVE:
		CInputManager::Instance()->SetHwnd(hWnd);
		break;

	case WM_DESTROY:
		// KillTimer(hWnd, 124);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


LRESULT CCore::MapEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rectView;

	switch (message)
	{
	case WM_CREATE:
		// 윈도우 핸들 Init
		CSceneManager::Instance()->LoadHwnd(MAP_EDIT_SCENE, hWnd);
		CSceneManager::Instance()->CreateScene<CMapEditScene>(SC_MAP_EDIT, hWnd);

		GetClientRect(hWnd, &rectView);

		SetTimer(hWnd, 124, 100, NULL);

		break;

	case WM_SIZE:
		GetClientRect(hWnd, &rectView);
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// 더블 버퍼링 준비
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hDoubleBufferBitmap = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferBitmap);
		Rectangle(hMemDC, 0, 0, 2000, 2000);


		// 컨텐츠 그리기
		if (CSceneManager::Instance()->pt_map_edit_scene_)
			CSceneManager::Instance()->pt_map_edit_scene_->Render(hMemDC, 0);
		BitBlt(hdc, 0, 0, 2000, 2000, hMemDC, 0, 0, SRCCOPY);


		// DC 삭제
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
		DeleteObject(hDoubleBufferBitmap);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_MOUSEMOVE:
		CInputManager::Instance()->SetHwnd(hWnd);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 124);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


LRESULT CCore::TileSetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rectView;

	switch (message)
	{
	case WM_CREATE:
		// 윈도우 핸들 Init
		CSceneManager::Instance()->LoadHwnd(ASSIST_SCENE, hWnd);
		CSceneManager::Instance()->CreateScene<CAssistScene>(SC_ASSIST, hWnd);


		GetClientRect(hWnd, &rectView);

		SetTimer(hWnd, 123, 100, NULL);
		
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &rectView);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// 더블 버퍼링 준비
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hDoubleBufferBitmap = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferBitmap);
		Rectangle(hMemDC, 0, 0, 2000, 2000);


		// 컨텐츠 그리기
		if (CSceneManager::Instance()->pt_assist_scene_)
			CSceneManager::Instance()->pt_assist_scene_->Render(hMemDC, 0);
		BitBlt(hdc, 0, 0, 2000, 2000, hMemDC, 0, 0, SRCCOPY);


		// DC 삭제
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
		DeleteObject(hDoubleBufferBitmap);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_MOUSEMOVE:
		CInputManager::Instance()->SetHwnd(hWnd);
		break;

	case WM_DESTROY:
		KillTimer(hWnd ,123);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
