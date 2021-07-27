#pragma once

#ifndef TYPE_H_
#define TYPE_H_

typedef struct _tagResolution
{
	unsigned int w;
	unsigned int h;
}MY_RESOLUTION, *PT_MY_RESOLUTION;

typedef struct _POSITION
{
	float x, y;

	_POSITION()
	{
		x = 0.f;
		y = 0.f;
	}

	_POSITION(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	_POSITION(int _x, int _y)
	{
		x = (float)_x;
		y = (float)_y;
	}

	_POSITION(const _POSITION& p)
	{
		x = p.x;
		y = p.y;
	}

	_POSITION(const POINT& p)
	{
		x = (float)p.x;
		y = (float)p.y;
	}

	void operator=(const _POSITION& p)
	{
		x = p.x;
		y = p.y;
	}

	void operator=(const POINT &p)
	{
		x = (float)p.x;
		y = (float)p.y;
	}


	//---------------------------------------------- µ¡¼À
	_POSITION operator+(const _POSITION& p)	const
	{
		_POSITION pos;
		pos.x = x + p.x;
		pos.y = y + p.y;
		return pos;
	}

	void operator+=(const _POSITION& p)
	{
		x = x + p.x;
		y = y + p.y;
	}

	_POSITION operator+(const POINT &p)	const
	{
		_POSITION pos;
		pos.x = x + (float)p.x;
		pos.y = y + (float)p.y;
		return pos;
	}

	void operator+=(const POINT& p)
	{
		x = x + p.x;
		y = y + p.y;
	}

	_POSITION operator+(int num)	const
	{
		_POSITION pos;
		pos.x = x + (float)num;
		pos.y = y + (float)num;
		return pos;
	}

	_POSITION operator+(float num)	const
	{
		_POSITION pos;
		pos.x = x + num;
		pos.y = y + num;
		return pos;
	}

	void operator+=(int num)
	{
		x = x + num;
		y = y + num;
	}

	void operator+=(float num)
	{
		x = x + num;
		y = y + num;
	}

	// ------------------------------------------ »©±â
	_POSITION operator-(const _POSITION& p)	const
	{
		_POSITION pos;
		pos.x = x - p.x;
		pos.y = y - p.y;
		return pos;
	}

	void operator-=(const _POSITION& p)
	{
		x = x - p.x;
		y = y - p.y;
	}

	_POSITION operator-(const POINT &p)	const
	{
		_POSITION pos;
		pos.x = x - (float)p.x;
		pos.y = y - (float)p.y;
		return pos;
	}

	void operator-=(const POINT& p)
	{
		x = x - p.x;
		y = y - p.y;
	}

	_POSITION operator-(int num)	const
	{
		_POSITION pos;
		pos.x = x - (float)num;
		pos.y = y - (float)num;
		return pos;
	}

	_POSITION operator-(float num)	const
	{
		_POSITION pos;
		pos.x = x - num;
		pos.y = y - num;
		return pos;
	}

	void operator-=(int num)
	{
		x = x - num;
		y = y - num;
	}

	void operator-=(float num)
	{
		x = x - num;
		y = y - num;
	}

	// ---------------------------------------- °ö¼À
	_POSITION operator*(const _POSITION& p)	const
	{
		_POSITION pos;
		pos.x = x * p.x;
		pos.y = y * p.y;
		return pos;
	}

	_POSITION operator*(const POINT &p)	const
	{
		_POSITION pos;
		pos.x = x * (float)p.x;
		pos.y = y * (float)p.y;
		return pos;
	}

	_POSITION operator*(int num)	const
	{
		_POSITION pos;
		pos.x = x * (float)num;
		pos.y = y * (float)num;
		return pos;
	}

	_POSITION operator*(float num)	const
	{
		_POSITION pos;
		pos.x = x * num;
		pos.y = y * num;
		return pos;
	}
}MY_POSE, *MY_PT_POSE, MY_SIZE, *MY_PT_SIZE;

#endif