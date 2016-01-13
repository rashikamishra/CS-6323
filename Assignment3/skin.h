#ifndef SKIN_H
#define SKIN_H
#endif

#ifndef PARSER_H
#include "Parser.h"
#endif
#ifndef VECTOR3_H
#include "vector3.h"
#endif

#include <vector>
#include "Weight.h"

#ifndef SKELETON_H
#include "skeleton.h"
#endif

#ifndef MATRICES_H
#include "Matrices.h"
#endif

class skin
{
	private:

		std::vector<Vector3> vertices = std::vector<Vector3>();
		std::vector<Vector3> normals = std::vector<Vector3>();

		std::vector<Vector3> bindingVertices = std::vector<Vector3>();
		std::vector<Vector3> bindingNormals = std::vector<Vector3>();

		std::vector<std::vector<Weights>> skinWeights = std::vector<std::vector<Weights>>();

		std::vector<Vector3> triangles = std::vector<Vector3>();

		std::vector<Matrices> matrixes = std::vector<Matrices>();

		std::vector<Matrices> invertedMatrixes = std::vector<Matrices>();

		std::vector<Matrices> skinningMatrixes = std::vector<Matrices>();

		std::vector<Matrices*> jointMatrixes = std::vector<Matrices*>();

	public:

		skin();

		void findInverses();

		void findSkinning();

		void setMatrixes(std::vector<Matrices*> m);

		skin(const char *f, std::vector<Matrices*> m);

		void update();

		void draw();

		~skin();

		bool Load(const char *file);
};
