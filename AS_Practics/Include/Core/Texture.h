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
	Image* img_;
	ImageAttributes imgAttr_;
	int w_;
	int h_;

	HDC hMemDC_;

	HINSTANCE hInst_;
	HBITMAP hBitmap_;
	BITMAP bitmap_;
	HBITMAP hOldBitmap_;

private:
	CTexture();
	~CTexture();	

public:
	Image*			GetImg() const		{ return img_; }
	// ImageAttributes GetImgAttr() const	{ return imgAttr_; }
	int				GetWidth() const	{ return w_; }
	int				GetHeight() const	{ return h_; }
	HDC				GetDC() const		{ return hMemDC_; }


	bool SetTexture(HINSTANCE _hInst, HDC& _hdc, const wchar_t* _file_name);
	void Render(HDC _hdc, float _time);
};


#endif