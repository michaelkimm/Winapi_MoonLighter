#pragma once
#ifndef CMAPEDITSCENE_H_
#define CMAPEDITSCENE_H_

#include "Scene.h"

class CMapEditScene :
	public CScene
{
	friend class CSceneManager;

private:
	CMapEditScene();
	~CMapEditScene();

public:
	virtual bool Init();
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif