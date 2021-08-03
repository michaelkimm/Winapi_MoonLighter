#pragma once

#ifndef CSCENE_H_
#define CSCENE_H_

#include "..\Game.h"
#include "Layer.h"
#include "..\Core\Camera.h"

class CScene
{
protected:
	friend class CSceneManager;
	
protected:
	CScene();
	virtual ~CScene() = 0;

protected:
	list<class CLayer*> layer_list_;

public:
	CCamera* camera_;

protected:
	MY_POSE pose_;			// 절대 좌표
	MY_SIZE wnd_size_;		// 윈도우 크기
	MY_SIZE world_size_;	// 텍스처 크기

	HWND hWnd_;

public:
	void SetPose(MY_POSE& _pose)			{ pose_ = _pose; }
	void SetPose(float _x, float _y)		{ pose_ = MY_POSE(_x, _y); }
	void SetWndSize(MY_SIZE& _size)			{ wnd_size_ = _size; }
	void SetWndSize(float _x, float _y)		{ wnd_size_ = MY_SIZE(_x, _y); }
	void SetWorldSize(MY_SIZE& _size)		{ world_size_ = _size; }
	void SetWorldSize(float _x, float _y)	{ world_size_ = MY_SIZE(_x, _y); }

	MY_POSE GetPose() const			{ return pose_; }
	MY_SIZE GetWndSize() const		{ return wnd_size_; }
	MY_SIZE GetWorldSize() const	{ return world_size_; }
	HWND	GetHwnd() const			{ return hWnd_; }

public:
	virtual bool Init(HWND _hWnd) = 0;
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

	class CLayer* CreateLayer(const string& _str_tag, int _z_order = 0);
	class CLayer* FindLayer(const string& _str_tag);
	//// define으로 정의하기에는 특정해야하는 인수가 클래스와 포인터 두개가 있었다. 따라서 템플릿 함수로 선언
	void SafeDeleteList(list<class CLayer*>& obj);
	static bool LayerZComp(CLayer* p1, CLayer* p2);

};

#endif


