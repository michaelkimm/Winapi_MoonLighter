#pragma once
#ifndef CCAMERAMANAGER_H_
#define CCAMERAMANAGER_H_

#include "..\Game.h"
#include "..\Object\Object.h"

class CCameraManager
{
	DECLARE_SINGLETON(CCameraManager)
	
private:
	MY_POSE pose_;			// 카메라 절대 좌표
	MY_SIZE wnd_size_;
	MY_SIZE world_size_;

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

	MY_POSE GetPose() const;
	MY_POSE GetWndSize() const;
	MY_SIZE GetWorldSize() const;
	CObject* GetMaster() const;


public:
	bool Init();
	void Input(float _time);
	void Update(float _time);
	void LateUpdate(float _time);
	void Collision(float _time);
	void Render(HDC _hdc, float _time);

};

#endif
