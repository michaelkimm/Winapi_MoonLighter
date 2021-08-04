#include "Texture.h"
#include "PathManager.h"
#include "Core.h"

CTexture::CTexture()
	// hMemDC_(NULL),
	// hDoubleBufferImage_(NULL)
{
}

CTexture::~CTexture()
{
	// SelectObject(hMemDC_, hOldBitmap_);
	// DeleteDC(hMemDC_);
	// DeleteObject(hBitmap_);
	// DeleteObject(hDoubleBufferImage_);
	SAFE_DELETE(img_);
}

bool CTexture::SetTexture(HINSTANCE _hInst, HDC& _hdc, const wchar_t * _file_name, const string& _str_path_key, const COLORREF& _color_key)
{
	//const wchar_t* texture_path = CPathManager::Instance()->FindPath(_str_path_key);

	//// �ؽ��� path�� ������ ����	
	//if (texture_path == NULL)
	//	return false;

	//// �ؽ��� path, �̸� ����
	//wstring path_ = texture_path;
	//path_ += _file_name;

	//// �̹��� ����
	//// img_ = Image::FromFile(path_.c_str());
	//hBitmap_ = (HBITMAP)LoadImageW(_hInst, path_.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//GetObject(hBitmap_, sizeof(BITMAP), &bitmap_);

	//// Į��Ű ����
	//SetColorKey(_color_key);

	////// ���� ó��
	////if (!img_)
	////	return false;

	//// Į��Ű ���� ����
	//// imgAttr_.SetColorKey(_color_key1, _color_key2);
	//// imgAttr_.SetColorKey(Color(255, 0, 200), Color(255, 10, 255));

	//// �ʺ� & ���� ����
	//// w_ = img_->GetWidth();
	//// h_ = img_->GetHeight();
	//w_ = bitmap_.bmWidth;
	//h_ = bitmap_.bmHeight;

	//// DC ����
	//if (hMemDC_)
	//	DeleteDC(hMemDC_);
	//hMemDC_ = CreateCompatibleDC(_hdc);
	//if (hMemDC_ == NULL)
	//	return false;


	//// DC�� �׷�����
	//if (hDoubleBufferImage_ == NULL)
	//	hDoubleBufferImage_ = CreateCompatibleBitmap(_hdc, w_, h_);


	//hOldBitmap_ = (HBITMAP)SelectObject(hMemDC_, hDoubleBufferImage_);

	//// Graphics graphics(hMemDC_);

	//// graphics.DrawImage(img_, Rect(0, 0, w_, h_), 0, 0, w_, h_, UnitPixel, &imgAttr_);
	//hOldBitmap_ = (HBITMAP)SelectObject(hMemDC_, hBitmap_);

	return true;
}


bool CTexture::SetTextureImg(const wchar_t* _file_name, const string& _str_path_key, const COLORREF& _color_key)
{
	const wchar_t* texture_path = CPathManager::Instance()->FindPath(_str_path_key);

	// �ؽ��� path�� ������ ����	
	if (texture_path == NULL)
		return false;

	// �ؽ��� path, �̸� ����
	wstring path_ = texture_path;
	path_ += _file_name;

	// �̹��� ����
	img_ = Image::FromFile(path_.c_str());

	// Į��Ű ����
	SetColorKey(_color_key);

	// ���� ó��
	if (!img_)
		return false;

	// Į��Ű ���� ����
	imgAttr_.SetColorKey(color_key, color_key);
	
	// �ʺ� & ���� ����
	w_ = img_->GetWidth();
	h_ = img_->GetHeight();

	return true;
}

//bool CTexture::SetImgInfo(Image* _pt_img, int _srcx, int _srcy, int _srcwidth, int _srcheight, const COLORREF& _color_key)
//{
//	img_ = _pt_img;
//	x_ = _srcx;
//	y_ = _srcy;
//	w_ = _srcwidth;
//	h_ = _srcheight;
//	imgAttr_.SetColorKey(_color_key, _color_key);
//	return true;
//}


void CTexture::Render(HDC _hdc, float _time)
{
	Graphics graphics(_hdc);

	// graphics.DrawImage(img_, Rect(xPos - (float)w / 2, yPos - (float)h / 2, w, h), 0, 0, w, h, UnitPixel, &imgAttr_);
}
