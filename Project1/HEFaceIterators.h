#pragma once

#include "stdafx.h"
#include "Iterator.h"
#include "HalfEdge.h"

class _HEFace_Iterator_HEVert : public _Base_Iterator<HEVert, HEEdge>
{
public:
	inline _HEFace_Iterator_HEVert(HEVert* _this, HEEdge* _father) : _Base_Iterator<HEVert, HEEdge>(_this, _father)
	{
	}
	inline virtual _HEFace_Iterator_HEVert& operator++()
	{
		return *(new (this) _HEFace_Iterator_HEVert(m_father->m_next->m_vert, m_father->m_next));
	}
};

class _HEFace_Iterator_HEEdge : public _Base_Iterator<HEEdge, HEFace>
{
public:
	inline _HEFace_Iterator_HEEdge(HEEdge* _this, HEFace* _father) : _Base_Iterator<HEEdge, HEFace>(_this, _father)
	{
	}
	inline virtual _HEFace_Iterator_HEEdge& operator++()
	{
		return *(new (this) _HEFace_Iterator_HEEdge(m_this->m_next, m_father));
	}
};