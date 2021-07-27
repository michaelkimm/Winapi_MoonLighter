#include "MoveObj.h"

CMoveObj::CMoveObj()	:
	angle_(0.f),
	speed_(100.f)
{
}

CMoveObj::CMoveObj(const CMoveObj & obj)	:
	CObject(obj)
{
	*this = obj;
	angle_ = obj.angle_;
}

CMoveObj::~CMoveObj()
{
}

void CMoveObj::Move(float _x, float _y)
{
	pose_.x += _x;
	pose_.y += _y;
}

void CMoveObj::Move(float _x, float _y, float _dt)
{
	pose_.x += _x * _dt;
	pose_.y += _y * _dt;
}

void CMoveObj::MoveX(float _x)
{
	pose_.x += _x;
}

void CMoveObj::MoveX(float _x, float _dt)
{
	pose_.x += _x * _dt;
}

void CMoveObj::MoveXFromSpeed(float _dt, MOVE_DIR _dir)
{
	pose_.x += speed_ * _dt * _dir;
}

void CMoveObj::MoveY(float _y)
{
	pose_.y += _y;
}

void CMoveObj::MoveY(float _y, float _dt)
{
	pose_.y += _y * _dt;
}

void CMoveObj::MoveYFromSpeed(float _dt, MOVE_DIR _dir)
{
	pose_.y += speed_ * _dt * _dir;
}

void CMoveObj::Move(const MY_POSE & _move)
{
	pose_ += _move;
}

void CMoveObj::Move(const MY_POSE & _move, float _dt)
{
	pose_ += _move * _dt;
}

void CMoveObj::MoveAngle(float _speed)
{
	pose_.x += cosf(angle_) * _speed;
	pose_.y += sinf(angle_) * _speed;
}

void CMoveObj::MoveAngle(float _speed, float _dt)
{
	pose_.x += cosf(angle_) * _speed * _dt;
	pose_.y += sinf(angle_) * _speed * _dt;
}

bool CMoveObj::Init()
{
	return true;
}

void CMoveObj::Input(float _time)
{
	CObject::Input(_time);
}

void CMoveObj::Update(float _time)
{
	CObject::Update(_time);
}

void CMoveObj::LateUpdate(float _time)
{
	CObject::LateUpdate(_time);
}

void CMoveObj::Collision(float _time)
{
	CObject::Collision(_time);
}

void CMoveObj::Render(HDC _hdc, float _time)
{
	CObject::Render(_hdc, _time);
}
