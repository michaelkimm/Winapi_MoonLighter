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
	void PaintTiles(CLayer* _target_layer, MY_POSE _pose, int _x_length, int _y_length);
	void PaintAllTile(const string& _target_layer = "");

private:
	MY_POSE mouse_pose_with_cam_;
	void UpdateMousePoseWithCam();

	

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