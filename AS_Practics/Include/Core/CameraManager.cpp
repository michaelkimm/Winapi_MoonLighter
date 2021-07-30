#include "CameraManager.h"
#include "InputManager.h"
#include "Core.h"
#include "..\Object\Object.h"

DEFINE_SINGLETON(CCameraManager);

CCameraManager::CCameraManager()	:
	master_(NULL)
{

}
CCameraManager::~CCameraManager()
{
	SAFE_RELEASE(master_)
}

void CCameraManager::SetMaster(CObject * _obj)
{
	SAFE_RELEASE(master_)
	master_ = _obj;
	if (master_)
		_obj->AddRef();
}

void CCameraManager::SetPose(const MY_SIZE & _size)
{
	pose_ = _size;
}

void CCameraManager::SetPose(float x, float y)
{
	pose_.x = x;
	pose_.y = y;
}

void CCameraManager::SetWndSize(const MY_SIZE & _size)
{
	wnd_size_ = _size;
}

void CCameraManager::SetWndSize(float w, float h)
{
	wnd_size_ = MY_SIZE(w, h);
}

void CCameraManager::SetWorldSize(const MY_SIZE & _size)
{
	world_size_ = _size;
}

void CCameraManager::SetWorldSize(float w, float y)
{
	world_size_ = MY_SIZE(w, y);
}

MY_POSE CCameraManager::GetPose() const
{
	return pose_;
}

MY_POSE CCameraManager::GetWndSize() const
{
	return wnd_size_;
}

CObject * CCameraManager::GetMaster() const
{
	master_->AddRef();
	return master_;
}

MY_SIZE CCameraManager::GetWorldSize() const
{
	return world_size_;
}


void CCameraManager::Move(float _x, float _y)
{
	pose_.x += _x;
	pose_.y += _y;
}

void CCameraManager::Move(float _x, float _y, float _dt)
{
	pose_.x += _x * _dt;
	pose_.y += _y * _dt;
}

void CCameraManager::MoveX(float _x)
{
	pose_.x += _x;
}

void CCameraManager::MoveX(float _x, float _dt)
{
	pose_.x += _x * _dt;
}

void CCameraManager::MoveXFromSpeed(float _dt, MOVE_DIR _dir)
{
	pose_.x += speed_ * _dt * _dir;
}

void CCameraManager::MoveY(float _y)
{
	pose_.y += _y;
}

void CCameraManager::MoveY(float _y, float _dt)
{
	pose_.y += _y * _dt;
}

void CCameraManager::MoveYFromSpeed(float _dt, MOVE_DIR _dir)
{
	pose_.y += speed_ * _dt * _dir;
}

void CCameraManager::Move(const MY_POSE & _move)
{
	pose_ += _move;
}

void CCameraManager::Move(const MY_POSE & _move, float _dt)
{
	pose_ += _move * _dt;
}


bool CCameraManager::Init()
{
	pose_.x = 0.f;
	pose_.y = 0.f;
	wnd_size_.x = CCore::Instance()->GetResolution().w;
	wnd_size_.y = CCore::Instance()->GetResolution().h;
	speed_ = 500.f;
	return true;
}

void CCameraManager::Input(float _time)
{
	if (master_ == NULL)
	{
		if (CInputManager::Instance()->GetKeyA())
		{
			MoveXFromSpeed(_time, MD_BACK);
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

void CCameraManager::Update(float _time)
{
	MY_POSE tmp_pose = pose_;

	if (master_ != NULL)
		tmp_pose = master_->GetPose() - (wnd_size_ * 0.5f);
	
	// 카메라가 플레이어를 정중앙에 둘 경우

	// 카메라가 왼쪽에서 더 왼쪽으로x
	if (tmp_pose.x < 0)
	{
		tmp_pose.x = 0;
	}

	// 카메라가 오른쪽에서 더 오른쪽x
	else if (tmp_pose.x > world_size_.x - wnd_size_.x)
	{
		tmp_pose.x = world_size_.x - wnd_size_.x;
	}
	
	// 카메라가 아래쪽 더 아래로x
	if (tmp_pose.y > world_size_.y - wnd_size_.y)
	{
		tmp_pose.y = world_size_.y - wnd_size_.y;
	}

	else // 카메라가 위쪽에서 더 위로x
	if (tmp_pose.y < 0)
	{
		tmp_pose.y = 0;
	}

	pose_ = tmp_pose;
	cout << "카메리 매니져 내 pose_: (" << pose_.x << ", " << pose_.y << ")\n";
}

void CCameraManager::LateUpdate(float _time)
{
}

void CCameraManager::Collision(float _time)
{
}

void CCameraManager::Render(HDC _hdc, float _time)
{
}
