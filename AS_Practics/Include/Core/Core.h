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
	
public:
	MY_RESOLUTION GetResolution() const { return wnd_rs_; }

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
};

#endif // !CCORE_H_

