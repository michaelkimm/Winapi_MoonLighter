#pragma once
#ifndef CSTATICOBJ_H_
#define CSTATICOBJ_H_

#include "Object.h"

class CStaticObj
	: public CObject
{
protected:
	CStaticObj();
	CStaticObj(const CStaticObj& obj);
	virtual ~CStaticObj() = 0;


public:
	virtual bool Init() = 0;
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif