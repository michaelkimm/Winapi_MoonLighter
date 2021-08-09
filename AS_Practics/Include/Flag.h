#pragma once
#ifndef FLAG_H_
#define FLAG_H_

// Scene type
enum SCENE_CREATE
{
	SC_CURRENT,
	SC_NEXT,
	SC_MAP_EDIT,
	SC_ASSIST
};

// Direction
enum MOVE_DIR
{
	MD_BACK = -1,
	MD_NONE,
	MD_FRONT
};

// Tile option
enum TILE_OPTION
{
	TILE_NONE,
	TILE_NOMOVE,
	TILE_TRANSPORT
};

// Tile name
enum TILE_NAME
{
	TILE_WATER1,
	TILE_WATER2,
	TILE_SAND1
};

// Mouse option
enum MOUSE_STATE
{
	LBUTTON_DOWN,
	LBUTTON_UP,
	RBUTTON_DOWN,
	RBUTTON_UP,
};

//// Layer type
//enum LAYER_NAME
//{
//	LAYER_NONE,
//	LAYER_FLOOR,
//	LAYER_MAP_OBJECT,
//	LAYER_PLAYER,
//	LAYER_UI
//};

#endif