#pragma once

#ifndef FIGUREDRAW_H_
#define FIGUREDRAW_H_

#include <iostream>
#include <windowsx.h>
#include <windows.h>
#include <WinUser.h>

void DrawGrid(HDC, POINT, POINT, UINT);									// �ڵ�, ���� ��ġ, �� ��ġ, ���� ����
void DrawGrid(HDC, POINT, UINT, UINT);									// �ڵ�, �߽� ��ġ, ����, ���� ����
void DrawEllipse(HDC hdc, POINT c1, LONG r);							// �ڵ�, �߽� ��ġ, ������
void DrawSunFlower(HDC hdc, POINT c1, LONG r1, LONG r2);				// �ڵ�, ���� �� �߽� ��ġ, ���� �� ������, �ٱ� �� ������
void DrawSunFlowerN(HDC hdc, POINT c1, LONG r1, LONG n);				// �ڵ�, ���� �� �߽� ��ġ, ���� �� ������, �ٱ� �� ������
void DrawRectangle(HDC hdc, POINT c1, LONG w, LONG h);					// �ڵ�, �߽��� ��ġ, ����, ����
void DrawRectangle(HDC hdc, POINT c1, LONG w, LONG h, float tilt);		// �ڵ�, �߽��� ��ġ, ����, ����, ȸ���� ����
void DrawRectangle(HDC hdc, POINT *c1);									// �ڵ�, �߽��� ��ġ, ����, ����, ȸ���� ����
void DrawInputText(HDC hdc, RECT r1, LPCTSTR str,						// �ڵ�, �簢��, ����, ĳ�� ����
	UINT Flags = DT_TOP | DT_LEFT | DT_SINGLELINE, bool Caret = false);
void DrawInputText(HDC hdc, POINT c, LONG w, LONG h,					
	LPCTSTR str, UINT Flags = DT_TOP | DT_LEFT | DT_SINGLELINE, bool Carett = false);	// �ڵ�, �߽���, ����, ����, ����, ĳ�� ����
void DrawStar(HDC hdc, POINT c, LONG r);								// �ڵ�, �߽���, ������
void DrawStarN(HDC hdc, POINT c, LONG r, LONG n);						// �ڵ�, �߽���, ������, �ٱ� ������ ����
void arrowKeysColor(HDC hdc, POINT c, LONG d,							// �ڵ�, �߽���, ����Ű ũ��, ����, ����Ű ��
	COLORREF crcolor, LONG keyVal);										// �ڵ�, �߽���, ����Ű ũ��, ����, ����Ű ��
void getRectPoints(POINT c1, LONG w, LONG h, float tilt, POINT *pts);	// �ڵ�, �߽��� ��ġ, ����, ����, ȸ���� ����
template <typename T>
T pointDotProduct(POINT a, POINT b);									// ����Ʈ �ڷ��� ���� �� ��ȯ

// input = ������1, ������2, ��ȯ ���� ���� ª�� �Ÿ�, ��ȯ ���� ���� ª�� �Ÿ� ����
// output = ��ħ ����
// obj1, obj2 ������ 2���� ���� ��, �� ������ ��ġ���� ���θ� ��ȯ�Ѵ�. ���� ª�� ���� �Ÿ� �� ������ �Ű������� ���� ������ ���� �� �ִ�.
bool satAlgorithm(POINT *object1Pt, POINT *object2Pt, float &shortest_distance, std::pair<float, float> &shortest_normal_vector);

bool doCirclesOverlap(float x1, float y1, float r1, float x2, float y2, float r2);		// �� ���� �߽��� �� ������
float getfDistance(POINT p1, POINT p2);													// ����Ʈ�� �� �ΰ��� L2N
float getL2N(float a, float b);

#endif