#include "Stage.h"
#include "Tile.h"
#include "..\Core\Texture.h"
#include "..\Core\CameraManager.h"
#include "..\Core\InputManager.h"
#include "..\Core\TfManager.h"
#include "..\Scene\SceneManager.h"

CStage::CStage()
{
}

CStage::CStage(const CStage& obj)	:
	CStaticObj(obj)
{
	*this = obj;
}

CStage::~CStage()
{
}

bool CStage::Init()
{
	return true;
}

void CStage::Input(float _time)
{
	CStaticObj::Input(_time);
}

void CStage::Update(float _time)
{
	CStaticObj::Update(_time);
}

void CStage::LateUpdate(float _time)
{
	CStaticObj::LateUpdate(_time);
}

void CStage::Collision(float _time)
{
	CStaticObj::Collision(_time);
}

void CStage::Render(HDC _hdc, float _time)
{
	CStaticObj::Render(_hdc, _time);
}
