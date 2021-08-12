#pragma once
#ifndef CMAPEDITSCENE_H_
#define CMAPEDITSCENE_H_

#include "MapToolScene.h"

class CMapEditScene :
	public CMapToolScene
{
	// ** Load할 변수 없음 ** //

	friend class CSceneManager;
	friend class CAssistScene;
	friend class CCore;

private:
	
	// edit할 레이어 지정. 다이얼로그 박스에서 지정한다.
	string edit_layer_;

	// 마우스 사각형 갯수
	int rect_num_x_;
	int rect_num_y_;
	vector<class CTile*> rect_tile_vec_;

public:
	void SetEditLayer(const string& _str)						{ edit_layer_ = _str; }
	void SetRectNumX(int _x)									{ rect_num_x_ = _x; }
	void SetRectNumY(int _y)									{ rect_num_y_ = _y; }
	void SetRectTileVec(vector<class CTile*> _rect_tile_vec)	{ rect_tile_vec_ = _rect_tile_vec; }

	string					GetEditLayer() const	{ return edit_layer_; }
	int						GetRectNumX() const		{ return rect_num_x_; }
	int						GetRectNumY() const		{ return rect_num_y_; }
	vector<class CTile*>	GetRectTileVec() const	{ return rect_tile_vec_; }

	void AddTile(class CTile* _t);
	void ClearTile();

private:
	// 카메라 스크롤을 고려한 마우스 위치 in 절대좌표
	MY_POSE mouse_pose_with_cam_;
	
	void UpdateMousePoseWithCam();

	// delete위한 변수
	MY_POSE prev_mouse_pose_with_cam_idx_;
	MY_POSE mouse_pose_with_cam_idx_;

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

public:
	virtual void Save(FILE* _pt_file);
	virtual void Load(FILE* _pt_file);

};

#endif