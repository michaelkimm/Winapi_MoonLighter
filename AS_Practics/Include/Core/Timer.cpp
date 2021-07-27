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
	// CPU�� ���ػ�(=������ Ŭ�� ������) Ÿ�̸ӷ� ���� Ŭ�� ���� ���Ѵ�.
	QueryPerformanceFrequency(&second_);
	// CPU�� ���ػ�(=������ Ŭ�� ������) Ÿ�̸ӷ� ���� �ʴ� ������(=�ʴ� Ŭ�� ��)�� ���Ѵ�.
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
	// ���� ���� ������ 64��Ʈ���� �����Ѵ� .QuadPart�� 64��Ʈ�� ������ ��´�.
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	//�������Ӵ� ���� �ɸ����� ��ȯ�Ѵ�.
	f_delta_time_ = (time.QuadPart - time_.QuadPart) / (float)second_.QuadPart;

	time_ = time;

	return true;
}
