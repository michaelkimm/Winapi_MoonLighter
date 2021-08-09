#pragma once
#ifndef CREF_H_
#define CREF_H_

class CRef
{
protected:
	CRef();
	virtual ~CRef() = 0;

protected:
	int ref_;

public:
	void AddRef()
	{
		++ref_;
	}

	void InitRef()
	{
		ref_ = 1;
	}

	int Release()
	{
		--ref_;
		if (ref_ == 0)
		{
			delete this;
			return 0;
		}

		return ref_;
	}
};

#endif
