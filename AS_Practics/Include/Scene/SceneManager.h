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
	bool Init();
	void Input(float _time);
	void Update(float _time);
	void LateUpdate(float _time);
	void Collision(float _time);
	void Render(HDC _hdc, float _time);

public:
	// 씬 종류에 따라 다르게 생성해야하기 때문에 템플릿으로 생성
	template <typename T>
	T* CreateScene(SCENE_CREATE _sc)
	{
		T* pt_tmp_scene = new T;
		if (!pt_tmp_scene->Init())
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
		}

		return pt_tmp_scene;
	}

};

#endif


