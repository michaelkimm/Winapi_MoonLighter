#pragma once
#ifndef CMAPEDITSCENE_H_
#define CMAPEDITSCENE_H_

#include "MapToolScene.h"

class CMapEditScene :
	public CMapToolScene
{
	friend class CSceneManager;
	friend class CAssistScene;
	friend class CCore;

	// ���콺 �簢�� ����
	static int rect_num_x_;
	static int rect_num_y_;
	static vector<class CTile*> rect_tile_vec_;
	static void AddTile(class CTile* _t);
	static void ClearTile();
	
public:
	void PaintTiles();
	void PaintAllTile();

	// edit�� ���̾� ����. ���̾�α� �ڽ����� �����Ѵ�.
	// �ܺ� �����̱� ������ ���� �ۺ�
	static string edit_layer_;

private:
	CMapEditScene();
	~CMapEditScene();

public:
	virtual bool Init(HWND _hWnd);
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

};

#endif