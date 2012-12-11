#pragma once

#include "stdafx.h"

class States
{
public:
	States(int _status);
	enum name
	{
		TRANSLATE,
		SCALE,
		ROTATE,
	} status;
	operator int();
	operator char*();
};