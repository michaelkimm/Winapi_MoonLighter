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

	// 마우스 사각형 갯수
	static int rect_num_x_;
	static int rect_num_y_;
	static vector<class CTile*> rect_tile_vec_;
	static void AddTile(class CTile* _t);
	static void ClearTile();
	
public:
	void PaintTiles();
	void PaintAllTile();

	// edit할 레이어 지정. 다이얼로그 박스에서 지정한다.
	// 외부 지정이기 때문에 전역 퍼블릭
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