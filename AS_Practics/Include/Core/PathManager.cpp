#include "PathManager.h"

DEFINE_SINGLETON(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{

}

bool CPathManager::Init()
{
	wchar_t root_path_[MAX_PATH] = { NULL, };

	// ���� ���� ��ġ ���
	int nSize = GetModuleFileName(NULL, root_path_, MAX_PATH);
	
	// ���� ���� ���� ��ġ ���
	for (int i = nSize; root_path_[i] != '\\'; i--)
	{
		root_path_[i] = '\0';
	}

	// ���� ���� ���� ��ġ path_map_�� ����
	path_map_.insert(make_pair(ROOT_PATH, root_path_));

	// �ؽ��� ���� ��ġ ����
	if (!CreatePath(TEXTURE_PATH, _T("Texture\\")))
		return false;

	// ������ ���� ��ġ ����
	if (!CreatePath(DATA_PATH, _T("Data\\")))
		return false;

	return true;
}

bool CPathManager::CreatePath(const string& _str_key, const wchar_t* _file_name, const string& _root_key)
{
	const wchar_t* root_path_ = FindPath(_root_key);

	// ��Ʈ path�� �����Ѵٸ�
	if (root_path_ != NULL)
	{
		wstring tmp_wstring = wstring(root_path_) + wstring(_file_name);
		path_map_.insert(make_pair(_str_key, tmp_wstring));
		return true;
	}

	return false;
}


const wchar_t* CPathManager::FindPath(const string&_str_key)
{
	unordered_map<string, wstring>::iterator iter = path_map_.find(_str_key);

	if (iter == path_map_.end())
		return NULL;

	return iter->second.c_str();
}