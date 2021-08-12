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

	// 텍스쳐 path가 없으면 리턴	
	if (texture_path == NULL)
		return false;

	// 텍스쳐 path, 이름 삽입
	wstring path_ = texture_path;
	wstring tmp_path_ = texture_path;
	path_ += _file_name;

	// Save & Load 위해 텍스쳐 키 & 파일 이름 & 경로 키
	texture_key_ = _texture_key;
	file_name_ = _file_name;
	path_key_ = _str_path_key;

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

void CTexture::Save(FILE * _pt_file)
{
	/*
	
	// 텍스쳐 키 & 파일 이름 & 경로 키
	string texture_key_;
	wstring file_name_;
	string path_key_;
	
	*/

	// 텍스쳐 키 저장
	int len = texture_key_.length();
	fwrite(&len, 4, 1, _pt_file);
	fwrite(texture_key_.c_str(), 1, len, _pt_file);

	// 텍스쳐 파일 이름 저장
	len = file_name_.length();
	fwrite(&len, 4, 1, _pt_file);
	fwrite(file_name_.c_str(), 2, len, _pt_file);

	// 경로 키 저장
	len = path_key_.length();
	fwrite(&len, 4, 1, _pt_file);
	fwrite(path_key_.c_str(), 1, len, _pt_file);

	// 타일 옵션 벡터 저장
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

	// 텍스쳐 키 읽기
	fread(&len, 4, 1, _pt_file);
	fread(&str_key, 1, len, _pt_file);
	str_key[len] = '\0';

	// 텍스쳐 파일 이름 읽기
	fread(&len, 4, 1, _pt_file);
	fread(&str_filename, 2, len, _pt_file);
	str_filename[len] = '\0';

	// 경로 키 읽기
	fread(&len, 4, 1, _pt_file);
	fread(&str_path, 1, len, _pt_file);
	str_path[len] = '\0';

	// 타일 옵션 벡터 저장
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
	
	// 자원 관리자에 텍스쳐 업로드
	CSourceManager::Instance()->LoadTexture(str_key, str_filename, str_path);
}
