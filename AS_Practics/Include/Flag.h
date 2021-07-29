#pragma once
#ifndef FLAG_H_
#define FLAG_H_

// Scene type
enum SCENE_CREATE
{
	SC_CURRENT,
	SC_NEXT
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

// Mouse option
enum MOUSE_STATE
{
	LBUTTON_DOWN,
	LBUTTON_UP,
	RBUTTON_DOWN,
	RBUTTON_UP,
};

#endif