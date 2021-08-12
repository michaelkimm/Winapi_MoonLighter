#pragma once

#ifndef CINGAMESCENE_H_
#define CINGAMESCENE_H_

#include "Scene.h"

class CInGameScene : 
	public  CScene
{
	// ** Load할 변수 없음 ** //

	// 씬 매니저는 생성 & 소멸자를 쓸 수 있어야함
	friend class CSceneManager;

private:
	// 씬 매니저를 통해서만 생성하기 때문에 private
	CInGameScene();
	~CInGameScene();

public:
	virtual bool Init(HWND _hWnd);
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

public:
	virtual void Save(FILE* _pt_file);
	virtual void Load(FILE* _pt_file);
};

#endif