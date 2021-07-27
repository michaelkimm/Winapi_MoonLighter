#pragma once
#ifndef CPATHMANAGER_H_
#define CPATHMANAGER_H_

#include "..\Game.h"

class CPathManager
{
	DECLARE_SINGLETON(CPathManager)

private:
	unordered_map<string, wstring> path_map_;
public:
	bool Init();

	bool CreatePath(const string& _str_key, const wchar_t* _file_name, const string& _root_key = ROOT_PATH);
	const wchar_t* FindPath(const string& _str_key);
};

#endif