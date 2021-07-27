#pragma once

#ifndef MACRO_H_
#define MACRO_H_

#define ROOT_PATH "root_path"
#define TEXTURE_PATH "texture_path"
#define PLAYER_TAG "player"

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

