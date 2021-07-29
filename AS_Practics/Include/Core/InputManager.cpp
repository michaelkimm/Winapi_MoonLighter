#include "InputManager.h"

DEFINE_SINGLETON(CInputManager)

CInputManager::CInputManager()
{
}

CInputManager::~CInputManager()
{
}

bool CInputManager::Init()
{
	// 상태 초기화
	key_a_ = false;
	key_w_ = false;
	key_d_ = false;
	key_s_ = false;

	MouseStateReset();

	return true;
}

void CInputManager::KeyBoardInput(float _time)
{
	// 상태 초기화
	key_a_ = false;
	key_w_ = false;
	key_d_ = false;
	key_s_ = false;

	// 상태 업데이트
	key_a_ = (GetKeyState(0x41) & 0x8000);
	key_w_ = (GetKeyState(0x57) & 0x8000);
	key_d_ = (GetKeyState(0x44) & 0x8000);
	key_s_ = (GetKeyState(0x53) & 0x8000);
}

void CInputManager::MouseInput(MOUSE_STATE _state,LPARAM _lParam)
{
	// 상태 업데이트
	switch (_state)
	{
	case LBUTTON_UP:
		mouse_left_up_ = true;
		break;
	case LBUTTON_DOWN:
		mouse_left_down_ = true;
		break;
	case RBUTTON_UP:
		mouse_right_up_ = true;
		break;
	case RBUTTON_DOWN:
		mouse_right_down_ = true;
		break;
	default:
		break;
	}
	mouse_pose_ = MY_POSE(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam));
}

void CInputManager::MouseStateReset()
{
	// 상태 초기화
	mouse_left_down_ = false;
	mouse_left_up_ = false;
	mouse_right_down_ = false;
	mouse_right_up_ = false;
	
	mouse_pose_ = MY_POSE{ 0.f, 0.f };
}

void CInputManager::Update(float _time)
{
}
