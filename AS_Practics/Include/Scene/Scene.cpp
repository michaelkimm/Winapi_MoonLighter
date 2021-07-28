#include "Scene.h"
#include "Layer.h"
#include "..\Core\Core.h"

CScene::CScene()
{
	CLayer* pt_layer = CreateLayer("Background", 0);
	pt_layer = CreateLayer("Monster", 1);
	pt_layer = CreateLayer("Player", 2);
	pt_layer = CreateLayer("UI", INT_MAX);
}

CScene::~CScene()
{
	// 레이어를 전부 동적할당 해제 및 리스트 클리어
	SafeDeleteList(layer_list_);
}

bool CScene::Init()
{
	return true;
}

void CScene::Input(float _time)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Input(_time);
	}
}

void CScene::Update(float _time)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Update(_time);
	}
}

void CScene::LateUpdate(float _time)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iter_end = layer_list_.end();

	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->LateUpdate(_time);
	}
}

void CScene::Collision(float _time)
{
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
