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
	//// define���� �����ϱ⿡�� Ư���ؾ��ϴ� �μ��� Ŭ������ ������ �ΰ��� �־���. ���� ���ø� �Լ��� ����
	void SafeDeleteList(list<class CLayer*>& obj);
	static bool LayerZComp(CLayer* p1, CLayer* p2);

};

#endif


