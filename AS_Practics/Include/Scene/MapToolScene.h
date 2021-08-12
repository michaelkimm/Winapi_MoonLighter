#pragma once
#ifndef CMAPTOOLSCENE_H_
#define CMAPTOOLSCENE_H_

#include "Scene.h"
#include "..\Object\Object.h"
#include "..\Object\Tile.h"

class CMapToolScene :
	public CScene
{
	// ** Load할 변수 없음 ** //

	friend class CSceneManager;

protected:
	CMapToolScene();
	virtual ~CMapToolScene() = 0;

protected:

	// 마우스 상태
	MY_SIZE mouse_down_pose_;
	bool past_mouse_down_;

protected:
	// 타일셋 백그라운드 레이어의 텍스처 이름
	string texture_tag_;

	// edit할 레이어 지정. 다이얼로그 박스에서 지정한다.
	// 외부 지정이기 때문에 전역 퍼블릭
	static string edit_layer_;

public:
	// 타일셋 백그라운드 레이어의 타일셋 시트를 바꾼다
	bool ChangeBackTileSheet(HWND _hwnd, const string& _str_key);
	string GetTextureTag() const { return texture_tag_; }

public:
	// 맵을 (_rect_num_x * _rect_num_y) 사이즈 타일 집합으로 채운다
	bool PaintMap(vector<class CTile*> _vec_tile, int _rect_num_x, int _rect_num_y, const string& _target_layer, MY_SIZE _world_size);

	// 맵을 (_rect_num_x * _rect_num_y) 사이즈 타일 집합으로 채운다
	bool PaintMap2(vector<class CTile*> _vec_tile, int _rect_num_x, int _rect_num_y, const string& _target_layer, MY_SIZE _world_size, const string& _texture_key);

	// 맵의 특정 위치에 타일 집합을 붙여 넣는다.
	void PaintTiles(CLayer* _target_layer, MY_POSE _pose, vector<CTile*> _rect_tile_vec, int _x_length, int _y_length);
	void PaintTiles2(CLayer* _target_layer, MY_POSE _pose, vector<CTile*> _rect_tile_vec, int _x_length, int _y_length, int _x, int _y, const string& _texture_key);

public:
	virtual bool Init(HWND _hWnd);
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

public:
	virtual void Save(FILE* _pt_file);
	virtual void Load(FILE* _pt_file);
};

#endif