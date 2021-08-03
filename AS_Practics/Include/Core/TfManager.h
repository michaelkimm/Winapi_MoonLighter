#pragma once
#ifndef CTFMANAGER_H_
#define CTFMANAGER_H_

#include "..\Game.h"

class CTfManager
{
	DECLARE_SINGLETON(CTfManager)
private:
	unordered_map<string, MY_POSE> wnd_tf_map_;

public:
	void LoadTf(const string& _tf_name, const MY_POSE& _tf_data);
	bool UpdateTf(const string& _tf_name, const MY_POSE& _tf_data);
	MY_POSE GetTf(const string& _tf_name) const;

public:
	bool Init();
	void Input(float _time);
	void Update(float _time);
	void LateUpdate(float _time);
};

#endif