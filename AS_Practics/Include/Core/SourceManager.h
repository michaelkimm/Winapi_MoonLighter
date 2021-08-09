#pragma once
#ifndef CANIMATION_H_
#define CANIMATION_H_

#include "..\Game.h"

class CSourceManager
{
	DECLARE_SINGLETON(CSourceManager)

private:
	unordered_map<string, class CTexture*> source_map_;
	class CTexture* back_buffer_;

	HINSTANCE hInst_;
	HDC hdc_;

public:
	class CTexture* GetBackBuffer() const;

public:
	bool Init(HINSTANCE _hInst, HDC& _hdc);

	// 저장할 텍스처 키, 텍스처 클래스, 텍스처 위치
	class CTexture* LoadTexture(const string& _texture_key, const wchar_t* _file_name, const string& _str_path_key = TEXTURE_PATH,
									const COLORREF& _color_key = RGB(255, 0, 255), bool _not_load_map = false);

	// source_map_에서 _texture_key에 해당하는 텍스처가 있는지 확인
	CTexture* FindTexture(const string& _texture_key);

	// 콤보박스에 텍스처를 로드하라
	void LoadTextureCombo(HWND _hwnd);
};

#endif