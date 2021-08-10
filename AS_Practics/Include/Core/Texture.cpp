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
	DeleteObject(hOldBitmap_);
}

TILE_OPTION CTexture::GetOptionVec(int _x, int _y) const
{
	return option_vec_[_x + _y * (w_ / TEXTURE_SIZE)];
}

void CTexture::SetOptionVec(TILE_OPTION _op, int _x, int _y)
{
	option_vec_[_x + _y * (w_ / TEXTURE_SIZE)] = _op;
}

bool CTexture::SetTexture(HINSTANCE _hInst, HDC& _hdc, const wchar_t * _file_name, const string& _str_path_key, const COLORREF& _color_key)
{
	const wchar_t* texture_path = CPathManager::Instance()->FindPath(_str_path_key);

	// 텍스쳐 path가 없으면 리턴	
	if (texture_path == NULL)
		return false;

	// 텍스쳐 path, 이름 삽입
	wstring path_ = texture_path;
	path_ += _file_name;

	// 이미지 삽입
	hBitmap_ = (HBITMAP)LoadImageW(_hInst, path_.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBitmap_, sizeof(BITMAP), &bitmap_);

	// 칼라키 설정
	SetColorKey(_color_key);

	// 너비 & 높이 설정
	w_ = bitmap_.bmWidth;
	h_ = bitmap_.bmHeight;
	
	// "텍스쳐 내 각 단위 타일의 옵션 정보 저장" 벡터 초기화
	option_vec_.assign((w_ / TEXTURE_SIZE) * (h_ / TEXTURE_SIZE), TILE_NONE);

	// DC 설정
	if (hMemDC_)
		DeleteDC(hMemDC_);
	hMemDC_ = CreateCompatibleDC(_hdc);
	if (hMemDC_ == NULL)
		return false;


	// DC에 그려놓기
	if (hDoubleBufferImage_ == NULL)
		hDoubleBufferImage_ = CreateCompatibleBitmap(_hdc, w_, h_);

	// 도화지 덮고 hBitmap 그리기
	hOldBitmap_ = (HBITMAP)SelectObject(hMemDC_, hDoubleBufferImage_);
	hOldBitmap_ = (HBITMAP)SelectObject(hMemDC_, hBitmap_);

	return true;
}

void CTexture::Render(HDC _hdc, float _time)
{
}
