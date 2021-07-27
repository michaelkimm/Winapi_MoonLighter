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

	// �̹� �ִ��� Ȯ��
	if (_pt_texture)
		return _pt_texture;

	_pt_texture = new CTexture();	// ref_cnt = 1;

	// ������ ���� ���
	if (!_pt_texture->SetTexture(hInst_, hdc_, _file_name))
	{
		SAFE_RELEASE(_pt_texture)
		return NULL;
	}

	// ������ �ؽ��� ��ü�� ����Ƽ� source_map�� �߰�
	source_map_.insert(make_pair(_str_key, _pt_texture));

	//// �ؽ�ó ������ �ı� �� Safe Releasse
	_pt_texture->AddRef();

	return _pt_texture;
}


CTexture* CSourceManager::FindTexture(const string& _str_key)
{
	unordered_map<string, class CTexture*>::iterator iter = source_map_.find(_str_key);

	if (iter == source_map_.end())
		return NULL;

	// �ؽ�ó�� ����Ű�� �����͸� �ǵ����ֱ� ������ ���� ī��Ʈ �߰�
	iter->second->AddRef();

	return iter->second;
}