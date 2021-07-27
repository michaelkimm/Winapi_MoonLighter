#include "Timer.h"

DEFINE_SINGLETON(CTimer)

CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}

bool CTimer::Init()
{
	// CPU의 고해상도(=일정한 클럭 진동수) 타이머로 부터 클럭 수를 구한다.
	QueryPerformanceFrequency(&second_);
	// CPU의 고해상도(=일정한 클럭 진동수) 타이머로 부터 초당 진동수(=초당 클럭 수)를 구한다.
	QueryPerformanceCounter(&time_);

	f_delta_time_ = 0.f;
	f_time_scale_ = 1.0f;
	f_fps_ = 0.f;
	f_fps_time_ = 0.f;
	i_frame_max_ = 60;
	i_frame_ = 0;

	return true;
}

bool CTimer::Update()
{
	// 정수 저장 공간을 64비트까지 지원한다 .QuadPart가 64비트에 정수를 담는다.
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	//한프레임당 몇초 걸리는지 반환한다.
	f_delta_time_ = (time.QuadPart - time_.QuadPart) / (float)second_.QuadPart;

	time_ = time;

	return true;
}
