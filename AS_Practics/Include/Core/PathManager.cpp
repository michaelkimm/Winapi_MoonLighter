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

	// 실행 파일 위치 얻기
	int nSize = GetModuleFileName(NULL, root_path_, MAX_PATH);
	
	// 실행 파일 폴더 위치 얻기
	for (int i = nSize; root_path_[i] != '\\'; i--)
	{
		root_path_[i] = '\0';
	}

	// 실행 파일 폴더 위치 path_map_에 대입
	path_map_.insert(make_pair(ROOT_PATH, root_path_));

	// 텍스쳐 폴더 위치 생성
	if (!CreatePath(TEXTURE_PATH, _T("Texture\\")))
		return false;

	// 데이터 폴더 위치 생성
	if (!CreatePath(DATA_PATH, _T("Data\\")))
		return false;

	return true;
}

bool CPathManager::CreatePath(const string& _str_key, const wchar_t* _file_name, const string& _root_key)
{
	const wchar_t* root_path_ = FindPath(_root_key);

	// 루트 path가 존재한다면
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