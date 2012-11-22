#pragma once

#include "stdafx.h"

template<typename _Tp, typename _TpFather = _Tp>
class _Base_Iterator
{
public:
	inline _Base_Iterator(_Tp* _this, _TpFather* _father) : m_this(_this), m_father(_father)
	{
	}
	inline const bool operator!=(const _Base_Iterator<_Tp, _TpFather> that) const
	{
		return m_this != that.m_this;
	}
	inline virtual _Base_Iterator<_Tp, _TpFather>& operator++() = 0;
	inline _Tp* operator->()
	{
		return (m_this);
	}
	inline _Tp& operator*()
	{
		return *(m_this);
	}
	friend typename _Tp;
protected:
	_Tp* m_this;
	_TpFather* m_father;
};