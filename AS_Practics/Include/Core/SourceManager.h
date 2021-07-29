#pragma once
#ifndef CANIMATION_H_
#define CANIMATION_H_

#include "..\Game.h"

class CSourceManager
{
	DECLARE_SINGLETON(CSourceManager)
	
private:
	unordered_map<string, class CTexture*> source_map_;
	HINSTANCE hInst_;
	HDC hdc_;
	class CTexture* back_buffer_;

public :
	class CTexture* GetBackBuffer() const;

public:
	bool Init(HINSTANCE _hInst, HDC& _hdc);

	// ������ �ؽ�ó Ű, �ؽ�ó Ŭ����, �ؽ�ó ��ġ
	class CTexture* LoadTexture(const string& _texture_key, const wchar_t* _file_name, const string& _str_path_key = TEXTURE_PATH,
									const COLORREF& _color_key = RGB(255, 0, 255));
	CTexture* FindTexture(const string& _texture_key);

};

#endif