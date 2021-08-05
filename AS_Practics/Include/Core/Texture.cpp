#include "Texture.h"
#include "PathManager.h"
#include "Core.h"

CTexture::CTexture()	:
	hMemDC_(NULL),
	hDoubleBufferImage_(NULL)
{
}

CTexture::~CTexture()
{
	SelectObject(hMemDC_, hOldBitmap_);
	DeleteDC(hMemDC_);
	DeleteObject(hBitmap_);
	DeleteObject(hDoubleBufferImage_);
}

bool CTexture::SetTexture(HINSTANCE _hInst, HDC& _hdc, const wchar_t * _file_name, const string& _str_path_key, const COLORREF& _color_key)
{
	const wchar_t* texture_path = CPathManager::Instance()->FindPath(_str_path_key);

	// �ؽ��� path�� ������ ����	
	if (texture_path == NULL)
		return false;

	// �ؽ��� path, �̸� ����
	wstring path_ = texture_path;
	path_ += _file_name;

	// �̹��� ����
	hBitmap_ = (HBITMAP)LoadImageW(_hInst, path_.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBitmap_, sizeof(BITMAP), &bitmap_);

	// Į��Ű ����
	SetColorKey(_color_key);

	// �ʺ� & ���� ����
	w_ = bitmap_.bmWidth;
	h_ = bitmap_.bmHeight;

	// DC ����
	if (hMemDC_)
		DeleteDC(hMemDC_);
	hMemDC_ = CreateCompatibleDC(_hdc);
	if (hMemDC_ == NULL)
		return false;


	// DC�� �׷�����
	if (hDoubleBufferImage_ == NULL)
		hDoubleBufferImage_ = CreateCompatibleBitmap(_hdc, w_, h_);


	hOldBitmap_ = (HBITMAP)SelectObject(hMemDC_, hDoubleBufferImage_);

	hOldBitmap_ = (HBITMAP)SelectObject(hMemDC_, hBitmap_);

	return true;
}

void CTexture::Render(HDC _hdc, float _time)
{
}
