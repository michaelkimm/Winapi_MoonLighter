#include "SceneManager.h"
#include "InGameScene.h"
#include "MapEditScene.h"
#include "AssistScene.h"
#include "..\Core\PathManager.h"


DEFINE_SINGLETON(CSceneManager)

CSceneManager::CSceneManager()
	: pt_scene_(NULL), pt_next_scene_(NULL), pt_assist_scene_(NULL), pt_map_edit_scene_(NULL), pt_tileset_setting_scene_(NULL)
{

}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(pt_scene_);
	SAFE_DELETE(pt_next_scene_);
	SAFE_DELETE(pt_assist_scene_);
	SAFE_DELETE(pt_map_edit_scene_);
	SAFE_DELETE(pt_tileset_setting_scene_);
}

void CSceneManager::LoadHwnd(const string& _scene_name, HWND _hWnd)
{
	hwnd_map_.insert(make_pair(_scene_name, _hWnd));
}


HWND CSceneManager::GetHwnd(const string& _scene_name) const
{
	auto search = hwnd_map_.find(_scene_name);
	if (search != hwnd_map_.end())
	{
		return search->second;
	}

	return NULL;
}

bool CSceneManager::Init()
{
	// CInGameScene 타입으로 현재 씬을 만든다.
	// CreateScene<CInGameScene>(SC_CURRENT);
	
	return true;
}

void CSceneManager::Input(float _time)
{
	// 순서 중요!
	if (pt_assist_scene_)
		pt_assist_scene_->Input(_time);
	if (pt_map_edit_scene_)
		pt_map_edit_scene_->Input(_time);
	if (pt_tileset_setting_scene_)
		pt_tileset_setting_scene_->Input(_time);
	// pt_scene_->Input(_time);
}

void CSceneManager::Update(float _time)
{
	// pt_scene_->Update(_time);
}

void CSceneManager::LateUpdate(float _time)
{
	// pt_scene_->LateUpdate(_time);
}

void CSceneManager::Collision(float _time)
{
	// pt_scene_->Collision(_time);
}

void CSceneManager::Render(HDC _hdc, float _time)
{
	// pt_scene_->Render(_hdc, _time);
	// pt_assist_scene_->Render(_hdc, _time);
}

void CSceneManager::Save(FILE* _pt_file)
{
	pt_map_edit_scene_->Save(_pt_file);
}
void CSceneManager::Load(FILE* _pt_file)
{
	pt_map_edit_scene_->Load(_pt_file);
}

void CSceneManager::SaveFromPath(const char * _pt_file_name, const string & _str_path_key, bool _full_path)
{
	// 파일 절대경로 가져오기
	wstring path = CPathManager::Instance()->FindPath(_str_path_key);

	// wstring to string
	string tmp_str = "";
	
	if (!_full_path)
	{
		tmp_str.assign(path.begin(), path.end());

		// 절대경로 완성
		tmp_str += _pt_file_name;
	}
	else
	{
		tmp_str = _pt_file_name;
	}

	FILE* pt_file = NULL;

	// 파일 쓰기 설정
	if (fopen_s(&pt_file, tmp_str.c_str(), "wb"))
	{
		cout << "파일 저장중.. 파일 열기 실패!\n";
		return;
	}

	// 파일 쓰기
	if (pt_file)
	{
		Save(pt_file);

		fclose(pt_file);
	}
}

void CSceneManager::LoadFromPath(const char * _pt_file_name, const string & _str_path_key, bool _full_path)
{
	// 파일 절대경로 가져오기
	wstring path = CPathManager::Instance()->FindPath(_str_path_key);

	// wstring to string
	string tmp_str = "";
	if (!_full_path)
	{
		tmp_str.assign(path.begin(), path.end());

		// 절대경로 완성
		tmp_str += _pt_file_name;
	}
	else
	{
		tmp_str = _pt_file_name;
	}

	FILE* pt_file = NULL;

	// 파일 불러오기 설정
	if (fopen_s(&pt_file, tmp_str.c_str(), "rb"))
	{
		cout << "파일 로딩중.. 파일 열기 실패!\n";
		return;
	}

	// 파일 쓰기
	if (pt_file)
	{
		Load(pt_file);

		fclose(pt_file);
	}
}
