#pragma once
#ifndef CINPUTMANAGER_H_
#define CINPUTMANAGER_H_

#include "..\Game.h"


class CInputManager
{
	DECLARE_SINGLETON(CInputManager)
private:

	HWND hWnd_;

	bool key_a_;
	bool key_w_;
	bool key_d_;
	bool key_s_;

	bool key_1_;
	bool key_2_;
	bool key_3_;

	bool key_del_;
	bool key_space_;
	bool key_ctrl_;

	bool past_mouse_left_down_;
	bool mouse_left_down_;
	bool mouse_left_up_;
	bool mouse_right_down_;
	bool mouse_right_up_;

	MY_POSE mouse_pose_;

public:
	HWND GetHwnd() const { return hWnd_; }
	bool GetKeyA() const { return key_a_; }
	bool GetKeyW() const { return key_w_; }
	bool GetKeyD() const { return key_d_; }
	bool GetKeyS() const { return key_s_; }

	bool GetKey1() const { return key_1_; }
	bool GetKey2() const { return key_2_; }
	bool GetKey3() const { return key_3_; }

	bool GetKeyDel() const { return key_del_; }
	bool GetKeySpace() const { return key_space_; }
	bool GetKeyCtrl() const { return key_ctrl_; }

	bool GetPastMouseLeftDown() const { return past_mouse_left_down_; }
	bool GetMouseLeftDown() const	{ return mouse_left_down_; }
	bool GetMouseLeftUp() const		{ return mouse_left_up_; }
	bool GetMouseRightDown() const	{ return mouse_right_down_; }
	bool GetMouseRightUp() const	{ return mouse_right_up_; }

	MY_POSE GetMousePose() const { return mouse_pose_; }

	void SetHwnd(HWND _hWnd) { hWnd_ = _hWnd; }
	void SetMousePose(LPARAM _lparam);

public:
	bool Init(HWND _hWnd);

	void KeyBoardInput(float _time);
	void KeyBoardStateReset();

	void MouseInput(float _time);
	void MouseInput(MOUSE_STATE _state, LPARAM _lparam);
	void MouseStateReset();

	void Update(float _time);
};

#endif
