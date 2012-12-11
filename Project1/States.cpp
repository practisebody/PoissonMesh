#include "stdafx.h"
#include "States.h"

States::States(int _status) : status((name)_status)
{
}

States::operator int()
{
	return status;
}

States::operator char*()
{
	static char strings[3][10] =
	{
		"Translate",
		"Scale",
		"Rotate",
	};
	return strings[status];
}