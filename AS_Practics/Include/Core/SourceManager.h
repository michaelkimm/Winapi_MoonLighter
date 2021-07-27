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

public:
	bool Init(HINSTANCE _hInst, HDC& _hdc);
	// 저장할 텍스처 키, 텍스처 클래스, 텍스처 위치
	class CTexture* LoadTexture(const string& _str_key, const wchar_t* _file_name, const string& _str_path_key);
	CTexture* FindTexture(const string& _str_key);

};

#endif