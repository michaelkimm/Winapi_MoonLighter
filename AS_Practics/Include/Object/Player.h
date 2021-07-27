
#pragma once
#ifndef CPLAYER_H_
#define CPLAYER_H_

#include "MoveObj.h"

class CPlayer :
	public CMoveObj
{
private:
	friend class CObject;

private:
	CPlayer();
	CPlayer(const CPlayer& obj);
	~CPlayer();

public:
	virtual bool Init();
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif