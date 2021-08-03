#pragma once
#ifndef CASSISTSCENE_H_
#define CASSISTSCENE_H_

#include "MapToolScene.h"

class CAssistScene :
	public CMapToolScene
{
private:
	friend class CSceneManager;

private:
	vector<class CTile*> rect_tile_vec_;

	void MakeCTileVec(MY_POSE _start_pose, int _num_x, int _num_y);

private:
	CAssistScene();
	~CAssistScene();

public:
	virtual bool Init(HWND _hWnd);
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

public:
};

#endif