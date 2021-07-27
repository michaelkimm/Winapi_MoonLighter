#pragma once

#ifndef CSCENE_H_
#define CSCENE_H_

#include "..\Game.h"
#include "Layer.h"

class CScene
{
protected:
	friend class CSceneManager;
	
protected:
	CScene();
	virtual ~CScene() = 0;

protected:
	list<class CLayer*> layer_list_;

public:
	virtual bool Init() = 0;
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

	class CLayer* CreateLayer(const string& _str_tag, int _z_order = 0);
	class CLayer* FindLayer(const string& _str_tag);
	//// define으로 정의하기에는 특정해야하는 인수가 클래스와 포인터 두개가 있었다. 따라서 템플릿 함수로 선언
	void SafeDeleteList(list<class CLayer*>& obj);
	static bool LayerZComp(CLayer* p1, CLayer* p2);

};

#endif


