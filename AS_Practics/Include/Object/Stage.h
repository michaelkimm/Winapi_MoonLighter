#pragma once
#ifndef CSTAGE_H_
#define CSTAGE_H_

#include "StaticObj.h"
#include "..\Core\SourceManager.h"

class CStage :
	public CStaticObj
{
public:
	CStage();
	~CStage();

	virtual bool Init();
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif