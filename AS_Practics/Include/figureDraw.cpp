

#include <iostream>
#include <vector>
#include <tchar.h>

#include "figureDraw.h"

#ifndef PI
#define PI 3.141592
#endif // !PI
void DrawGrid(HDC hdc, POINT p1, POINT p2, UINT n)	// ���� ��ġ, �� ��ġ, ���� ����
{
	LONG dx = (p2.x - p1.x) / n; // ���� ĭ ����
	LONG dy = (p2.y - p1.y) / n; // ���� ĭ ����

	// ���� �� �׸���
	for (LONG j = p1.y; j <= p2.y; j += dy)
	{
		MoveToEx(hdc, p1.x, j, NULL);
		LineTo(hdc, p2.x, j);
	}
	// ���� �� �׸���
	for (LONG i = p1.x; i <= p2.x; i += dx)
	{
		MoveToEx(hdc, i, p1.y, NULL);
		LineTo(hdc, i, p2.y);
	}
}
void DrawGrid(HDC hdc, POINT c1, UINT dx, UINT n)		// �߽� ��ġ, ����, ���� ����
{
	LONG d = (n / 2) * dx;
	POINT p1{ c1.x - d, c1.y - d };
	POINT p2{ c1.x + d, c1.y + d };
	DrawGrid(hdc, p1, p2, n);							// �߽� ��ġ�� ������ �˸�, ���� ��ġ�� �� ��ġ�� �� �� �־��. �̸� �̿��ؼ� DrawGrid �� �Լ��� ���� �߽��ϴ�.
}

void DrawEllipse(HDC hdc, POINT c1, LONG r)
{
	Ellipse(hdc, c1.x - r, c1.y - r, c1.x + r, c1.y + r);
}

void DrawSunFlower(HDC hdc, POINT c1, LONG r1, LONG r2)		// �ܰ��� ���������� �׸��� �عٶ�� �׸���
{
	// n_out ���ϱ�
	LONG n_out = (LONG)(round(180 / (asin((double)r2 / (r1 + r2)) * (180 / PI))));	// ���� �� �翡 ���� ��� ���⳪? �������� ���� �̿��߽��ϴ�. sin���Լ��� ���ǳ׿�

	// ���� �� �׸���
	for (LONG theta = 0; theta < 360; theta += (360 / n_out))						// ���� �� �翡 �ִ� ������ �߽��� r1+r2�������� ������ ���ӿ��� �����߽��ϴ�. ���� ��� ���� sin cos���� ǥ���˴ϴ�.
	{
		LONG x2{ (LONG)(c1.x + (double)(r1 + r2) * cos(theta * PI / 180)) };
		LONG y2{ (LONG)(c1.y + (double)(r1 + r2) * sin(theta * PI / 180)) };
		DrawEllipse(hdc, POINT{ x2,y2 }, r2);
	}
}

void DrawSunFlowerN(HDC hdc, POINT c1, LONG r1, LONG n)		// �ܰ��� ������ �׸��� �عٶ�� �׸���
{
	// r2 ���ϱ�
	double s = sin(360 / n / 2 * PI / 180);
	LONG r2 = (LONG)(round((r1 * (sin(s) / (1 - sin(s))))));						// DrawEllipseFlower���� n_out���ϴ� ������ �����ϸ� �ǿ�

	// ���� �� �׸���
	for (LONG theta = 0; theta < 360; theta += (360 / n))						// ���� �� �翡 �ִ� ������ �߽��� r1+r2�������� ������ ���ӿ��� �����߽��ϴ�. ���� ��� ���� sin cos���� ǥ���˴ϴ�.
	{
		LONG x2{ (LONG)(c1.x + (double)(r1 + r2) * cos(theta * PI / 180)) };
		LONG y2{ (LONG)(c1.y + (double)(r1 + r2) * sin(theta * PI / 180)) };
		DrawEllipse(hdc, POINT{ x2,y2 }, r2);
	}
}

void DrawRectangle(HDC hdc, POINT c1, LONG w, LONG h)		// �ڵ�, �߽��� ��ġ, ����, ����
{
	Rectangle(hdc, c1.x - w / 2, c1.y - h / 2, c1.x + w / 2, c1.y + h / 2);
}

void DrawRectangle(HDC hdc, POINT c1, LONG w, LONG h, float tilt)		// �ڵ�, �߽��� ��ġ, ����, ����, ȸ���� ����
{
	// p1234�� ���������� �簢�� ��ġ pts�� translation & rotation ��ȯ �� ���� ��ġ ���� ����
	POINT p1234[4] = { -w / 2, -h / 2, w / 2, -h / 2, w / 2, h / 2, -w / 2, h / 2 };
	POINT pts[4] = { NULL, };
	// ��ȯ ��� ����
	getRectPoints(c1, w, h, tilt, pts);
	// �׸���
	DrawRectangle(hdc, pts);
}
void DrawRectangle(HDC hdc, POINT *c1)									// �ڵ�, �߽��� ��ġ, ����, ����, ȸ���� ����
{
	int i;
	for (i = 0; i < 3; i++)
	{
		MoveToEx(hdc, c1[i].x, c1[i].y, NULL);
		LineTo(hdc, c1[i + 1].x, c1[i + 1].y);
	}
	MoveToEx(hdc, c1[i].x, c1[i].y, NULL);
	LineTo(hdc, c1[0].x, c1[0].y);
}
void DrawInputText(HDC hdc, RECT r1, LPCTSTR str, UINT Flags, bool Caret)
{
	// �簢�� �׸���
	POINT c1{ (r1.left + r1.right) / 2, (r1.top + r1.bottom) / 2 };
	LONG w{ r1.right - r1.left };
	LONG h{ r1.bottom - r1.top };
	DrawRectangle(hdc, c1, w, h);

	// �ؽ�Ʈ �׸���
	LONG gap = 2;
	RECT r2{ r1.left + gap, r1.top + gap, r1.right - gap, r1.bottom - gap };	// ĳ�� ũ�� �����ؼ� �۰� �簢�� ����
		// �ؽ�Ʈ ĳ�� ����
	if (Caret == true)
	{
		SIZE size;
		GetTextExtentPoint(hdc, str, _tcslen(str), &size);
		SetCaretPos(r2.left + size.cx, r1.top);
	}
	// �ؽ�Ʈ �׸���
	DrawText(hdc, str, _tcslen(str), &r2, Flags);
}

void DrawInputText(HDC hdc, POINT c, LONG w, LONG h, LPCTSTR str, UINT Flags, bool Caret)
{
	// �簢�� �׸���
	DrawRectangle(hdc, c, w, h);

	// �ؽ�Ʈ �׸���
	LONG gap = 2;
	RECT r2{ c.x - w / 2 + gap, c.y - h / 2 + gap, c.x + w / 2 - gap, c.y + h / 2 - gap };	// ĳ�� ũ�� �����ؼ� �۰� �簢�� ����
		// �ؽ�Ʈ ĳ�� ����
	if (Caret == true)
	{
		SIZE size;
		GetTextExtentPoint(hdc, str, _tcslen(str), &size);
		SetCaretPos(c.x - w / 2 + size.cx, c.y - h / 2);
	}
	// �ؽ�Ʈ �׸���
	DrawText(hdc, str, _tcslen(str), &r2, Flags);
}

void DrawStar(HDC hdc, POINT c, LONG r)
{
	double double_d;	// �߽ɿ��� ������ �Ÿ�
	POINT point[10];	// �ٱ� ������ �� �� ������ ����
	LONG pointIdx = 0;	// ������ �ε���
	int theta = 90 - 2 * (360 / (2 * 5));
	double theta2rad = PI / 180;

	double short_d = r * sin(theta * theta2rad) / cos(theta * theta2rad);	// �߽ɿ��� ���� ���������� �Ÿ�
	for (theta = 0; theta < 360; theta += (360 / 10))
	{
		// ������ ���� �޶����� d ����
		if (theta / (360 / 10) % 2 == 1)
			double_d = short_d;
		else
			double_d = r;

		// �ٱ� ������ �� �� ������ ����
		double x = cos(theta * theta2rad) * double_d + c.x;		// �����̵� ��� & ȸ�� ����� �����ϼŵ� �ǰ�, ���ͷ� �����ϼŵ� �˴ϴ�.
		double y = sin(theta * theta2rad) * double_d + c.y;
		point[pointIdx++] = { (LONG)x, (LONG)y };
	}

	// �� �׸���
	Polygon(hdc, point, 10);
}


