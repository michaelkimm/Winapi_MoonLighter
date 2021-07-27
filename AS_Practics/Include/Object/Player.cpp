#include "Player.h"
#include "..\Core\SourceManager.h"

CPlayer::CPlayer()
{
	SetPose(100.f, 100.f);
	SetSize(40.f, 40.f);
	SetSpeed(400.f);

	//SetTexture(PLAYER_TAG, L"sigong.bmp");
}

CPlayer::CPlayer(const CPlayer & obj)	:
	CMoveObj(obj)
{
	*this = obj;
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	return true;
}

void CPlayer::Input(float _time)
{
	CMoveObj::Input(_time);

	if (GetAsyncKeyState('W') & 0x8000)
	{
		MoveYFromSpeed(_time, MD_BACK);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		MoveYFromSpeed(_time, MD_FRONT);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		MoveXFromSpeed(_time, MD_BACK);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		MoveXFromSpeed(_time, MD_FRONT);
	}
}

void CPlayer::Update(float _time)
{
	CMoveObj::Update(_time);
}

void CPlayer::LateUpdate(float _time)
{
	CMoveObj::LateUpdate(_time);
}

void CPlayer::Collision(float _time)
{
	CMoveObj::Collision(_time);
}

void CPlayer::Render(HDC _hdc, float _time)
{
	CMoveObj::Render(_hdc, _time);
	//Rectangle(_hdc, pose_.x, pose_.y, pose_.x + size_.x, pose_.y + size_.y);
	
}
