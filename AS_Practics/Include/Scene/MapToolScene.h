#pragma once
#ifndef CMAPTOOLSCENE_H_
#define CMAPTOOLSCENE_H_

#include "Scene.h"
#include "..\Object\Object.h"
#include "..\Object\Tile.h"

class CMapToolScene :
	public CScene
{
	// ** Load�� ���� ���� ** //

	friend class CSceneManager;

protected:
	CMapToolScene();
	virtual ~CMapToolScene() = 0;

protected:

	// ���콺 ����
	MY_SIZE mouse_down_pose_;
	bool past_mouse_down_;

protected:
	// Ÿ�ϼ� ��׶��� ���̾��� �ؽ�ó �̸�
	string texture_tag_;

	// edit�� ���̾� ����. ���̾�α� �ڽ����� �����Ѵ�.
	// �ܺ� �����̱� ������ ���� �ۺ�
	static string edit_layer_;

public:
	// Ÿ�ϼ� ��׶��� ���̾��� Ÿ�ϼ� ��Ʈ�� �ٲ۴�
	bool ChangeBackTileSheet(HWND _hwnd, const string& _str_key);
	string GetTextureTag() const { return texture_tag_; }

public:
	// ���� (_rect_num_x * _rect_num_y) ������ Ÿ�� �������� ä���
	bool PaintMap(vector<class CTile*> _vec_tile, int _rect_num_x, int _rect_num_y, const string& _target_layer, MY_SIZE _world_size);

	// ���� (_rect_num_x * _rect_num_y) ������ Ÿ�� �������� ä���
	bool PaintMap2(vector<class CTile*> _vec_tile, int _rect_num_x, int _rect_num_y, const string& _target_layer, MY_SIZE _world_size, const string& _texture_key);

	// ���� Ư�� ��ġ�� Ÿ�� ������ �ٿ� �ִ´�.
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