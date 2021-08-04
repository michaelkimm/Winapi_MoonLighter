#include "SourceManager.h"
#include "Texture.h"

DEFINE_SINGLETON(CSourceManager)

CSourceManager::CSourceManager()
{
	
}

CSourceManager::~CSourceManager()
{
	SafeReleaseMap(source_map_);
	SAFE_RELEASE(back_buffer_);
}

CTexture * CSourceManager::GetBackBuffer() const
{
	back_buffer_->AddRef();
	return back_buffer_;
}

bool CSourceManager::Init(HINSTANCE _hInst, HDC& _hdc)
{
	hInst_ = _hInst;
	hdc_ = _hdc;

	// �� ���۸� �ҷ��´�.
	back_buffer_ = LoadTexture("back_buffer", _T("back_white.bmp"));

	CTexture* tmp_pt = LoadTexture(SV_BEACH_SUMMER, _T("SV_Beach_Summer.png"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(WATER1, _T("water1.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(WATER2, _T("water2.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(EMPTY_WHITE_16, _T("empty_white_16.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(EMPTY_BLACK_16, _T("empty_black_16.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(EMPTY_BW_16, _T("empty_bw_16.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	return true;
}

class CTexture* CSourceManager::LoadTexture(const string& _texture_key, const wchar_t* _file_name, const string& _str_path_key, const COLORREF& _color_key)
{
	class CTexture* _pt_texture = FindTexture(_texture_key);

	// �̹� �ִ��� Ȯ��
	if (_pt_texture)
		return _pt_texture;

	_pt_texture = new CTexture();	// ref_cnt = 1;

	// ������ ��ο� ���� ���
	if (!_pt_texture->SetTextureImg(_file_name, _str_path_key, _color_key))
	{
		SAFE_RELEASE(_pt_texture);
		return NULL;
	}

	// ������ ��ο� �ִ� ���, �ؽ��� ��ü�� ���� source_map�� �߰�
	source_map_.insert(make_pair(_texture_key, _pt_texture));

	// insert���� pt_texture �������� ������ ���� ī��Ʈ ����.
	_pt_texture->AddRef();

	return _pt_texture;
}


CTexture* CSourceManager::FindTexture(const string& _texture_key)
{
	unordered_map<string, class CTexture*>::iterator iter = source_map_.find(_texture_key);

	if (iter == source_map_.end())
		return NULL;

	// �ؽ�ó�� ����Ű�� �����͸� �ǵ����ֱ� ������ ���� ī��Ʈ �߰�
	iter->second->AddRef();

	return iter->second;
}