#include "skin.h"
#include <iostream>


skin::skin()
{
	Load("tube_smooth.skin");
	findInverses();
	findSkinning();
}

skin::skin(const char* f, std::vector<Matrices*> m)
{
	Load(f); 
	jointMatrixes = m;
	findInverses();
	findSkinning();
}
skin::~skin()
{
}

void skin::findInverses() 
{
	Matrices temp;
	invertedMatrixes.clear();
	for (int i = 0; i < (int)matrixes.size(); i++) 
	{
		temp = matrixes[i];
		temp.FastInverse();
		invertedMatrixes.push_back(temp);
	}
}

void skin::findSkinning() 
{
	Matrices temp;
	skinningMatrixes.clear();
	for (int i = 0; i < (int)jointMatrixes.size(); i++)
	{
		temp = *jointMatrixes[i];
		temp.Dot(temp, invertedMatrixes[i]);
		skinningMatrixes.push_back(temp);
	}
}

void skin::setMatrixes(std::vector<Matrices*> m) 
{ 
	jointMatrixes = m; 
}

void skin::update() 
{
	Matrices skinningTemp;

	Vector3 currPos;
	Vector3 currNorm;

	bindingVertices.clear();
	bindingNormals.clear();

	findSkinning();

	for (int i = 0; i < (int)vertices.size(); i++)
	{
		Vector3 pos = Vector3(0, 0, 0);
		Vector3 nor = Vector3(0, 0, 0);

		for (int j = 0; j < (int)skinWeights[i].size(); j++)
		{
			skinningTemp = skinningMatrixes[skinWeights[i][j].getIndex()];
			skinningTemp.Transform(vertices[i], currPos);
			skinningTemp.Transform3x3(normals[i], currNorm);

			currPos = currPos * skinWeights[i][j].getWeight();
			currNorm = currNorm * skinWeights[i][j].getWeight();

			pos = pos + currPos;
			nor = nor + currPos;
		}
		bindingVertices.push_back(pos);
		nor.Normalize();
		bindingNormals.push_back(nor);
	}
}

void skin::draw() 
{
	glMatrixMode(GL_MODELVIEW);
	Vector3 temp;
	Vector3 vertex;
	Vector3 normal;

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < (int)triangles.size(); i++) 
	{
		temp = triangles[i];

		vertex = bindingVertices[(int)temp.x];
		normal = bindingNormals[(int)temp.x];

		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(vertex.x, vertex.y, vertex.z);

		vertex = bindingVertices[(int)temp.y];
		normal = bindingNormals[(int)temp.y];

		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(vertex.x, vertex.y, vertex.z);

		vertex = bindingVertices[(int)temp.z];
		normal = bindingNormals[(int)temp.z];

		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
}

bool skin::Load(const char *file)
{
	Parser reader;
	reader.Open(file);

	vertices.clear();
	normals.clear();
	skinWeights.clear();
	triangles.clear();
	matrixes.clear();

	int tokenCount = 0;
	int switchCase;
	Vector3 vertex;

	while (1) 
	{
		char token[256];
		reader.GetToken(token);
		if (strcmp(token, "positions") == 0)
		{
			switchCase = 0;
			tokenCount = reader.GetInt();
			reader.SkipLine();
		}
		else if (strcmp(token, "normals") == 0)
		{
			switchCase = 1;
			tokenCount = reader.GetInt();
			reader.SkipLine();
		}
		else if (strcmp(token, "skinweights") == 0)
		{
			switchCase = 2;
			tokenCount = reader.GetInt();
			reader.SkipLine();
		}
		else if (strcmp(token, "triangles") == 0)
		{
			switchCase = 3;
			tokenCount = reader.GetInt();
			reader.SkipLine();
		}
		else if (strcmp(token, "bindings") == 0)
		{
			switchCase = 4;
			tokenCount = reader.GetInt();
			reader.SkipLine();
		}
		else 
		{
			reader.SkipLine();
			switchCase = 5;
		}

		int boneInfluenceCount;
		Weights boneWeight;
		std::vector<Weights> weightsList;

		Matrices bindMatrix;
		for (int i = 0; i < tokenCount; i++)
		{
			switch (switchCase) 
			{
				case 0:
					vertex.x = reader.GetFloat();
					vertex.y = reader.GetFloat();
					vertex.z = reader.GetFloat();
					vertices.push_back(vertex);
					break;

				case 1:
					vertex.x = reader.GetFloat();
					vertex.y = reader.GetFloat();
					vertex.z = reader.GetFloat();
					normals.push_back(vertex);
					break;

				case 2:
					boneInfluenceCount = (int)reader.GetFloat();
					weightsList.clear();
					for (int j = 0; j < boneInfluenceCount; j++)
					{
						boneWeight.setIndex((int)reader.GetFloat());
						boneWeight.setWeight(reader.GetFloat());
						weightsList.push_back(boneWeight);
					}
					skinWeights.push_back(weightsList);
					break;

				case 3:
					vertex.x = reader.GetFloat();
					vertex.y = reader.GetFloat();
					vertex.z = reader.GetFloat();
					triangles.push_back(vertex);
					break;

				case 4:
					reader.SkipLine();
					bindMatrix.a.x = reader.GetFloat();
					bindMatrix.a.y = reader.GetFloat();
					bindMatrix.a.z = reader.GetFloat();

					reader.SkipLine();
					bindMatrix.b.x = reader.GetFloat();
					bindMatrix.b.y = reader.GetFloat();
					bindMatrix.b.z = reader.GetFloat();
				
					reader.SkipLine();
					bindMatrix.c.x = reader.GetFloat();
					bindMatrix.c.y = reader.GetFloat();
					bindMatrix.c.z = reader.GetFloat();
				
					reader.SkipLine();
					bindMatrix.d.x = reader.GetFloat();
					bindMatrix.d.y = reader.GetFloat();
					bindMatrix.d.z = reader.GetFloat();

					reader.SkipLine(); 
					reader.SkipLine();

					matrixes.push_back(bindMatrix);
					break;
			}
		}
		if (switchCase == 4)
			break;
	}
	reader.Close();
	return true;
}
