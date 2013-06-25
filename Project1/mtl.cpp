#include "stdafx.h"
#include "mtl.h"
#include "bmp.h"
#include "tga.h"
#include "dds.h"

void ReadMTL(const char* filename, map<string, Material*>& _Materials)
{
	FILE* fd = fopen(filename, "r");
	char* line = new char[Parameters::MAX_LINE_LENGTH];
	char* prefix = new char[Parameters::MAX_PREFIX_LENGTH];
	char* suffix = new char[Parameters::MAX_SUFFIX_LENGTH];
	char* strName = new char[Parameters::MAX_LINE_LENGTH];

	while (fgets(line, Parameters::MAX_LINE_LENGTH, fd) != NULL)
	{
		sscanf(line, "%s %[^\n]", prefix, line);
		if (strcmp(prefix, "newmtl") == 0)
		{
			GLfloat r, g, b, d;
			Material* material = new Material();
			sscanf(line, "%s", strName);
			_Materials.insert(map<string, Material*>::value_type(strName, material));
			while (fgets(line, Parameters::MAX_LINE_LENGTH, fd) != NULL && strcmp(line, "\n") != 0)
			{
				sscanf(line, "%s %[^\n]", prefix, line);
				if (strcmp(prefix, "d") == 0)
				{
					sscanf(line, "%f", &d);
					material->d = d;
				}
				else if (strcmp(prefix, "Ka") == 0)
				{
					sscanf(line, "%f%f%f", &r, &g, &b);
					material->Ka[0] = r;
					material->Ka[1] = g;
					material->Ka[2] = b;
				}
				else if (strcmp(prefix, "Kd") == 0)
				{
					sscanf(line, "%f%f%f", &r, &g, &b);
					material->Kd[0] = r;
					material->Kd[1] = g;
					material->Kd[2] = b;
				}
				else if (strcmp(prefix, "Ks") == 0)
				{
					sscanf(line, "%f%f%f", &r, &g, &b);
					material->Ks[0] = r;
					material->Ks[1] = g;
					material->Ks[2] = b;
				}
				else if (strcmp(prefix, "Ns") == 0)
				{
					GLfloat shininess;
					sscanf(line, "%f", &shininess);
					material->Ns = shininess;
				}
				else if (strcmp(prefix, "map_Kd") == 0)
				{
					map<string, GLuint>::iterator mapIt;
					GLuint texture;
					sscanf(line, "%s", strName);
					if ((mapIt = Material::m_Textures.find(strName)) != Material::m_Textures.end())
					{
						material->Kd_map = mapIt->second;
					}
					else
					{
						sscanf(strName, "%*[^.].%s", suffix);
						if (strcmp(suffix, "bmp") == 0)
						{
							texture = ReadBMP2TEX(strName);
						}
						else if (strcmp(suffix, "tga") == 0)
						{
							texture = ReadTGA2TEX(strName);
						}
						else if (strcmp(suffix, "dds") == 0)
						{
							texture = ReadDDS2TEX(strName);
						}
						Material::m_Textures[strName] = texture;
						material->Kd_map = texture;
					}
				}
			}
		}
	}
	delete [] line;
	delete [] prefix;
	delete [] strName;
	fclose(fd);
}