void DrawStarN(HDC hdc, POINT c, LONG r, LONG n)
{
	double double_d;						// �߽ɿ��� ������ �Ÿ�
	POINT * point = new POINT[2 * n];		// �ٱ� ������ �� �� ������ ����
	LONG pointIdx = 0;						// ������ �ε���
	int theta = 90 - 2 * (360 / (2 * n));	// ������ ���� ����
	double theta2rad = PI / 180;

	// ������ Ŀ���鼭 ������ ��ġ�� �ϳ��� point �迭�� ����
	double short_d = r * sin(theta * theta2rad) / cos(theta * theta2rad);	// �߽ɿ��� ���� ���������� �Ÿ�
	for (theta = 0; theta < 360; theta += (360 / (2 * n)))
	{
		// ������ ���� �޶����� d ����
		if (theta / (360 / (2 * n)) % 2 == 1)
			double_d = short_d;
		else
			double_d = r;

		// �ٱ� ������ �� �� ������ ����
		double x = cos(theta * theta2rad) * double_d + c.x;		// �����̵� ��� & ȸ�� ����� �����ϼŵ� �ǰ�, ���ͷ� �����ϼŵ� �˴ϴ�.
		double y = sin(theta * theta2rad) * double_d + c.y;
		point[pointIdx++] = { (LONG)x, (LONG)y };
	}

	// �� �׸���
	Polygon(hdc, point, 2 * n);

	delete[] point;
}

