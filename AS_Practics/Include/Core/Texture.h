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
	int x_;
	int y_;
	int w_;
	int h_;

public:
	Image*			GetImg() const { return img_; }
	// ImageAttributes GetImgAttr() const	{ return imgAttr_; }
	int				GetWidth() const { return w_; }
	int				GetHeight() const { return h_; }
	HDC				GetDC() const { return hMemDC_; }
	HDC hMemDC_;

private:
	HINSTANCE hInst_;
	HBITMAP hBitmap_;
	BITMAP bitmap_;
	HBITMAP hOldBitmap_;
	HBITMAP hDoubleBufferImage_;

	COLORREF color_key;
public:
	void		SetColorKey(COLORREF _color_key)	{ color_key = _color_key; }
	COLORREF	GetColorKey() const					{ return color_key; }

private:
	CTexture();
	~CTexture();	

public:
	


	bool SetTexture(HINSTANCE _hInst, HDC& _hdc, const wchar_t* _file_name, const string& _str_path_key = TEXTURE_PATH,
							const COLORREF& _color_key = RGB(255, 0, 255));
	bool SetTextureImg(const wchar_t* _file_name, const string& _str_path_key = TEXTURE_PATH, const COLORREF& _color_key = RGB(255, 0, 255));
	bool SetImgInfo(Image* _pt_img, int srcx, int srcy, int _srcwidth, int _srcheight, const COLORREF& _color_key = RGB(255, 0, 255));

	void Render(HDC _hdc, float _time);
};


#endif