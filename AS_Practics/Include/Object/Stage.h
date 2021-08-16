#pragma once
#ifndef CSTAGE_H_
#define CSTAGE_H_

#include "..\Game.h"

#include "StaticObj.h"
#include "..\Core\SourceManager.h"
#include"Tile.h"

class CStage :
	public CStaticObj
{
	friend class CObject;
	
private:
	CStage();
	CStage(const CStage& obj);
	~CStage();

	virtual CStage* Clone();

public:
	virtual bool Init();
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif