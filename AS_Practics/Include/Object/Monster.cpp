	#include "Monster.h"
#include "..\Core\Core.h"

CMonster::CMonster()	:
	dir_(MD_FRONT)
{
	SetPose(800.f, 100.f);
	SetSize(100.f, 100.f);
	SetSpeed(300.f);
}

CMonster::CMonster(const CMonster & obj) :
	CMoveObj(obj)
{
	*this = obj;
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	return true;
}

void CMonster::Update(float _time)
{
	CMoveObj::Update(_time);

	MoveYFromSpeed(_time, dir_);

	if (pose_.y + size_.y >= CCore::Instance()->GetResolution().h)
	{
		pose_.y = CCore::Instance()->GetResolution().h - size_.y;
		dir_ = MD_BACK;
	}

	else if (pose_.y <= 0)
	{
		pose_.y = 0;
		dir_ = MD_FRONT;
	}
}

void CMonster::LateUpdate(float _time)
{
	CMoveObj::LateUpdate(_time);
}

void CMonster::Collision(float _time)
{
	CMoveObj::Collision(_time);
}

void CMonster::Render(HDC _hdc, float _time)
{
	CMoveObj::Render(_hdc, _time);
	Rectangle(_hdc, pose_.x, pose_.y, pose_.x + size_.x, pose_.y + size_.y);
}

void CMonster::Save(FILE * _pt_file)
{
	CMoveObj::Save(_pt_file);

	// 방향 값(enum) 저장
	fwrite(&dir_, 4, 1, _pt_file);
}

void CMonster::Load(FILE * _pt_file)
{
	CMoveObj::Load(_pt_file);
}
