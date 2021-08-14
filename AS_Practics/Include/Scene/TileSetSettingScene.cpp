#include "TileSetSettingScene.h"

#include "AssistScene.h"

#include "InGameScene.h"
#include "MapEditScene.h"
#include "SceneManager.h"

#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Tile.h"
#include "..\Object\Stage.h"

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

	// AssistScene���� �ؽ��� �ױ׸� �����´�.
	CAssistScene* pt_assist = static_cast<CAssistScene*>(CSceneManager::Instance()->pt_assist_scene_);
	
	// CTileSetSettingScene�� �� �ؽ��� �ױ� ����
	texture_tag_ = pt_assist->GetTextureTag();

	// FLOOR ���̾ �ش� �ؽ��ķ� ���.
	ChangeBackTileSheet(_hWnd, texture_tag_);

	// UI ���̾ ���
	// vector<CTile*> vec_tile;

	// �������� ������Ʈ �ʱ�ȭ
	CLayer* pt_floor_layer = FindLayer(FLOOR_LAYER);

	CStage* pt_stage = CObject::CreateObj<CStage>("gray_frame", STAGE_CLASS, pt_floor_layer, MY_POSE(0, 0));
	pt_stage->AddTiles(MY_POSE(0, 0), world_size_.x / TEXTURE_SIZE, world_size_.y / TEXTURE_SIZE,
						TEXTURE_SIZE, TEXTURE_SIZE, EMPTY_GRAY_32, TEXTURE_PATH, true);

	pt_floor_layer->PushObj(pt_stage);

	SAFE_RELEASE(pt_stage);

	// : >> OXU �����
	CLayer* pt_ui_layer = FindLayer(UI_LAYER);

	// Ÿ���� Ư���� ���� �ٸ� �ؽ��ĸ� ������ ������Ʈ�� ����� �ʹ�.
	pt_stage = CObject::CreateObj<CStage>("OXT", STAGE_CLASS, pt_ui_layer, MY_POSE(0, 0));
	pt_stage->AddTiles2(MY_POSE(0, 0), world_size_.x / TEXTURE_SIZE, world_size_.y / TEXTURE_SIZE,
		TEXTURE_SIZE, TEXTURE_SIZE, texture_tag_, OXT_32, TEXTURE_PATH, true);



	pt_floor_layer->PushObj(pt_stage);

	SAFE_RELEASE(pt_stage);

	return true;
	// <<
}

void CTileSetSettingScene::Input(float _time)
{
	CMapToolScene::Input(_time);

	// �ȴ����ٰ� �����ٸ�
	if (!CInputManager::Instance()->GetPastMouseLeftDown())
	{
		if (CInputManager::Instance()->GetMouseLeftDown())
		{
			mouse_down_pose_ = CInputManager::Instance()->GetMousePose();

			// ���콺 �ٿ� ��ġ ���� by camera
			MY_POSE mouse_down_pose_cam_ = mouse_down_pose_;
			mouse_down_pose_cam_ += camera_->GetPose();

			// mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 
			mouse_down_pose_cam_ /= TEXTURE_SIZE;
			int mouse_down_pose_x_idx = floor(mouse_down_pose_cam_.x);
			int mouse_down_pose_y_idx = floor(mouse_down_pose_cam_.y);

			// ���� ó��
			if (mouse_down_pose_x_idx >= (world_size_.x / TEXTURE_SIZE) || mouse_down_pose_x_idx < 0)
				return;
			if (mouse_down_pose_y_idx >= (world_size_.y / TEXTURE_SIZE) || mouse_down_pose_y_idx < 0)
				return;


			// ���� ���� ����ִ� �ؽ��ĸ� �����´�.
			CTexture* pt_texture = CSourceManager::Instance()->FindTexture(texture_tag_);

			// �ٲ� ��ġ ã��
			TILE_OPTION tile_option = pt_texture->GetOptionVec(mouse_down_pose_x_idx, mouse_down_pose_y_idx);

			// � ������ �ٲ��� ���� (�ɼ� & �ؽ���)
			string texture_str;
			switch (tile_option)
			{
			case TILE_NONE:
				tile_option = TILE_NOMOVE;
				texture_str = X_32;
				break;
			case TILE_NOMOVE:
				tile_option = TILE_TRANSPORT;
				texture_str = T_32;
				break;
			case TILE_TRANSPORT:
				tile_option = TILE_NONE;
				texture_str = O_32;
				break;
			}

			// �ؽ��� �ٲٱ�
			// UI ���̾��
			CLayer* pt_layer = FindLayer(UI_LAYER);
			if (pt_layer == NULL) return;

			// ������Ʈ�� ���
			CObject* pt_obj = pt_layer->GetObj("OXT");
			if (pt_obj == NULL) return;

			// ������Ʈ �� Ÿ�Ͽ���
			CObject* pt_tile = pt_obj->GetTileFromVec(mouse_down_pose_x_idx + mouse_down_pose_y_idx * (world_size_.x / TEXTURE_SIZE));
			if (pt_tile == NULL) return;
			pt_tile->SetTexture(texture_str);

			// ���� Ÿ�� �ɼ����� �ش� ��ġ Ư�� ����
			pt_texture->SetOptionVec(tile_option, mouse_down_pose_x_idx, mouse_down_pose_y_idx);

			SAFE_RELEASE(pt_tile);
			SAFE_RELEASE(pt_obj);
			SAFE_RELEASE(pt_texture);
		}
	}
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

void CTileSetSettingScene::Save(FILE * _pt_file)
{
	CMapToolScene::Save(_pt_file);
}

void CTileSetSettingScene::Load(FILE * _pt_file)
{
	CMapToolScene::Save(_pt_file);
}
