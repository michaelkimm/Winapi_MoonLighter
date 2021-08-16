#pragma once
#ifndef CMOVEOBJ_H_
#define CMOVEOBJ_H_

#include "Object.h"

class CMoveObj
	: public CObject
{
protected:
	CMoveObj();
	CMoveObj(const CMoveObj& obj);
	virtual ~CMoveObj();

public:
	float angle_;
	float speed_;

public:
	void SetAngle(float _angle) { angle_ = _angle; }
	void SetSpeed(float _speed) { speed_ = _speed; }
	float GetAngle()			{ return angle_; }
	float GetSpeed()			{ return speed_; }

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
	void MoveAngle(float _speed);
	void MoveAngle(float _speed, float _dt);

public:
	virtual bool Init() = 0;
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

	virtual CMoveObj* Clone() = 0;

public:
	virtual void Save(FILE* _pt_file);
	virtual void Load(FILE* _pt_file);
};

#endif

