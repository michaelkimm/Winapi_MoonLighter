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

private:
	class CScene* pt_scene_;
	string	str_tag_;
	int		z_order_;
	list<class CObject*> obj_list_;

private:
	CLayer();
	~CLayer();

public:
	void SetScene(class CScene* _scene)	{ pt_scene_ = _scene; }
	void SetTag(const string& _str_tag) { str_tag_ = _str_tag; }
	void SetZOrder(int _z_order)		{ z_order_ = _z_order; }
	
	CScene* GetScene()	{ return pt_scene_; }
	int		GetZOrder()	{ return z_order_; }
	string	GetTag()	{ return str_tag_; }

public:
	void AddObj(class CObject* obj);
	void Clear();

public:
	bool Init();
	void Input(float _time);
	void Update(float _time);
	void LateUpdate(float _time);
	void Collision(float _time);
	void Render(HDC _hdc, float _time);

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
};

#endif