#pragma once
#ifndef CTEXTURE_H_
#define CTEXTURE_H_

#include "Ref.h"
#include "..\Game.h"

class CTexture :
	public CRef
{
	friend class CSourceManager;
private:
	int w_;
	int h_;

public:
	int				GetWidth() const { return w_; }
	int				GetHeight() const { return h_; }

private:
	HDC hMemDC_;
	HINSTANCE hInst_;
	HBITMAP hBitmap_;
	BITMAP bitmap_;
	HBITMAP hOldBitmap_;
	HBITMAP hDoubleBufferImage_;

	COLORREF color_key;
public:
	void		SetColorKey(COLORREF _color_key)	{ color_key = _color_key; }
	COLORREF	GetColorKey() const					{ return color_key; }
	HDC			GetDC() const { return hMemDC_; }

private:
	CTexture();
	~CTexture();	

public:
	bool SetTexture(HINSTANCE _hInst, HDC& _hdc, const wchar_t* _file_name, const string& _str_path_key = TEXTURE_PATH,
							const COLORREF& _color_key = RGB(255, 0, 255));
	void Render(HDC _hdc, float _time);
};


#endif