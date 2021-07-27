#pragma once
#ifndef COBJECT_H_
#define COBJECT_H_

#include "..\Game.h"
#include "..\Core\Ref.h"
#include "..\Scene\Layer.h"

class CObject
	: public CRef
{
protected:
	string	str_tag_;
	MY_POSE pose_;
	MY_SIZE size_;
	MY_POSE pivot_;

	class CTexture* texture_;

public:
	void SetTag(const string& _str_tag) { str_tag_ = _str_tag; }
	void SetPose(MY_POSE& _pose)		{ pose_ = _pose; }
	void SetPose(float _x, float _y)	{ pose_ = MY_POSE(_x, _y); }
	void SetPose(int _x, int _y)		{ pose_ = MY_POSE(_x, _y); }
	void SetSize(MY_SIZE& _size)		{ size_ = _size; }
	void SetSize(float _x, float _y)	{ size_ = MY_SIZE(_x, _y); }
	void SetSize(int _x, int _y)		{ size_ = MY_SIZE(_x, _y); }
	void SetPivot(MY_POSE& _pivot)		{ pivot_ = _pivot; }
	void SetTexture(class CTexture* _t);
	void SetTexture(const string& _str_key, const wchar_t* _pFileName = NULL, const string& _str_path_key = TEXTURE_PATH);


	string GetTag() const { return str_tag_; }
	MY_POSE GetPose() const { return pose_; }
	MY_SIZE GetSize() const { return size_; }
	MY_POSE GetPivot() const { return pivot_; }
	CTexture* GetTexture() const { return texture_; }

protected:
	class CScene* scene_;
	class CLayer* layer_;

public:
	void	SetScene(class CScene* _scene)	{ scene_ = _scene; }
	void	SetLayer(class CLayer* _layer)	{ layer_ = _layer; }
	CScene* GetScene()	const				{ return scene_; }
	CLayer* GetLayer()	const				{ return layer_; }

protected:
	CObject();
	CObject(const CObject& _obj);
	virtual ~CObject();
	 
public:
	virtual bool Init() = 0;
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

public:
	template <typename T>
	static T* CreateObj(const string& _str_tag, CLayer* _layer)
	{
		T* pt_obj = new T;

		if (!pt_obj->Init())
		{
			SAFE_DELETE(pt_obj);
			return nullptr;
		}

		if (_layer)
		{
			_layer->AddObj(pt_obj);
		}

		// pt_obj->SetTag(_str_tag);

		return pt_obj;
	}
};

#endif


