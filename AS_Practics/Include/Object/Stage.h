#pragma once
#ifndef CSTAGE_H_
#define CSTAGE_H_

#include "..\Game.h"

#include "StaticObj.h"
#include "..\Core\SourceManager.h"
#include"Tile.h"

class CStage :
	public CStaticObj
{
private:
	vector<CTile*> tile_vec_;
	int tile_x_num_;
	int tile_y_num_;
	int tile_width_;
	int tile_height_;

	TILE_NAME tile_edit_name_;

public:
	bool CreateTile(int _num_x, int _num_y, int _size_x, int _size_y, 
						const string& _texture_key, const wchar_t* _file_name, const string& _root_str);
	void SetTileXNum(int _x_num) { tile_x_num_ = _x_num; }
	void SetTileYNum(int _y_num) { tile_y_num_ = _y_num; }
	void SetTileWidth(int _width) { tile_width_ = _width; }
	void SetTileHeight(int _height) { tile_height_ = _height; }

	int GetTileXNum() const { return tile_x_num_; }
	int GetTileYNum() const { return tile_y_num_; }
	int GetTileWidth() const { return tile_width_; }
	int GetTileHeight() const { return tile_height_; }

public:
	CStage();
	~CStage();

	virtual bool Init();
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif