#pragma once

#ifndef CINGAMESCENE_H_
#define CINGAMESCENE_H_

#include "Scene.h"

class CInGameScene : 
	public  CScene
{
	// ** Load�� ���� ���� ** //

	// �� �Ŵ����� ���� & �Ҹ��ڸ� �� �� �־����
	friend class CSceneManager;

private:
	// �� �Ŵ����� ���ؼ��� �����ϱ� ������ private
	CInGameScene();
	~CInGameScene();

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