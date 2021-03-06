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
#include "..\Scene\TileSetSettingScene.h"
#include "..\Object\Object.h"
#include "..\Object\Tile.h"
#include "..\Object\Stage.h"

DEFINE_SINGLETON(CCore)
bool CCore::loop_ = true;

CCore::CCore()
{
	// 메모리 릭 체크
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 메모리 릭 일어난 곳으로 이동 시켜주는 함수
	_CrtSetBreakAlloc(401805);
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

	// PathManager 초기화
	if (!CPathManager::Instance()->Init())
		return false;

	// SourceManager 초기화
	if (!CSourceManager::Instance()->Init(hInst_, hdc_))
		return false;

	// 인스턴스 핸들을 저장 후, 창을 만든다.
	if (!Create())
		return false;

	// HDC 생성
	hdc_ = GetDC(hWnd_);

	// 타이머 초기화
	if (!CTimer::Instance()->Init())
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
			// cout << "peek\n";
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
			// cout << "Logic\n";
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

	// 맵툴 에디터 창
	wcex_.lpfnWndProc = ChildWndProc;
	wcex_.lpszMenuName = MAKEINTRESOURCEW(IDR_MAP_TOOL_MENU);
	wcex_.lpszClassName = _T("Child Window");
	RegisterClassExW(&wcex_);

	// 맵툴 에디터 창 왼쪽
	wcex_.lpfnWndProc = MapEditProc;
	wcex_.lpszMenuName = NULL;
	wcex_.lpszClassName = _T("MapEdit Window");
	RegisterClassExW(&wcex_);

	// 맵툴 에디터 창 오른쪽
	wcex_.lpfnWndProc = TileSetProc;
	wcex_.lpszMenuName = NULL;
	wcex_.lpszClassName = _T("TileSet Window");
	RegisterClassExW(&wcex_);

	// 맵툴 에디터 내 타일셋 세팅 창
	wcex_.lpfnWndProc = TileSettingProc;
	wcex_.lpszMenuName = NULL;
	wcex_.lpszClassName = _T("TileSetting Window");
	RegisterClassExW(&wcex_);

	// 타일 셋 세팅 창 내 타일 셋 보여주는 MDI 윈도우
	wcex_.lpfnWndProc = TileSettingInnerProc;
	wcex_.lpszMenuName = NULL;
	wcex_.lpszClassName = _T("TileSettingInner Window");
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

	CTfManager::Instance()->LoadTf("TileSetProc", MY_POSE{ rectView.right - size_x + dx, 0.f });

	return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT	rect_view;
	static HWND hwndClient;
	// CLIENTCREATESTRUCT clientcreate;
	// MDICREATESTRUCT mdicreate;
	// HWND hwndChild;

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

		// clientcreate.hWindowMenu = GetSubMenu(GetMenu(hWnd), 0);
		// clientcreate.idFirstChild = 100;
		/*hwndClient = CreateWindow(
			_T("MDICLIENT"),
			NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
			0, 0, rect_view.right, rect_view.bottom + 100,
			hWnd,
			NULL,
			CCore::Instance()->GetHInstance(),
			(LPSTR)&clientcreate);*/
		hwndClient = CreateWindow(
			_T("Child Window"),
			NULL,
			WS_OVERLAPPEDWINDOW,
			0, 0, rect_view.right + 100, rect_view.bottom + 200,
			hWnd,
			NULL,
			CCore::Instance()->GetHInstance(),
			NULL);

		ShowWindow(hwndClient, SW_SHOW);

		break;
		// WS_OVERLAPPEDWINDOW
		// WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILENEW:
			//mdicreate.szClass = _T("Child Window");
			//mdicreate.szTitle = _T("Child Window");
			//mdicreate.hOwner = CCore::Instance()->GetHInstance();
			//mdicreate.x = 0;
			//mdicreate.y = 0;
			//mdicreate.cx = rect_view.right;
			//mdicreate.cy = rect_view.bottom; //  (int)tmp_rect.y;
			//mdicreate.style = 0;
			//mdicreate.lParam = 0;
			//hwndChild = (HWND)SendMessage(hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			UpdateWindow(hWnd);
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
	static HWND tmp_hwnd = NULL;

	static HWND h_tile_setting = NULL;

	int dx = 1;
	float size_coef = 0.35;
	float size_x = 0;
	float size_y = 0;

	// 파일 I/O 정보
	OPENFILENAME OFN;
	OPENFILENAME SFN;
	TCHAR str[100], lp_str_file[100] = _T("");
	TCHAR filter[] = _T("Every File(*.*) \0*.*\0Text File\0*.txt;*.doc\0");

	switch (message)
	{
	case WM_CREATE:
		// 윈도우 핸들 Init
		// CSceneManager::Instance()->LoadHwnd(MAP_EDIT_SCENE, hWnd);

		GetClientRect(hWnd, &rect_view);

		// Child Window를 두개 영역으로 나눈다.
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

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_TILESET_SELECTION:
			DialogBoxW(CCore::Instance()->GetHInstance(), MAKEINTRESOURCE(IDD_DIALOG_TSS), hWnd, Dlg_TSS_Proc);
			break;
		case ID_LAYER_SELECTION:
			DialogBoxW(CCore::Instance()->GetHInstance(), MAKEINTRESOURCE(IDD_DIALOG_LAYER), hWnd, Dlg_Layer_Proc);
			break;
		case ID_TILESET_SETTING:
			// 타일 세팅 윈도우 생성
			h_tile_setting = CreateWindow(
				_T("TileSetting Window"),
				NULL,
				WS_OVERLAPPEDWINDOW,
				0, 0, rect_view.right / 2, rect_view.bottom,
				hWnd,
				NULL,
				CCore::Instance()->GetHInstance(),
				NULL);

			ShowWindow(h_tile_setting, SW_SHOW);
			UpdateWindow(h_tile_setting);
			break;
			
		case ID_SAVE_MAP:
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hWnd;
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = lp_str_file;
			SFN.nMaxFile = 100;
			SFN.lpstrInitialDir = _T(".");
			if (GetSaveFileName(&SFN) != 0)
			{
				_stprintf_s(str, _T("%s 파일을 저장하시겠습니까?"), SFN.lpstrFile);
				MessageBox(hWnd, str, _T("저장하기 선택"), MB_OK);
				
				// 문자열 데이터 형변환
				wstring tmp_wstr = lp_str_file;
				string tmp_file_path = "";
				tmp_file_path.assign(tmp_wstr.begin(), tmp_wstr.end());

				// 데이터 저장
				CSceneManager::Instance()->SaveFromPath(tmp_file_path.c_str());
			}
			break;

		case ID_LOAD_MAP:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = filter;
			OFN.lpstrFile = lp_str_file;
			OFN.nMaxFile = 100;
			OFN.lpstrInitialDir = _T(".");
			if (GetOpenFileName(&OFN) != 0)
			{
				_stprintf_s(str, _T("%s 파일을 열겠습니까?"), OFN.lpstrFile);
				MessageBox(hWnd, str, _T("열기 선택"), MB_OK);

				// 문자열 데이터 형변환
				wstring tmp_wstr = lp_str_file;
				string tmp_file_path = "";
				tmp_file_path.assign(tmp_wstr.begin(), tmp_wstr.end());

				// 데이터 업로드
				CSceneManager::Instance()->LoadFromPath(tmp_file_path.c_str());

			}
			break;
		}
		break;

	case WM_KEYDOWN:
		if (wParam == 0x20)
		{
			// 스페이스 키 누를 시, 맵을 인자에 대입된 특성의 타일셋으로 칠한다.
			CMapEditScene* pt_map_edit_scene = static_cast<CMapEditScene*>(CSceneManager::Instance()->pt_map_edit_scene_);
			pt_map_edit_scene->PaintMap(pt_map_edit_scene->GetRectTileVec(), pt_map_edit_scene->GetRectNumX(), pt_map_edit_scene->GetRectNumY(),
											pt_map_edit_scene->GetEditLayer(), pt_map_edit_scene->GetWorldSize());
		}
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
		return 0;

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

	case WM_KEYDOWN:
		cout << "MapEditProc WM_KEYDONW!\n";
		if (CInputManager::Instance()->GetKey1())
			cout << "MapEditProc key 1 눌림!\n";

		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// 더블 버퍼링 준비
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hDoubleBufferBitmap = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferBitmap);

		// 월드 사이즈 만큼만 더블버퍼링 도화지 만들자
		MY_POSE world_size = CSceneManager::Instance()->pt_map_edit_scene_->GetWorldSize();
		Rectangle(hMemDC, 0, 0, world_size.x, world_size.y);


		// 컨텐츠 그리기
		if (CSceneManager::Instance()->pt_map_edit_scene_)
			CSceneManager::Instance()->pt_map_edit_scene_->Render(hMemDC, 0);
		BitBlt(hdc, 0, 0, world_size.x, world_size.y, hMemDC, 0, 0, SRCCOPY);


		// DC 삭제
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
		DeleteObject(hDoubleBufferBitmap);
		DeleteObject(hOldBitmap);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_MOUSEMOVE:
		CInputManager::Instance()->SetHwnd(hWnd);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 124);
		return 0;

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

		// 씬 생성
		CSceneManager::Instance()->CreateScene<CAssistScene>(SC_ASSIST, hWnd);

		GetClientRect(hWnd, &rectView);

		SetTimer(hWnd, 123, 100, NULL);

		break;
	case WM_SIZE:
		GetClientRect(hWnd, &rectView);
		break;
	case WM_TIMER:
		if (wParam == PAINT_AGAIN && lParam == PAINT_AGAIN)
		{
			InvalidateRect(hWnd, NULL, true);
			break;
		}
		else
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

		// 월드 사이즈 만큼만 더블버퍼링 도화지 만들자
		MY_POSE world_size = CSceneManager::Instance()->pt_assist_scene_->GetWorldSize();
		Rectangle(hMemDC, 0, 0, world_size.x, world_size.y);


		// 컨텐츠 그리기
		if (CSceneManager::Instance()->pt_assist_scene_)
			CSceneManager::Instance()->pt_assist_scene_->Render(hMemDC, 0);
		BitBlt(hdc, 0, 0, world_size.x, world_size.y, hMemDC, 0, 0, SRCCOPY);


		// DC 삭제
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
		DeleteObject(hDoubleBufferBitmap);
		DeleteObject(hOldBitmap);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_MOUSEMOVE:
		CInputManager::Instance()->SetHwnd(hWnd);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 123);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR  CALLBACK CCore::Dlg_TSS_Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 콤보박스 핸들
	static HWND hCombo;

	// 콤보박스에서 선택된 문자열 위치
	static int selection = -1;

	// 콤박스로부터 가져올 문자열 저장
	TCHAR name[256] = { 0, };
	char ch_name[256] = { 0, };

	// AssistScene 내 타일 셋 바꿀 때 사용
	CAssistScene* pt_assist = NULL;

	// ASSIST_SCENE 핸들 저장
	HWND tmp_hwnd;

	switch (message)
	{
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hwnd, IDC_TSS_COMBO);

		// 콤보박스 켜지자마자 문자열 업로드
		CSourceManager::Instance()->LoadTextureCombo(hCombo);
		return 1;
		
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_TSS_OK:
			if (selection == -1)
			{
				EndDialog(hwnd, 0);
				break;
			}
			// 콤보박스 내 selection 위치에서 문자열 가져오기
			ComboBox_GetLBText(hCombo, selection, name);
			WideCharToMultiByte(CP_ACP, 0, name, 256, ch_name, 256, NULL, NULL);

			// 가져온 문자열에 해당하는 타일셋으로 Assiscene 타일셋 변경
			pt_assist = static_cast<CAssistScene*>(CSceneManager::Instance()->pt_assist_scene_);
			pt_assist->ChangeBackTileSheet(CSceneManager::Instance()->GetHwnd(ASSIST_SCENE), string(ch_name));

			tmp_hwnd = CSceneManager::Instance()->GetHwnd(ASSIST_SCENE);
			SendMessage(tmp_hwnd, WM_TIMER, PAINT_AGAIN, PAINT_AGAIN);

			EndDialog(hwnd, 0);
			break;

		case IDC_TSS_COMBO:
			if (HIWORD(wParam) == CBN_SELCHANGE)
				selection = (int)SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			break;

		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
	}
	return 0;
}


