#include "Layer.h"
#include "..\Object\Object.h"

CLayer::CLayer() :
	pt_scene_(nullptr), 
	str_tag_(""), 
	z_order_(0)
{
}

CLayer::~CLayer()
{
	SafeReleaseList(obj_list_);
}

void CLayer::AddObj(class CObject* _obj)
{
	_obj->SetScene(pt_scene_);
	_obj->SetLayer(this);
	_obj->AddRef();

	obj_list_.push_back(_obj);
}

bool CLayer::Init()
{
	return true;
}

void CLayer::Input(float _time)
{
	list<class CObject*>::iterator iter;
	list<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Input(_time);
	}
}

void CLayer::Update(float _time)
{
	list<class CObject*>::iterator iter;
	list<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Update(_time);
	}
}

void CLayer::LateUpdate(float _time)
{
	list<class CObject*>::iterator iter;
	list<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->LateUpdate(_time);
	}
}

void CLayer::Collision(float _time)
{
	list<class CObject*>::iterator iter;
	list<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Collision(_time);
	}
}

void CLayer::Render(HDC _hdc, float _time)
{
	list<class CObject*>::iterator iter;
	list<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Render(_hdc, _time);
	}
}
