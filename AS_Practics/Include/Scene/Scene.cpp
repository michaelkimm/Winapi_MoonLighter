#include "Scene.h"
#include "Layer.h"
#include "..\Core\Core.h"
#include "..\Core\InputManager.h"

CScene::CScene()
{
	/*CLayer* pt_layer = CreateLayer("Background", 0);
	pt_layer = CreateLayer("Monster", 10);
	pt_layer = CreateLayer("Player", 20);
	pt_layer = CreateLayer("UI", INT_MAX - 1);
	pt_layer = CreateLayer("Assist_scene_mouse_rect", INT_MAX);*/

	CLayer* pt_layer = CreateLayer(FLOOR_LAYER, 0);
	pt_layer = CreateLayer(MAP_OBJ_LAYER, 10);
	pt_layer = CreateLayer(PLAYER_LAYER, 20);
	pt_layer = CreateLayer(UI_LAYER, INT_MAX - 1);
	pt_layer = CreateLayer(MOUSE_RECT_LAYER, INT_MAX);

	camera_ = new CCamera();
}

CScene::~CScene()
{
	// 레이어를 전부 동적할당 해제 및 리스트 클리어
	SafeDeleteList(layer_list_);
	SAFE_DELETE(camera_);
}

bool CScene::Init(HWND _hWnd)
{
	return true;
}

void CScene::Input(float _time)
{
	// 씬의 카메라 업데이트
	if (CInputManager::Instance()->GetHwnd() != hWnd_)
		return;

	camera_->Input(_time);

	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Input(_time);
	}
}

void CScene::Update(float _time)
{
	// 씬의 카메라 업데이트
	camera_->Update(_time);

	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Update(_time);
	}
}

void CScene::LateUpdate(float _time)
{
	// 씬의 카메라 업데이트
	camera_->LateUpdate(_time);

	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->LateUpdate(_time);
	}
}

void CScene::Collision(float _time)
{
	// 씬의 카메라 업데이트
	camera_->Collision(_time);

	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Collision(_time);
	}
}

void CScene::Render(HDC _hdc, float _time)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Render(_hdc, _time);
	}
}

CLayer * CScene::CreateLayer(const string & _str_tag, int _z_order)
{
	CLayer* pt_layer = new CLayer;

	// 속성 설정
	pt_layer->SetScene(this);
	pt_layer->SetTag(_str_tag);
	pt_layer->SetZOrder(_z_order);

	// 리스트에 삽입
	layer_list_.push_back(pt_layer);

	if (layer_list_.size() >= 2)
		layer_list_.sort(CScene::LayerZComp);

	return pt_layer;
}

CLayer * CScene::FindLayer(const string & _str_tag)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		if ((*iter)->GetTag() == _str_tag)
			return (*iter);
	}
	return nullptr;
}

void CScene::SafeDeleteList(list<class CLayer*>& obj)
{
	list<class CLayer*>::iterator iter;
	list<class CLayer*>::iterator iter_end = obj.end();
	for (iter = obj.begin(); iter != iter_end; iter++)
	{
		SAFE_DELETE((*iter));
	}
	obj.clear();
}

bool CScene::LayerZComp(CLayer * p1, CLayer * p2)
{
	return p1->GetZOrder() < p2->GetZOrder();
}

void CScene::Save(FILE * _pt_file)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Save(_pt_file);
	}
}

void CScene::Load(FILE * _pt_file)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Load(_pt_file);
	}
}



