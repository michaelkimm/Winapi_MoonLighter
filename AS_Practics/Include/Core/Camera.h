#pragma once
#ifndef CCAMERA_H_
#define CCAMERA_H_

#include "..\Game.h"

class CCamera
{
	// friend class CCameraManager;
private:
	MY_POSE pose_;			// 카메라 절대 좌표
	MY_SIZE wnd_size_;
	MY_SIZE world_size_;

	HWND hWnd_;

private:
	class CObject* master_;

public:

	void SetPose(const MY_SIZE& _size);
	void SetPose(float x, float y);
	void SetWndSize(const MY_SIZE& _size);
	void SetWndSize(float w, float h);
	void SetWorldSize(const MY_SIZE& _size);
	void SetWorldSize(float w, float h);
	void SetMaster(class CObject* _obj);
	void SetHwnd(HWND _hWnd);

	MY_POSE GetPose() const;
	MY_POSE GetWndSize() const;
	MY_SIZE GetWorldSize() const;
	CObject* GetMaster() const;
	HWND	GetHwnd() const;

private:
	float speed_;

public:
	void SetSpeed(float _speed) { speed_ = _speed; }
	float GetSpeed() { return speed_; }

public:
	void Move(float _x, float _y);
	void Move(float _x, float _y, float _dt);

	void MoveX(float _x);
	void MoveX(float _x, float _dt);
	void MoveXFromSpeed(float _dt, MOVE_DIR _dir);

	void MoveY(float _y);
	void MoveY(float _y, float _dt);
	void MoveYFromSpeed(float _dt, MOVE_DIR _dir);

	void Move(const MY_POSE& _move);
	void Move(const MY_POSE& _move, float _dt);


public:
	CCamera();
	~CCamera();

public:
	bool Init(MY_SIZE _pose, MY_SIZE _wnd_size, MY_SIZE _world_size, float _speed);
	void Input(float _time);
	void Update(float _time);
	void LateUpdate(float _time);
	void Collision(float _time);
	void Render(HDC _hdc, float _time);
};


#endif
