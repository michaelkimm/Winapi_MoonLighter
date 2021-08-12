#include "StaticObj.h"

CStaticObj::CStaticObj()
{
}

CStaticObj::CStaticObj(const CStaticObj & obj)	:
	CObject(obj)
{
	*this = obj;
}

CStaticObj::~CStaticObj()
{
}

bool CStaticObj::Init()
{
	return true;
}

void CStaticObj::Input(float _time)
{
	CObject::Input(_time);
}

void CStaticObj::Update(float _time)
{
	CObject::Update(_time);
}

void CStaticObj::LateUpdate(float _time)
{
	CObject::LateUpdate(_time);
}

void CStaticObj::Collision(float _time)
{
	CObject::Collision(_time);
}

void CStaticObj::Render(HDC _hdc, float _time)
{
	CObject::Render(_hdc, _time);
}

void CStaticObj::Save(FILE * _pt_file)
{
	CObject::Save(_pt_file);
}

void CStaticObj::Load(FILE * _pt_file)
{
	CObject::Load(_pt_file);
}
