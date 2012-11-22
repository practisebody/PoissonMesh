#pragma once

#include "stdafx.h"
#include "Utility.h"

void ReadOBJ(const char* filename, vector<HEVert*>& _Vertices, vector<Point*>& _TextPoints, vector<Vector*>& _NormVectors,
		vector<HEFace*>& _Faces, map<string, Material*>& _Materials);