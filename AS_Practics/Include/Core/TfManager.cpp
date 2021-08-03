#include "TfManager.h"

DEFINE_SINGLETON(CTfManager)

CTfManager::CTfManager()
{
}

CTfManager::~CTfManager()
{
}

void CTfManager::LoadTf(const string & _tf_name, const MY_POSE & _tf_data)
{
	auto search = wnd_tf_map_.find(_tf_name);
	if (search == wnd_tf_map_.end())
		wnd_tf_map_.insert(make_pair(_tf_name, _tf_data));
	
	return;
}

bool CTfManager::UpdateTf(const string & _tf_name, const MY_POSE & _tf_data)
{
	auto search = wnd_tf_map_.find(_tf_name);
	if (search != wnd_tf_map_.end())
	{
		search->second = _tf_data;
		return true;
	}
	else
		return false;
}

MY_POSE CTfManager::GetTf(const string & _tf_name) const
{
	auto search = wnd_tf_map_.find(_tf_name);
	if (search != wnd_tf_map_.end())
		return search->second;

	return MY_POSE(-1, -1);
}

bool CTfManager::Init()
{
	return false;
}

void CTfManager::Input(float _time)
{
}

void CTfManager::Update(float _time)
{
}

void CTfManager::LateUpdate(float _time)
{
}
