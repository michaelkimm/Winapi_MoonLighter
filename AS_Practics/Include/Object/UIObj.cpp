#include "UIObj.h"

CUIObj::CUIObj()
{
}

CUIObj::CUIObj(const CUIObj& obj) :
	CStaticObj(obj)
{
	*this = obj;
}

CUIObj::~CUIObj()
{
	SafeReleaseList(tile_vec_);
}

bool CUIObj::Init()
{
	return true;
}

void CUIObj::Input(float _time)
{
	CStaticObj::Input(_time);
}

void CUIObj::Update(float _time)
{
	CStaticObj::Update(_time);
}

void CUIObj::LateUpdate(float _time)
{
	CStaticObj::LateUpdate(_time);
}

void CUIObj::Collision(float _time)
{
	CStaticObj::Collision(_time);
}

void CUIObj::Render(HDC _hdc, float _time)
{
	CStaticObj::Render(_hdc, _time);
}
