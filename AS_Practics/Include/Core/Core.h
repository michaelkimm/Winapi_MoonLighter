#pragma once

#ifndef CCORE_H_
#define CCORE_H_

#include "..\Game.h"
class CCore
{
	DECLARE_SINGLETON(CCore)

private:
	static bool		loop_;

private:
	HINSTANCE		hInst_;
	HWND			hWnd_;
	HDC				hdc_;
	MY_RESOLUTION	wnd_rs_;
	WNDCLASSEXW		wcex_;
	
public:
	HINSTANCE		GetHInstance() const	{ return hInst_; }
	HWND			GetHWnd() const			{ return hWnd_; }
	MY_RESOLUTION	GetResolution() const	{ return wnd_rs_; }
	WNDCLASSEXW		GetWcex() const			{ return wcex_; }

public:
	bool	Init(HINSTANCE);
	int		Run();
private:
	void	Logic();
	void	Input(float _time);
	void	Update(float _time);
	void	LateUpdate(float _time);
	void	Collision(float _time);
	void	Render(float _time);

private:
	// 외부에 보여줄 필요 없음
	ATOM MyRegisterClass();
	BOOL Create();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	bool map_edit_mode_;
	MY_POSE pose_;

public:
	void SetMapEditMode(bool _m) { map_edit_mode_ = _m; }
	void SetPose(MY_POSE _p) { pose_ = _p; }
	void SetPose(float _x, float _y) { pose_ = MY_POSE(_x, _y); }

	bool GetMapEditMode() const { return map_edit_mode_; }

private:
	HWND child_hwnd[2];
public:
	void SetChildHwnd(int _idx, HWND _h) { child_hwnd[_idx] = _h; }

private:
	static LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MapEditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK TileSetProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif // !CCORE_H_

