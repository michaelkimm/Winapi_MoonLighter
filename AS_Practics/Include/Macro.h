#pragma once

#ifndef MACRO_H_
#define MACRO_H_

#define PRINT_POINT_DATA(d)	cout << "data: (" << d.x << ", " << d.y << endl;

// 사용자 정의 명령어
#define PAINT_AGAIN 8888

#define ROOT_PATH "root_path"
#define TEXTURE_PATH "texture_path"
#define DATA_PATH "data_path"
#define PLAYER_TAG "player"

// : >> Object class tag
#define PLAYER_CLASS "player_class"
#define STAGE_CLASS "stage_class"
#define NATURE_OBJ_CLASS "map_obj_class"
#define UI_OBJ_CLASS "ui_obj_class"
#define TILE_CLASS "tile_class"
// <<

// : >> Scene name
#define MAP_EDIT_SCENE "MapEditScene"
#define INGAME_SCENE "InGameScene"
#define ASSIST_SCENE "AssistScene"
#define TILESET_SETTING_SCENE "TileSetSettingScene"
#define TILESET_SETTING_INNER_SCENE "TileSetSettingInnerScene"
// << 

// : >> Layer name
#define NONE_LAYER "none_layer"
#define FLOOR_LAYER "floor_layer"
#define MAP_OBJ_LAYER "map_obj_layer"
#define UI_LAYER "ui_layer"
#define MOUSE_RECT_LAYER "mouse_rect_layer"
#define PLAYER_LAYER "player_layer"
// <<

// : >> Texture characteristic

// Texture name
#define WATER1 "water1"
#define WATER2 "water2"
#define SAND1 "sand1"
#define EMPTY_WHITE_32 "empty_white_32"
#define EMPTY_BLACK_32 "empty_black_32"
#define EMPTY_BW_32 "empty_bw_32"
#define EMPTY_GRAY_32 "empty_gray_32"
#define O_32 "O_32"
#define X_32 "X_32"
#define T_32 "T_32"
#define OXT_32 "OXT_32"
#define SV_BEACH_SUMMER "SV_Beach_Summer"
#define SV_BEACH_WINTER "SV_Beach_Winter"

// Texture size
#define TEXTURE_SIZE 32
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


