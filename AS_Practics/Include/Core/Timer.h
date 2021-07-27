#pragma once
#ifndef CTIMER_H_
#define CTIMER_H_

#include "..\Game.h"

class CTimer
{
	DECLARE_SINGLETON(CTimer)

private:
	LARGE_INTEGER	second_;
	LARGE_INTEGER	time_;
	float			f_delta_time_;
	float			f_time_scale_;
	float			f_fps_;
	float			f_fps_time_;
	int				i_frame_max_;
	int				i_frame_;

public:
	bool Init();
	bool Update();

public:
	float GetDeltaTime() const	{ return f_delta_time_ * f_time_scale_; }
	float GetFps() const		{ return f_fps_; }
	float GetTimeScale() const	{ return f_time_scale_; }
	void SetTimeScale(float _s) { f_time_scale_ = _s; }
};


#endif
