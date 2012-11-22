#pragma once

#include "stdafx.h"
#include "Iterator.h"
#include "HEEdge.h"

class _HEVert_Iterator_HEVert : public _Base_Iterator<HEVert, HEEdge>
{
public:
	inline _HEVert_Iterator_HEVert(HEVert* _this, HEEdge* _father) : _Base_Iterator<HEVert, HEEdge>(_this, _father)
	{
	}
	inline virtual _HEVert_Iterator_HEVert& operator++()
	{
		if (m_father->m_pair == NULL)
		{
			return *(new (this) _HEVert_Iterator_HEVert(m_father->m_vert->endVert()));
		}
		return *(new (this) _HEVert_Iterator_HEVert(m_father->m_pair->m_next->m_next->m_vert, m_father->m_pair->m_next));
	}
};

class _HEVert_Iterator_HEEdge : public _Base_Iterator<HEEdge, HEVert>
{
public:
	inline _HEVert_Iterator_HEEdge(HEEdge* _this, HEVert* _father) : _Base_Iterator<HEEdge, HEVert>(_this, _father)
	{
	}
	inline virtual _HEVert_Iterator_HEEdge& operator++()
	{
		return *(new (this) _HEVert_Iterator_HEEdge(m_this->m_pair->m_next, m_father));
	}
};