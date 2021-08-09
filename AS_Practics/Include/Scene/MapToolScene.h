#pragma once
#ifndef CMAPTOOLSCENE_H_
#define CMAPTOOLSCENE_H_

#include "Scene.h"
class CMapToolScene :
	public CScene
{
	friend class CSceneManager;

protected:
	CMapToolScene();
	virtual ~CMapToolScene() = 0;

protected:
	MY_POSE pose_;	// 절대 좌표
	MY_SIZE size_;

	// 마우스 상태
	MY_SIZE mouse_down_pose_;
	bool past_mouse_down_;
	bool mouse_up_;

public:
	void SetPose(MY_POSE& _pose) { pose_ = _pose; }
	void SetPose(float _x, float _y) { pose_ = MY_POSE(_x, _y); }
	void SetSize(MY_SIZE& _size) { size_ = _size; }
	void SetSize(float _x, float _y) { size_ = MY_SIZE(_x, _y); }

	MY_POSE GetPose() const { return pose_; }
	MY_SIZE GetSize() const { return size_; }


protected:
	// 타일셋 백그라운드 레이어의 텍스처 이름
	string texture_tag_;

	// edit할 레이어 지정. 다이얼로그 박스에서 지정한다.
	// 외부 지정이기 때문에 전역 퍼블릭
	static string edit_layer_;

public:
	// 타일셋 백그라운드 레이어의 타일셋 시트를 바꾼다
	bool ChangeBackTileSheet(HWND _hwnd, const string& _str_key);
	string GetTextureTag() const { return texture_tag_; }

	

public:
	virtual bool Init(HWND _hWnd);
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif