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
	MY_POSE pose_;			// ���� ��ǥ
	MY_SIZE wnd_size_;		// ������ ũ��
	MY_SIZE world_size_;	// �ؽ�ó ũ��

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
	//// define���� �����ϱ⿡�� Ư���ؾ��ϴ� �μ��� Ŭ������ ������ �ΰ��� �־���. ���� ���ø� �Լ��� ����
	void SafeDeleteList(list<class CLayer*>& obj);
	static bool LayerZComp(CLayer* p1, CLayer* p2);

};

#endif


