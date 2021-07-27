#pragma once


#include <Windows.h>
#include <iostream>
#include <list>
#include <vector>
#include <tchar.h>
#include "resource.h"

#include <unordered_map>

// 메모리 누수 체크 기능 제공
#include <crtdbg.h>

using namespace std;

#include "Macro.h"
#include "Type.h"
#include "Flag.h"

// use transparentblt
#pragma comment(lib, "msimg32.lib")

// >> : GDI+
#include "MyFunc/MyGdiplus.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

// << :

template <typename T>
void SafeReleaseList(T& obj)
{
	typename T::iterator iter;
	typename T::iterator iter_end = obj.end();
	for (iter = obj.begin(); iter != iter_end; iter++)
	{
		SAFE_RELEASE((*iter));
	}
	obj.clear();
}

template <typename T>
void SafeReleaseMap(T& obj)
{
	typename T::iterator iter;
	typename T::iterator iter_end = obj.end();
	for (iter = obj.begin(); iter != iter_end; iter++)
	{
		SAFE_RELEASE((*iter).second);
	}
	obj.clear();
}

template <typename T>
void SafeDeleteMap(T& obj)
{
	typename T::iterator iter;
	typename T::iterator iter_end = obj.end();
	for (iter = obj.begin(); iter != iter_end; iter++)
	{
		SAFE_DELETE((*iter).second);
	}
	obj.clear();
}