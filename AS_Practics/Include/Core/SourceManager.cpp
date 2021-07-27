#include "SourceManager.h"
#include "Texture.h"

DEFINE_SINGLETON(CSourceManager)

CSourceManager::CSourceManager()
{
	
}

CSourceManager::~CSourceManager()
{
	SafeReleaseMap(source_map_);
}

bool CSourceManager::Init(HINSTANCE _hInst, HDC& _hdc)
{
	hInst_ = _hInst;
	hdc_ = _hdc;
	return true;
}

class CTexture* CSourceManager::LoadTexture(const string& _str_key, const wchar_t* _file_name, const string& _str_path_key)
{
	class CTexture* _pt_texture = FindTexture(_str_key);

	// 이미 있는지 확인
	if (_pt_texture)
		return _pt_texture;

	_pt_texture = new CTexture();	// ref_cnt = 1;

	// 파일이 없는 경우
	if (!_pt_texture->SetTexture(hInst_, hdc_, _file_name))
	{
		SAFE_RELEASE(_pt_texture)
		return NULL;
	}

	// 있으면 텍스쳐 객체를 만들아서 source_map에 추가
	source_map_.insert(make_pair(_str_key, _pt_texture));

	//// 텍스처 포인터 파괴 전 Safe Releasse
	_pt_texture->AddRef();

	return _pt_texture;
}


CTexture* CSourceManager::FindTexture(const string& _str_key)
{
	unordered_map<string, class CTexture*>::iterator iter = source_map_.find(_str_key);

	if (iter == source_map_.end())
		return NULL;

	// 텍스처를 가리키는 포인터를 되돌려주기 때문에 참조 카운트 추가
	iter->second->AddRef();

	return iter->second;
}