#include "Core.h"
#include "..\Scene\SceneManager.h"
#include "..\Core\Timer.h"
#include "PathManager.h"
#include "SourceManager.h"
#include "Texture.h"
#include "CameraManager.h"
#include "InputManager.h"

DEFINE_SINGLETON(CCore)
bool CCore::loop_ = true;

CCore::CCore()
{
	// 메모리 릭 체크
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// 메모리 릭 일어난 곳으로 이동 시켜주는 함수
	// _CrtSetBreakAlloc(576);
}
CCore::~CCore()
{
	// GDi plus 종료
	Gdi_End();

	// 장면 관리자 파괴
	CSceneManager::DestroyInst();

	// 초시계 파괴
	CTimer::DestroyInst();

	// source 관리자 파괴
	CSourceManager::DestroyInst();

	//  path 관리자 파괴
	CPathManager::DestroyInst();

	// 카메라 관리자 파괴
	CCameraManager::DestroyInst();

	// 입력 관리자 파괴
	CInputManager::DestroyInst();

	ReleaseDC(hWnd_, hdc_);
}

bool CCore::Init(HINSTANCE _hInst)
{
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

	// GDI plus 초기화
	Gdi_Init();

	// 타이머 초기화
	if (!CTimer::Instance()->Init())
		return false;

	// PathManager 초기화
	if (!CPathManager::Instance()->Init())
		return false;

	// SourceManager 초기화
	if (!CSourceManager::Instance()->Init(hInst_, hdc_))
		return false;

	// 창 생성
	if (!CSceneManager::Instance()->Init())
		return false;

	// 카메라 관리자 생성
	if (!CCameraManager::Instance()->Init())
		return false;
	
	if (!CInputManager::Instance()->Init())
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

		Logic();
	}
	return (int)msg.wParam;
}

void CCore::Logic()
{
	CTimer::Instance()->Update();

	float delta_time = CTimer::Instance()->GetDeltaTime();

	Input(delta_time);
	Update(delta_time);
	LateUpdate(delta_time);
	Collision(delta_time);
	Render(delta_time);

	// 마우스 입력 초기화
	CInputManager::Instance()->MouseStateReset();
}

void CCore::Input(float _time)
{
	CSceneManager::Instance()->Input(_time);
	// CCameraManager::Instance()->Input(_time);
	CInputManager::Instance()->KeyBoardInput(_time);
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
	// 더블 버퍼링
	
	// 빈 흰색 도화지 그리기
	CTexture* back_buffer = CSourceManager::Instance()->GetBackBuffer();
	// Rectangle(back_buffer->GetDC(), 0, 0, back_buffer->GetWidth(), back_buffer->GetHeight());

	// 컨텐츠 그리기
	CSceneManager::Instance()->Render(back_buffer->GetDC(), _time);

	// 흰색 도화지에 그려진걸 옮기기
	BitBlt(hdc_, 0, 0, back_buffer->GetWidth(), back_buffer->GetHeight(), back_buffer->GetDC(), 0, 0, SRCCOPY);
	
	SAFE_RELEASE(back_buffer);
}


ATOM CCore::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst_;
	wcex.hIcon = LoadIcon(hInst_, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_ICON1);
	wcex.lpszClassName = L"AS21prac";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEMOVE:
		CInputManager::Instance()->SetMousePose(lParam);
		break;
	case WM_LBUTTONDOWN:
		CInputManager::Instance()->MouseInput(LBUTTON_DOWN, lParam);
		break;
	case WM_LBUTTONUP:
		CInputManager::Instance()->MouseInput(LBUTTON_UP, lParam);
		break;
	case WM_RBUTTONDOWN:
		CInputManager::Instance()->MouseInput(RBUTTON_DOWN, lParam);
		break;
	case WM_RBUTTONUP:
		CInputManager::Instance()->MouseInput(RBUTTON_UP, lParam);
		break;
	case WM_DESTROY:
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

	// 실제 윈도우 타이틀바나 메뉴를 포함한 윈도우 크기를 구한다.
	RECT rc = { 0, 0, wnd_rs_.w, wnd_rs_.h };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);


	// 위에서 구해준 크기로 윈도우 클라이언트 영역의 크기를 원하는 크기로 맞춰줘야한다.
	SetWindowPos(hWnd_, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

	ShowWindow(hWnd_, SW_SHOW);
	UpdateWindow(hWnd_);

	return TRUE;
}