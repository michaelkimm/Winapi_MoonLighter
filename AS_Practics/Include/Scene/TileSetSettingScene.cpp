#include "TileSetSettingScene.h"

#include "AssistScene.h"

#include "InGameScene.h"
#include "MapEditScene.h"
#include "SceneManager.h"

#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Tile.h"

#include "..\Core\Core.h"
#include "..\Core\SourceManager.h"
#include "..\Core\TfManager.h"
#include "..\Core\InputManager.h"
#include "..\Core\Texture.h"


CTileSetSettingScene::CTileSetSettingScene()
{

}
CTileSetSettingScene::~CTileSetSettingScene()
{

}

bool CTileSetSettingScene::Init(HWND _hWnd)
{

	if (!CMapToolScene::Init(_hWnd))
		return false;

	// AssistScene에서 텍스쳐 테그를 가져온다.
	CAssistScene* pt_assist = static_cast<CAssistScene*>(CSceneManager::Instance()->pt_assist_scene_);
	
	// FLOOR 레이어를 특정 텍스쳐로 깐다.
	ChangeBackTileSheet(_hWnd, pt_assist->GetTextureTag());
	return true;
}

void CTileSetSettingScene::Input(float _time)
{
	CMapToolScene::Input(_time);
}

void CTileSetSettingScene::Update(float _time)
{
	CMapToolScene::Update(_time);
}

void CTileSetSettingScene::LateUpdate(float _time)
{
	CMapToolScene::LateUpdate(_time);
}

void CTileSetSettingScene::Collision(float _time)
{
	CMapToolScene::Collision(_time);
}

void CTileSetSettingScene::Render(HDC _hdc, float _time)
{
	CMapToolScene::Render(_hdc, _time);
}
