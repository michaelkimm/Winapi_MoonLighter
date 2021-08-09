#pragma once
#ifndef CASSISTSCENE_H_
#define CASSISTSCENE_H_

#include "MapToolScene.h"

class CAssistScene :
	public CMapToolScene
{
private:
	friend class CSceneManager;

private:
	// ���콺�� �׸��� �簢���� ���̴� Ÿ�� ����
	// vector<class CTile*> rect_tile_vec_;

	// �뵵 ��. �켱 ����
	void MakeCTileVec(MY_POSE _start_pose, int _num_x, int _num_y);

public:
	// Ÿ�ϼ� ��׶��� ���̾��� Ÿ�ϼ� ��Ʈ�� �ٲ۴�
	bool ChangeBackTileSheet(HWND _hwnd, const string& _str_key);

private:
	CAssistScene();
	~CAssistScene();

public:
	virtual bool Init(HWND _hWnd);
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

public:
};

#endif