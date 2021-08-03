#pragma once

#ifndef MACRO_H_
#define MACRO_H_

#define PRINT_POINT_DATA(d)	cout << "data: (" << d.x << ", " << d.y << endl;

#define ROOT_PATH "root_path"
#define TEXTURE_PATH "texture_path"
#define PLAYER_TAG "player"

// : >> Scene name
#define MAP_EDIT_SCENE "MapEditScene"
#define INGAME_SCENE "InGameScene"
#define ASSIST_SCENE "AssistScene"
// << 

// : >> Texture name
#define WATER1 "water1"
#define WATER2 "water2"
#define SAND1 "sand1"
#define EMPTY_BLACK_16 "empty_black_16"
// <<

#define SAFE_DELETE(p)			if(p)	{ delete p; p = NULL; }
#define SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = NULL; }
#define SAFE_RELEASE(p)			if(p)	{ p->Release(); p = NULL; }

// : >> singleton
#define DECLARE_SINGLETON(Type)	\
	private:\
		static Type* instance;\
	public:\
		static Type* Instance()\
		{\
			if (instance == nullptr)\
				instance = new Type();\
			return instance;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(instance);\
		}\
	private:\
	Type();\
	~Type();

#define DEFINE_SINGLETON(Type) Type* Type::instance = nullptr;
// <<

#endif


