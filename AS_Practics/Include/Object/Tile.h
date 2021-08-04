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
	TILE_OPTION option_;

public:
	void SetIdxInTexture(float _x, float _y) { idx_int_texture_ = MY_POSE{ _x, _y }; }
	void SetOption(const TILE_OPTION& _op)	{ option_ = _op; }

	TILE_OPTION GetOption() const	{ return option_; }
	MY_POSE		GetIdxInTexture() const { return idx_int_texture_; }


	// bool SetTileTexture(int _srcx, int _srcy, int _srcwidth, int _srcheight, const string& _texture_key, const wchar_t* _pFileName = NULL, const string& _str_path_key = TEXTURE_PATH, const Color& _color_key = Color(255, 0, 255));

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

	CTile* clone();
};

#endif