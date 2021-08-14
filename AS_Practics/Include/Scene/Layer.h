#pragma once

#ifndef CLAYER_H_
#define CLAYER_H_

#include "..\Game.h"
#include "Scene.h"

class CLayer
{
private:
	// 씬 클래스에서 CLayer를 생성할 것이기 때문에 프렌드 클래스 생성
	friend class CScene;

	int tile_x_num_;
	int tile_y_num_;
	int tile_width_;
	int tile_height_;

public:
	void SetTileXNum(int _x_num) { tile_x_num_ = _x_num; }
	void SetTileYNum(int _y_num) { tile_y_num_ = _y_num; }
	void SetTileWidth(int _width) { tile_width_ = _width; }
	void SetTileHeight(int _height) { tile_height_ = _height; }

	int GetTileXNum() const { return tile_x_num_; }
	int GetTileYNum() const { return tile_y_num_; }
	int GetTileWidth() const { return tile_width_; }
	int GetTileHeight() const { return tile_height_; }

private:
	class CScene* pt_scene_;
	string	str_tag_;
	int		z_order_;
	vector<class CObject*> obj_list_;

public:
	void SetScene(class CScene* _scene) { pt_scene_ = _scene; }
	void SetTag(const string& _str_tag) { str_tag_ = _str_tag; }
	void SetZOrder(int _z_order) { z_order_ = _z_order; }
	void PushObj(class CObject*);

	CScene* GetScene() { return pt_scene_; }
	int		GetZOrder() { return z_order_; }
	string	GetTag() { return str_tag_; }

	void DeleteObj(float _x, float _y);
	void DeleteAll();
	vector<class CObject*>::iterator FindObj(MY_POSE _pose_idx);

	void		AddObj(class CObject* obj, bool _do_sort = false);
	CObject*	GetObj(int _idx) const;
	CObject*	GetObj(const string& _name_tag);
	int			GetObjCnt() { return obj_list_.size(); }
	void		Clear();
	void		SortObjListZOrder();

private:
	CLayer();
	~CLayer();

public:
	bool Init();
	void Input(float _time);
	void Update(float _time);
	void LateUpdate(float _time);
	void Collision(float _time);
	void Render(HDC _hdc, float _time);

public:
	virtual void Save(FILE* _pt_file);
	virtual void Load(FILE* _pt_file);

public:
	template <typename T>
	T* FindObj(const string& _str_tag)
	{
		typename list<class CObject*>::iterator iter;
		typename list<class CObject*>::iterator iter_end = obj_list_.end();

		for (iter = obj_list_.begin(); iter != iter_end; iter++)
		{
			if ((*iter)->GetTag() == _str_tag)
			{
				(*iter)->AddRef();
				return static_cast<T*>(*iter);
			}
		}
		return NULL;
	}

	// static bool CmpObjY(class CObject* a, class CObject *b);

	bool CreateTile(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
		const string& _texture_key, const string& _root_str, bool _no_tile_only_size = false);
	bool CreateTileSpriteSheet(const MY_POSE& _start_pose, int _size_x, int _size_y,
		const string& _texture_key, const string& _root_str);

	/*bool CreateTile(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
		const string& _texture_key, const wchar_t* _file_name, const string& _root_str);
	bool CreateTileSpriteSheet(const MY_POSE& _start_pose, int _size_x, int _size_y,
		const string& _texture_key, const wchar_t* _file_name, const string& _root_str);*/
};

#endif