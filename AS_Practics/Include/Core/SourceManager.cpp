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

	// 백 버퍼를 불러온다.
	back_buffer_ = LoadTexture("back_buffer", _T("back_white.bmp"), TEXTURE_PATH, RGB(255, 0, 255), true);

	CTexture* tmp_pt = LoadTexture(SV_BEACH_SUMMER, _T("SV_Beach_Summer_32_pink.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(SV_BEACH_WINTER, _T("SV_Beach_Winter_32_pink_1.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(WATER1, _T("water1_32.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(WATER2, _T("water2_32.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(SAND1, _T("sand1_32.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(EMPTY_WHITE_32, _T("empty_white_32.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(EMPTY_BLACK_32, _T("empty_black_32.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(EMPTY_BW_32, _T("empty_bw_32.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	tmp_pt = LoadTexture(EMPTY_GRAY_32, _T("empty_gray_32.bmp"));
	if (tmp_pt == NULL)
		return false;
	SAFE_RELEASE(tmp_pt);

	return true;
}

class CTexture* CSourceManager::LoadTexture(const string& _texture_key, const wchar_t* _file_name, const string& _str_path_key, const COLORREF& _color_key, bool _not_load_map)
{
	class CTexture* _pt_texture = FindTexture(_texture_key);

	// 이미 있는지 확인
	if (_pt_texture)
		return _pt_texture;

	_pt_texture = new CTexture;	// ref_cnt = 1;

	// 파일이 없는 경우
	if (!_pt_texture->SetTexture(hInst_, hdc_, _file_name, _str_path_key, _color_key))
	{
		SAFE_RELEASE(_pt_texture)
		return NULL;
	}
	if (!_not_load_map)
	{
		// 있으면 텍스쳐 객체를 만들아서 source_map에 추가
		source_map_.insert(make_pair(_texture_key, _pt_texture));

		// 텍스처 포인터 insert하기 때문에 참조 카운트 증가
		_pt_texture->AddRef();
	}

	return _pt_texture;
}


CTexture* CSourceManager::FindTexture(const string& _texture_key)
{
	unordered_map<string, class CTexture*>::iterator iter = source_map_.find(_texture_key);

	if (iter == source_map_.end())
		return NULL;

	// 텍스처를 가리키는 포인터를 되돌려주기 때문에 참조 카운트 추가
	iter->second->AddRef();

	return iter->second;
}


void CSourceManager::LoadTextureCombo(HWND _hwnd)
{
	unordered_map<string, class CTexture*>::iterator iter;
	unordered_map<string, class CTexture*>::iterator iter_end = source_map_.end();

	for (iter = source_map_.begin(); iter != iter_end; iter++)
	{
		const char* charBuff = (iter->first).c_str();
		TCHAR szUniCode[256] = { 0, };
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, charBuff, strlen(charBuff), szUniCode, 256);

		SendMessage(_hwnd, CB_ADDSTRING, 0, (LPARAM)szUniCode);
		cout << ((iter->first).c_str()) << endl;
	}
	return;
}