#pragma once
#ifndef CUIOBJ_H_
#define CUIOBJ_H_

#include "StaticObj.h"
#include "..\Game.h"


class CUIObj :
	public CStaticObj
{
	friend class CObject;

private:
	CUIObj();
	CUIObj(const CUIObj& obj);
	~CUIObj();

	CUIObj* Clone();

public:
	virtual bool Init();
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif