#include "NatureObj.h"

CNatureObj::CNatureObj()
{
}

CNatureObj::CNatureObj(const CNatureObj& obj) :
	CStaticObj(obj)
{
	*this = obj;
}

CNatureObj::~CNatureObj()
{
}

bool CNatureObj::Init()
{
	return true;
}

void CNatureObj::Input(float _time)
{
	CStaticObj::Input(_time);
}

void CNatureObj::Update(float _time)
{
	CStaticObj::Update(_time);
}

void CNatureObj::LateUpdate(float _time)
{
	CStaticObj::LateUpdate(_time);
}

void CNatureObj::Collision(float _time)
{
	CStaticObj::Collision(_time);
}

void CNatureObj::Render(HDC _hdc, float _time)
{
	CStaticObj::Render(_hdc, _time);
}