void arrowKeysColor(HDC hdc, POINT c, LONG d, COLORREF crcolor, LONG keyVal)
{
	POINT left_c = { c.x - d, c.y };
	POINT top_c = { c.x, c.y - d };
	POINT right_c = { c.x + d, c.y };

	// ����Ű 4�� �׸���
	DrawInputText(hdc, top_c, d, d, _T("Up"), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawInputText(hdc, c, d, d, _T("Down"), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawInputText(hdc, left_c, d, d, _T("Left"), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawInputText(hdc, right_c, d, d, _T("Right"), DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	// keyVal != 0 �̸�
		// �� ���� ���� �׸���
	if (keyVal != 0)
	{
		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		switch (keyVal)
		{
		case VK_LEFT:
			DrawRectangle(hdc, left_c, d, d);
			break;
		case VK_RIGHT:
			DrawRectangle(hdc, right_c, d, d);
			break;
		case VK_UP:
			DrawRectangle(hdc, top_c, d, d);
			break;
		case VK_DOWN:
			DrawRectangle(hdc, c, d, d);
			break;
		default:
		{
		}
		}
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
	}
}

void getRectPoints(POINT c1, LONG w, LONG h, float tilt, POINT *pts)		// �ڵ�, �߽��� ��ġ, ����, ����, ȸ���� ����
{
	// p1234�� ���������� �簢�� ��ġ pts�� translation & rotation ��ȯ �� ���� ��ġ ���� ����
	POINT p1234[4] = { -w / 2, -h / 2, w / 2, -h / 2, w / 2, h / 2, -w / 2, h / 2 };


	// ��ȯ ��� ����
	for (int i = 0; i < 4; i++)
	{
		POINT pt{ NULL, };
		pts[i].x = (long)(p1234[i].x * cos((double)tilt) - p1234[i].y * sin((double)tilt) + c1.x);
		pts[i].y = (long)(p1234[i].x * sin((double)tilt) + p1234[i].y * cos((double)tilt) + c1.y);
	}
}

template <typename T>
T pointDotProduct(POINT a, POINT b)
{
	return a.x * b.x + a.y * b.y;
}

bool satAlgorithm(POINT *object1Pt, POINT *object2Pt, float &shortest_distance, std::pair<float, float> &shortest_normal_vector)
{
	// �� ���ͷ� ��ġ���� ���� �Ǵ�
			// ��� ���� ���ϱ�

	// obj1 �� ���� ���� ����
	std::vector<std::pair<float, float>> norms;
	float fdistance = 0.f;			// ���� ���� ���� ����� ��Į�� ��
	for (int i = 0; i < 3; i++)
	{
		fdistance = getfDistance(object1Pt[i], object1Pt[i + 1]);
		norms.emplace_back(std::pair<float, float>{ -(object1Pt[i + 1].y - object1Pt[i].y) / fdistance, (object1Pt[i + 1].x - object1Pt[i].x) / fdistance });
	}
	fdistance = getfDistance(object1Pt[0], object1Pt[3]);
	norms.emplace_back(std::pair<float, float>{ -(object1Pt[0].y - object1Pt[3].y) / fdistance, (object1Pt[0].x - object1Pt[3].x) / fdistance });

	// obj2 �� ���� ���� ����
	for (int i = 0; i < 3; i++)
	{
		fdistance = getfDistance(object2Pt[i], object2Pt[i + 1]);
		norms.emplace_back(std::pair<float, float>{ -(object2Pt[i + 1].y - object2Pt[i].y) / fdistance, (object2Pt[i + 1].x - object2Pt[i].x) / fdistance });
	}
	fdistance = getfDistance(object2Pt[0], object2Pt[3]);
	norms.emplace_back(std::pair<float, float>{ -(object2Pt[0].y - object2Pt[3].y) / fdistance, (object2Pt[0].x - object2Pt[3].x) / fdistance });

	// �� ��� ���Ϳ� ���翵�� �� ���ϱ�
		// ��ġ���� �Ǵ�
		// �� ������ �ִ� �ּ� �� ���ϱ�
		// (�ִ� - �ּ�), (�ּ� - �ִ�) �� ���밪 ���� ���� �ּ� �Ÿ�
		// �� ���͵� ����

	auto pairFloat2DotProduct = [](POINT p1, std::pair<float, float> v1)	// ���� �ζ��� �Լ�
	{
		return p1.x * v1.first + p1.y * v1.second;
	};

	// �� ���� ���Ϳ� ���� obj���� �� ���翵
	float min_abs = 3.4E+37f;
	for (std::pair<float, float> &norm : norms)
	{
		// �ش� ���� ���Ϳ� ���� obj1 ���翵
		float proj1 = pairFloat2DotProduct(object1Pt[0], norm);
		float obj1_proj_max = proj1, obj1_proj_min = proj1;
		for (int i = 1; i < 4; i++)
		{
			proj1 = pairFloat2DotProduct(object1Pt[i], norm);
			if (proj1 >= obj1_proj_max) obj1_proj_max = proj1;				// �ִ� �ּ� ���� �� ������Ʈ
			if (proj1 <= obj1_proj_min) obj1_proj_min = proj1;
		}

		// �ش� ���� ���Ϳ� ���� obj2 ���翵
		proj1 = pairFloat2DotProduct(object2Pt[0], norm);
		float obj2_proj_max = proj1, obj2_proj_min = proj1;
		for (int i = 1; i < 4; i++)
		{
			proj1 = pairFloat2DotProduct(object2Pt[i], norm);
			if (proj1 >= obj2_proj_max) obj2_proj_max = proj1;				// �ִ� �ּ� ���� �� ������Ʈ
			if (proj1 <= obj2_proj_min) obj2_proj_min = proj1;
		}

		if (obj1_proj_min <= obj2_proj_max && obj1_proj_max >= obj2_proj_min)
		{
			// ��ġ��
			// �ּ� ���밪 ���ϱ�
			fabs(obj2_proj_max - obj1_proj_min) <= fabs(obj1_proj_max - obj2_proj_min) ?
				min_abs = fabs(obj2_proj_max - obj1_proj_min) : min_abs = fabs(obj1_proj_max - obj2_proj_min);		// ���� �ּ� ���밪 ������Ʈ

			if (min_abs <= shortest_distance)	// ���� �ּ� ���밪�� �� �� �ּҰ� �� ���� ���� ������Ʈ
			{
				shortest_distance = min_abs;
				shortest_normal_vector = norm;
			}
		}
		else
			return false;	// �Ȱ�ħ
	}
	// ��ħ
	return true;
}

bool doCirclesOverlap(float x1, float y1, float r1, float x2, float y2, float r2)
{
	return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) <= (r1 + r2)*(r1 + r2);
}

float getfDistance(POINT p1, POINT p2)
{
	return sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

float getL2N(float a, float b)
{
	return sqrtf(a*a + b * b);
}