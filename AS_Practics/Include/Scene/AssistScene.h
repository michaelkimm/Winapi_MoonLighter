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
	// 마우스가 그리는 사각형에 쓰이는 타일 저장
	// vector<class CTile*> rect_tile_vec_;

	// 용도 모름. 우선 유지
	void MakeCTileVec(MY_POSE _start_pose, int _num_x, int _num_y);

public:
	// 타일셋 백그라운드 레이어의 타일셋 시트를 바꾼다
	bool ChangeBackTileSheet(HWND _hwnd, const string& _str_key);

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