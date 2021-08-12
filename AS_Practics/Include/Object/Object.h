#pragma once
#ifndef COBJECT_H_
#define COBJECT_H_

#include "..\Game.h"
#include "..\Core\Ref.h"
#include "..\Scene\Layer.h"

class CObject
	: public CRef
{
	friend class CLayer;

protected:
	string	str_tag_;
	MY_POSE pose_;		// Àý´ë ÁÂÇ¥
	// MY_POSE pose_idx;	// ¸Ê¿¡¼­ ÀÎµ¦½º À§Ä¡
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
	bool SetTexture(class CTexture* _t);
	bool SetTexture(const string& _texture_key, const wchar_t* _pFileName = NULL, const string& _str_path_key = TEXTURE_PATH,
							const Color& _color_key = Color(255, 0, 255));


	string GetTag() const { return str_tag_; }
	MY_POSE GetPose() const { return pose_; }
	MY_SIZE GetSize() const { return size_; }
	MY_POSE GetPivot() const { return pivot_; }
	CTexture* GetTexture() const;

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
	virtual void Save(FILE* _pt_file);
	virtual void Load(FILE* _pt_file);

public:
	template <typename T>
	static T* CreateObj(const string& _str_tag, CLayer* _layer, MY_POSE _pose = MY_POSE(0, 0), bool _sort = false)
	{
		T* pt_obj = new T; 

		if (!pt_obj->Init())
		{
			SAFE_DELETE(pt_obj);
			return nullptr;
		}

		pt_obj->SetPose(_pose);

		if (_layer)
		{
			_layer->AddObj(pt_obj, _sort);
		}

		pt_obj->SetTag(_str_tag);

		return pt_obj;
	}

	static bool CmpObjY(class CObject* a, class CObject *b)
	{
		float y1 = a->GetPose().y;
		float y2 = b->GetPose().y;
		bool result = y1 < y2;
		return !result;
	}
};



#endif


