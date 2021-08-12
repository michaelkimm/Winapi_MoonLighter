#pragma once

#ifndef CSCENEMANAGER_H_
#define CSCENEMANAGER_H_

#include "..\Game.h"
#include "Scene.h"

class CSceneManager
{
	DECLARE_SINGLETON(CSceneManager)

private:
	// 현재 가리키는 씬 저장 공간
	class CScene* pt_scene_;
	class CScene* pt_next_scene_;

public:
	// 보조 화면 저장 공간
	class CScene* pt_map_edit_scene_;
	class CScene* pt_assist_scene_;
	class CScene* pt_tileset_setting_scene_;

private:
	unordered_map<string, HWND> hwnd_map_;

public:
	void LoadHwnd(const string& _scene_name, HWND _hWnd);
	HWND GetHwnd(const string& _scene_name) const;
public:
	bool Init();
	void Input(float _time);
	void Update(float _time);
	void LateUpdate(float _time);
	void Collision(float _time);
	void Render(HDC _hdc, float _time);

public:
	void Save(FILE* _pt_file);
	void Load(FILE* _pt_file);

	void SaveFromPath(const char* _pt_file_name, const string& _str_path_key = DATA_PATH, bool _full_path = true);
	void LoadFromPath(const char* _pt_file_name, const string& _str_path_key = DATA_PATH, bool _full_path = true);

public:
	// 씬 종류에 따라 다르게 생성해야하기 때문에 템플릿으로 생성
	template <typename T>
	T* CreateScene(SCENE_CREATE _sc, HWND _hWnd)
	{
		T* pt_tmp_scene = new T;
		if (!pt_tmp_scene->Init(_hWnd))
		{
			SAFE_DELETE(pt_tmp_scene);
			return NULL;
		}

		switch (_sc)
		{
		case SC_CURRENT:
			SAFE_DELETE(pt_scene_);
			pt_scene_ = pt_tmp_scene;
			break;
		case SC_NEXT:
			SAFE_DELETE(pt_next_scene_);
			pt_next_scene_ = pt_tmp_scene;
			break;
		case SC_MAP_EDIT:
			SAFE_DELETE(pt_map_edit_scene_);
			pt_map_edit_scene_ = pt_tmp_scene;
			break;
		case SC_ASSIST:
			SAFE_DELETE(pt_assist_scene_);
			pt_assist_scene_ = pt_tmp_scene;
			break;
		case SC_TILESET_SETTING:
			SAFE_DELETE(pt_tileset_setting_scene_);
			pt_tileset_setting_scene_ = pt_tmp_scene;
			break;
		}
		return pt_tmp_scene;
	}

};

#endif


