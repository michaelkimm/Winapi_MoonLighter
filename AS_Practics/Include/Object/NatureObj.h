#pragma once
#ifndef CNATUREOBJ_H_
#define CNATUREOBJ_H_

#include "StaticObj.h"
#include "..\Game.h"

class CNatureObj :
	public CStaticObj
{
	friend class CObject;

private:
	CNatureObj();
	CNatureObj(const CNatureObj& obj);
	~CNatureObj();

public:
	virtual bool Init();
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif