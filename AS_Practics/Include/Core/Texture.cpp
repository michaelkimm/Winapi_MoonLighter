#include "Texture.h"
#include "PathManager.h"

CTexture::CTexture()	:
	hMemDC_(NULL)
{
}

CTexture::~CTexture()
{
	SelectObject(hMemDC_, hOldBitmap_);
	DeleteDC(hMemDC_);
	DeleteObject(hBitmap_);
}

bool CTexture::SetTexture(HINSTANCE _hInst, HDC& _hdc, const wchar_t * _file_name)
{
	const wchar_t* texture_path = CPathManager::Instance()->FindPath(TEXTURE_PATH);

	// 텍스쳐 path가 없으면 리턴	
	if (texture_path == NULL)
		return false;

	// 텍스쳐 path, 이름 삽입
	wstring path_ = texture_path;
	path_ += _file_name;

	// 이미지 삽입
	// img_ = Image::FromFile(path_.c_str());
	hBitmap_ = (HBITMAP)LoadImageW(NULL, path_.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBitmap_, sizeof(BITMAP), &bitmap_);

	//// 예외 처리
	//if (!img_)
	//	return false;

	// 칼라키 범위 설정
	// imgAttr_.SetColorKey(_color_key1, _color_key2);
	// imgAttr_.SetColorKey(Color(200, 0, 200), Color(255, 10, 255));

	// 너비 & 높이 설정
	// w_ = img_->GetWidth();
	// h_ = img_->GetHeight();
	w_ = bitmap_.bmWidth;
	h_ = bitmap_.bmHeight;

	// DC 설정
	if (hMemDC_)
		DeleteDC(hMemDC_);
	hMemDC_ = CreateCompatibleDC(_hdc);
	if (hMemDC_ == NULL)
		return false;

	// Gdi_Draw(hMemDC_);

	// DC에 그려놓기
	// Graphics graphics(hMemDC_);
	// graphics.DrawImage(img_, Rect(0, 0, w_, h_), 0, 0, w_, h_, UnitPixel, &imgAttr_);
	hOldBitmap_ = (HBITMAP)SelectObject(hMemDC_, hBitmap_);

	return true;
	}

void CTexture::Render(HDC _hdc, float _time)
{
	Graphics graphics(_hdc);

	// graphics.DrawImage(img_, Rect(xPos - (float)w / 2, yPos - (float)h / 2, w, h), 0, 0, w, h, UnitPixel, &imgAttr_);
}
