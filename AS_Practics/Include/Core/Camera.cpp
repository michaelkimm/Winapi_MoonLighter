#include "Camera.h"

#include "..\Scene\SceneManager.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "Core.h"
#include "..\Object\Object.h"


void CCamera::SetMaster(CObject * _obj)
{
	SAFE_RELEASE(master_)
		master_ = _obj;
	if (master_)
		_obj->AddRef();
}

void CCamera::SetPose(const MY_SIZE & _size)
{
	pose_ = _size;
}

void CCamera::SetPose(float x, float y)
{
	pose_.x = x;
	pose_.y = y;
}

void CCamera::SetWndSize(const MY_SIZE & _size)
{
	wnd_size_ = _size;
}

void CCamera::SetWndSize(float w, float h)
{
	wnd_size_ = MY_SIZE(w, h);
}

void CCamera::SetWorldSize(const MY_SIZE & _size)
{
	world_size_ = _size;
}

void CCamera::SetWorldSize(float w, float y)
{
	world_size_ = MY_SIZE(w, y);
}

void CCamera::SetHwnd(HWND _hWnd)
{
	hWnd_ = _hWnd;

	RECT tmp_rect;

	// HWND 정하면 그에 맞는 월드 사이즈 조정
	GetClientRect(hWnd_, &tmp_rect);
	SetWndSize(tmp_rect.right, tmp_rect.bottom);

}

MY_POSE CCamera::GetPose() const
{
	return pose_;
}

MY_POSE CCamera::GetWndSize() const
{
	return wnd_size_;
}

CObject * CCamera::GetMaster() const
{
	master_->AddRef();
	return master_;
}

MY_SIZE CCamera::GetWorldSize() const
{
	return world_size_;
}

HWND CCamera::GetHwnd() const
{
	return hWnd_;
}


void CCamera::Move(float _x, float _y)
{
	pose_.x += _x;
	pose_.y += _y;
}

void CCamera::Move(float _x, float _y, float _dt)
{
	pose_.x += _x * _dt;
	pose_.y += _y * _dt;
}

void CCamera::MoveX(float _x)
{
	pose_.x += _x;
}

void CCamera::MoveX(float _x, float _dt)
{
	pose_.x += _x * _dt;
}

void CCamera::MoveXFromSpeed(float _dt, MOVE_DIR _dir)
{
	pose_.x += speed_ * _dt * _dir;
}

void CCamera::MoveY(float _y)
{
	pose_.y += _y;
}

void CCamera::MoveY(float _y, float _dt)
{
	pose_.y += _y * _dt;
}

void CCamera::MoveYFromSpeed(float _dt, MOVE_DIR _dir)
{
	pose_.y += speed_ * _dt * _dir;
}

void CCamera::Move(const MY_POSE & _move)
{
	pose_ += _move;
}

void CCamera::Move(const MY_POSE & _move, float _dt)
{
	pose_ += _move * _dt;
}

CCamera::CCamera()	:
	master_(NULL)
{
}

CCamera::~CCamera()
{
	SAFE_RELEASE(master_)
}

bool CCamera::Init(MY_SIZE _pose, MY_SIZE _wnd_size, MY_SIZE _world_size, float _speed)
{
	pose_.x = _pose.x;
	pose_.y = _pose.y;
	wnd_size_.x = _wnd_size.x;
	wnd_size_.y = _wnd_size.y;
	world_size_.x = _world_size.x;
	world_size_.y = _world_size.y;
	speed_ = _speed;
	return true;
}

void CCamera::Input(float _time)
{
	if (master_ == NULL)
	{
		if (CInputManager::Instance()->GetKeyA())
		{
			MoveXFromSpeed(_time, MD_BACK);
			// cout << "씬에서 키 A 눌러짐!\n";
		}
		if (CInputManager::Instance()->GetKeyW())
		{
			MoveYFromSpeed(_time, MD_BACK);
		}
		if (CInputManager::Instance()->GetKeyD())
		{
			MoveXFromSpeed(_time, MD_FRONT);
		}
		if (CInputManager::Instance()->GetKeyS())
		{
			MoveYFromSpeed(_time, MD_FRONT);
		}
	}
}

void CCamera::Update(float _time)
{
	MY_POSE tmp_pose = pose_;

	if (master_ != NULL)
		tmp_pose = master_->GetPose() - (wnd_size_ * 0.5f);

	// : >> 카메라가 플레이어를 정중앙에 둘 경우

	// 카메라가 왼쪽에서 더 왼쪽으로x
	if (tmp_pose.x < 0)
	{
		tmp_pose.x = 0;
	}

	// 카메라가 오른쪽에서 더 오른쪽x
	else if ((world_size_.x > wnd_size_.x) && tmp_pose.x > world_size_.x - wnd_size_.x)
	{
		tmp_pose.x = world_size_.x - wnd_size_.x;
	}

	// 카메라가 아래쪽 더 아래로x
	if ((world_size_.y > wnd_size_.y) && tmp_pose.y > world_size_.y - wnd_size_.y)
	{
		tmp_pose.y = world_size_.y - wnd_size_.y;
	}

	// 카메라가 위쪽에서 더 위로x
	else if (tmp_pose.y < 0)
	{
		tmp_pose.y = 0;
	}

	// <<

	pose_ = tmp_pose;
	// cout << "카메리 내 pose_: (" << pose_.x << ", " << pose_.y << ")\n";
}

void CCamera::LateUpdate(float _time)
{
}

void CCamera::Collision(float _time)
{
}

void CCamera::Render(HDC _hdc, float _time)
{
}