INT_PTR  CALLBACK CCore::Dlg_Layer_Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 콤보박스 핸들
	static HWND hCombo;

	// 콤보박스에서 선택된 문자열 위치
	static string selection = NONE_LAYER;

	// AssistScene 내 타일 셋 바꿀 때 사용
	CAssistScene* pt_assist = NULL;

	// MapEdidScene 포인터
	static CMapEditScene* pt_map_edit_scene;

	switch (message)
	{
	case WM_INITDIALOG:
		// floor ~ object 레이어까지 radio 버튼 등록. default = floor
		CheckRadioButton(hwnd, IDR_LAYER_FLOOR, IDR_LAYER_OBJECT, IDR_LAYER_FLOOR);
		selection = FLOOR_LAYER;

		// MapEdidScene 초기화
		pt_map_edit_scene = static_cast<CMapEditScene*>(CSceneManager::Instance()->pt_map_edit_scene_);

		return 1;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDBTN_LAYER_OK :
			// cout << selection.c_str() << " 선택!\n";
			pt_map_edit_scene->SetEditLayer(selection);
			EndDialog(hwnd, 0);
			break;

		case IDR_LAYER_FLOOR:
			selection = FLOOR_LAYER;
			break;

		case IDR_LAYER_OBJECT:
			selection = MAP_OBJ_LAYER;
			break;

		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
	}
	return 0;
}

