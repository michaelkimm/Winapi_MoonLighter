#pragma once
#ifndef CMONSTER_H_
#define CMONSTER_H_

#include "MoveObj.h"

class CMonster :
	public CMoveObj
{
private:
	friend class CObject;

private:
	MOVE_DIR dir_;

private:
	CMonster();
	CMonster(const CMonster& obj);
	~CMonster();

public:
	virtual bool Init();
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif