 #include "SceneManager.h"
#include "InGameScene.h"

DEFINE_SINGLETON(CSceneManager)

CSceneManager::CSceneManager()
	: pt_scene_(NULL)
{

}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(pt_scene_)
}

bool CSceneManager::Init()
{
	// CInGameScene 타입으로 현재 씬을 만든다.
	CreateScene<CInGameScene>(SC_CURRENT);
	return true;
}

void CSceneManager::Input(float _time)
{
	pt_scene_->Input(_time);
}

void CSceneManager::Update(float _time)
{
	pt_scene_->Update(_time);
}

void CSceneManager::LateUpdate(float _time)
{
	pt_scene_->LateUpdate(_time);
}

void CSceneManager::Collision(float _time)
{
	pt_scene_->Collision(_time);
}

void CSceneManager::Render(HDC _hdc, float _time)
{
	pt_scene_->Render(_hdc, _time);
}


