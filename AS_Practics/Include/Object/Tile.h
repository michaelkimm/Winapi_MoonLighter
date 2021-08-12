#pragma once
#ifndef CTILE_H_
#define CTILE_H_

#include "StaticObj.h"
#include "..\Game.h"

class CTile :
	public CStaticObj
{
private:
	friend class CObject;
	friend class CLayer;

private:
	MY_POSE idx_int_texture_;
	// TILE_OPTION option_;

public:
	void	SetIdxInTexture(float _x, float _y) { idx_int_texture_ = MY_POSE{ _x, _y }; }
	MY_POSE	GetIdxInTexture() const { return idx_int_texture_; }

	CTile* Clone();

private:
	CTile();
	CTile(const CTile& _tile);
	~CTile();

public:
	virtual bool Init();
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