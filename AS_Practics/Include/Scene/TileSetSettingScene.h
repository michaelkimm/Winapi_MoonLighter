#pragma once
#ifndef CTILESETSETTINGSCENE_H_
#define CTILESETSETTINGSCENE_H_

#include "MapToolScene.h"
#include "SceneManager.h"
#include "..\Game.h"

class CTileSetSettingScene :
	public CMapToolScene
{
	friend class CSceneManager;

private:
	string texture_tag_;

private:
	CTileSetSettingScene();
	~CTileSetSettingScene();

public:
	virtual bool Init(HWND _hWnd);
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);
};

#endif