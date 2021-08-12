#include "Texture.h"
#include "PathManager.h"
#include "Core.h"
#include "SourceManager.h"

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

bool CTexture::SetTexture(HINSTANCE _hInst, HDC& _hdc, const string& _texture_key, const wchar_t * _file_name, const string& _str_path_key, const COLORREF& _color_key)
{
	const wchar_t* texture_path = CPathManager::Instance()->FindPath(_str_path_key);

	// �ؽ��� path�� ������ ����	
	if (texture_path == NULL)
		return false;

	// �ؽ��� path, �̸� ����
	wstring path_ = texture_path;
	wstring tmp_path_ = texture_path;
	path_ += _file_name;

	// Save & Load ���� �ؽ��� Ű & ���� �̸� & ��� Ű
	texture_key_ = _texture_key;
	file_name_ = _file_name;
	path_key_ = _str_path_key;

	// �̹��� ����
	hBitmap_ = (HBITMAP)LoadImageW(_hInst, path_.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBitmap_, sizeof(BITMAP), &bitmap_);

	// Į��Ű ����
	SetColorKey(_color_key);

	// �ʺ� & ���� ����
	w_ = bitmap_.bmWidth;
	h_ = bitmap_.bmHeight;
	
	// "�ؽ��� �� �� ���� Ÿ���� �ɼ� ���� ����" ���� �ʱ�ȭ
	option_vec_.assign((w_ / TEXTURE_SIZE) * (h_ / TEXTURE_SIZE), TILE_NONE);

	// DC ����
	if (hMemDC_)
		DeleteDC(hMemDC_);
	hMemDC_ = CreateCompatibleDC(_hdc);
	if (hMemDC_ == NULL)
		return false;


	// DC�� �׷�����
	if (hDoubleBufferImage_ == NULL)
		hDoubleBufferImage_ = CreateCompatibleBitmap(_hdc, w_, h_);

	// ��ȭ�� ���� hBitmap �׸���
	hOldBitmap_ = (HBITMAP)SelectObject(hMemDC_, hDoubleBufferImage_);
	hOldBitmap_ = (HBITMAP)SelectObject(hMemDC_, hBitmap_);

	return true;
}

void CTexture::Render(HDC _hdc, float _time)
{
}

void CTexture::Save(FILE * _pt_file)
{
	/*
	
	// �ؽ��� Ű & ���� �̸� & ��� Ű
	string texture_key_;
	wstring file_name_;
	string path_key_;
	
	*/

	// �ؽ��� Ű ����
	int len = texture_key_.length();
	fwrite(&len, 4, 1, _pt_file);
	fwrite(texture_key_.c_str(), 1, len, _pt_file);

	// �ؽ��� ���� �̸� ����
	len = file_name_.length();
	fwrite(&len, 4, 1, _pt_file);
	fwrite(file_name_.c_str(), 2, len, _pt_file);

	// ��� Ű ����
	len = path_key_.length();
	fwrite(&len, 4, 1, _pt_file);
	fwrite(path_key_.c_str(), 1, len, _pt_file);

	// Ÿ�� �ɼ� ���� ����
	len = option_vec_.size();
	fwrite(&len, 4, 1, _pt_file);
	vector<TILE_OPTION>::iterator iter;
	vector<TILE_OPTION>::iterator iter_end = option_vec_.end();
	for (iter = option_vec_.begin(); iter != iter_end; iter++)
	{
		fwrite(&(*iter), 4, 1, _pt_file);
	}
}

void CTexture::Load(FILE * _pt_file)
{
	int len;

	char str_key[MAX_PATH] = {};
	wchar_t str_filename[MAX_PATH] = {};
	char str_path[MAX_PATH] = {};

	// �ؽ��� Ű �б�
	fread(&len, 4, 1, _pt_file);
	fread(&str_key, 1, len, _pt_file);
	str_key[len] = '\0';

	// �ؽ��� ���� �̸� �б�
	fread(&len, 4, 1, _pt_file);
	fread(&str_filename, 2, len, _pt_file);
	str_filename[len] = '\0';

	// ��� Ű �б�
	fread(&len, 4, 1, _pt_file);
	fread(&str_path, 1, len, _pt_file);
	str_path[len] = '\0';

	// Ÿ�� �ɼ� ���� ����
	option_vec_.clear();
	fread(&len, 4, 1, _pt_file);
	option_vec_.resize(len, TILE_NONE);

	TILE_OPTION tmp_option;
	vector<TILE_OPTION>::iterator iter;
	vector<TILE_OPTION>::iterator iter_end = option_vec_.end();
	for (iter = option_vec_.begin(); iter != iter_end; iter++)
	{
		fread(&tmp_option, 4, 1, _pt_file);
		*iter = tmp_option;
	}
	
	// �ڿ� �����ڿ� �ؽ��� ���ε�
	CSourceManager::Instance()->LoadTexture(str_key, str_filename, str_path);
}