#define IDC_BUTTON 9999
LRESULT CCore::TileSettingProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 창 크기
	static RECT	rect_view;
	
	// 버튼 핸들
	static HWND h_button = NULL;

	// 버튼 설정
	int btn_w;
	int btn_h;
			 ;
	int btn_x;
	int btn_y;

	// 타일 세팅 안쪽 윈도우 핸들
	static HWND h_inner_tile_setting = NULL;

	int inner_w;
	int inner_h;
	int inner_x;
	int inner_y;

	switch (message)
	{
	case WM_CREATE:
		// 윈도우 핸들 Init
		GetClientRect(hWnd, &rect_view);
		CSceneManager::Instance()->LoadHwnd(TILESET_SETTING_SCENE, hWnd);

		// : >>  버튼 생성

		// 버튼 크기 및 위치
		btn_w = 100;
		btn_h = 25;

		btn_x = rect_view.right * 0.5 - btn_w * 0.5;
		btn_y = rect_view.bottom - rect_view.bottom / 16 - btn_h * 0.5;

		// 버튼 윈도우 생성
		h_button = CreateWindow(
			_T("button"),
			_T("확인"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			btn_x,
			btn_y,
			btn_w,
			btn_h,
			hWnd,
			(HMENU)IDC_BUTTON,
			CCore::Instance()->GetHInstance(),
			NULL
		);
		// <<

		// : >> 안쪽 윈도우 생성

		// 안쪽 윈도우 크기 및 위치
		inner_x = rect_view.right * 0.1;
		inner_y = 2 * btn_h;
		inner_w = rect_view.right - 2 * inner_x;
		inner_h = rect_view.bottom - 2 * inner_y;
		

		// 타일 세팅 윈도우 안쪽 윈도우 생성
		h_inner_tile_setting = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			_T("TileSettingInner Window"),
			NULL,
			WS_CHILD | WS_VISIBLE,
			inner_x, inner_y, inner_w, inner_h,
			hWnd,
			NULL,
			CCore::Instance()->GetHInstance(),
			NULL);
		//  | WS_VSCROLL | WS_HSCROLL

		// <<

		// ShowWindow(h_inner_tile_setting, SW_SHOW);
		// UpdateWindow(h_inner_tile_setting);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:
			return 0;

		default:
			break;
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// 텍스트 위치
		int text_x = rect_view.right / 3 + 20;
		int text_y = rect_view.bottom / 32;

		TextOut(hdc, text_x, text_y, _T("타일셋 데이터 설정"), 10);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CCore::TileSettingInnerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 창 크기
	static RECT	rect_view;

	// 세팅 씬 포인터
	// CTileSetSettingScene* pt_setting;

	// 타일 저장한 벡터
	// vector<CTile*> vec_tile;

	// 타일 저장한 오브젝트(stage)
	// CStage* pt_stage;
	
	// 타일 포인터
	// CTile* pt_tile;

	switch (message)
	{
	case WM_CREATE:
		// 윈도우 핸들 Init
		GetClientRect(hWnd, &rect_view);
		CSceneManager::Instance()->LoadHwnd(TILESET_SETTING_INNER_SCENE, hWnd);

		// 타이머 설정
		SetTimer(hWnd, 200, 200, NULL);

		// 씬 초기화
		CSceneManager::Instance()->CreateScene<CTileSetSettingScene>(SC_TILESET_SETTING, hWnd);
		
		// 세팅 씬 불러오기
		// pt_setting = static_cast<CTileSetSettingScene*>(CSceneManager::Instance()->pt_tileset_setting_scene_);

		// 스테이지 오브젝트 초기화
		// pt_stage->AddTiles(MY_POSE(0, 0), pt_setting->GetWorldSize().x / TEXTURE_SIZE, pt_setting->GetWorldSize().y / TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_SIZE,
		// 					EMPTY_GRAY_32, TEXTURE_PATH, true);

		// SAFE_RELEASE(pt_stage);

		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILENEW:
			break;
		}
		break;

	case WM_MOUSEMOVE:
		CInputManager::Instance()->SetHwnd(hWnd);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// 더블 버퍼링 준비
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hDoubleBufferBitmap = CreateCompatibleBitmap(hdc, rect_view.right, rect_view.bottom);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferBitmap);
		
		// 월드 사이즈 만큼만 더블버퍼링 도화지 만들자
		MY_POSE world_size = CSceneManager::Instance()->pt_tileset_setting_scene_->GetWorldSize();
		Rectangle(hMemDC, 0, 0, world_size.x, world_size.y);


		// 컨텐츠 그리기
		if (CSceneManager::Instance()->pt_tileset_setting_scene_)
			CSceneManager::Instance()->pt_tileset_setting_scene_->Render(hMemDC, 0);
		BitBlt(hdc, 0, 0, world_size.x, world_size.y, hMemDC, 0, 0, SRCCOPY);


		// DC 삭제
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
		DeleteObject(hDoubleBufferBitmap);
		DeleteObject(hOldBitmap);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		KillTimer(hWnd, 200);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}