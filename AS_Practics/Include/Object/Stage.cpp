#include "Stage.h"
#include "..\Core\Texture.h"

CStage::CStage()
{
}

CStage::~CStage()
{
}

bool CStage::Init()
{
	SetPose(0.f, 0.f);
	return true;
}

void CStage::Update(float _time)
{
	CStaticObj::Update(_time);
}

void CStage::LateUpdate(float _time)
{
	CStaticObj::LateUpdate(_time);
}

void CStage::Collision(float _time)
{
	CStaticObj::Collision(_time);
}

void CStage::Render(HDC _hdc, float _time)
{
	CStaticObj::Render(_hdc, _time);
}
