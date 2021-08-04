#pragma once
#ifndef CMAPTOOLSCENE_H_
#define CMAPTOOLSCENE_H_

#include "Scene.h"
class CMapToolScene :
	public CScene
{
	friend class CSceneManager;

protected:
	CMapToolScene();
	virtual ~CMapToolScene() = 0;

protected:
	// 마우스 상태
	MY_SIZE mouse_down_pose_;
	bool past_mouse_down_;
	bool mouse_up_;

public:
	virtual bool Init(HWND _hWnd);
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif