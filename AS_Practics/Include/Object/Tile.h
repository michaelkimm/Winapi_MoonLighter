#pragma once
#ifndef CTILE_H_
#define CTILE_H_

#include "StaticObj.h"
#include "..\Game.h"

class CTile :
	public CStaticObj
{
private:
	friend class CObject;
	friend class CStage;

private:
	TILE_OPTION option_;

public:
	void SetOption(const TILE_OPTION& _op)	{ option_ = _op; }

	TILE_OPTION GetOption() const	{ return option_; }

private:
	CTile();
	CTile(const CTile& _tile);
	~CTile();

public:
	virtual bool Init();
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

	CTile* clone();
};

#endif