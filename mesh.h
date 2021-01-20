#ifndef MESH_INCLUDED_H
#define MESH_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "obj_loader.h"
#include "transform.h"

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	glm::vec3* GetPos() { return &pos; }
	glm::vec2* GetTexCoord() { return &texCoord; }
	glm::vec3* GetNormal() { return &normal; }

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
};

class Mesh
{
public:
	Mesh(const std::string& fileName);
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

	void Draw();
	bool RayIntersectsTriangle(glm::vec3 rayOrigin,
		glm::vec3 rayVector,
		std::vector<unsigned int> indeces,
		glm::vec3& outIntersectionPoint,
		const Transform& transform);
	std::vector<glm::vec3> GetMouseIntersections(glm::vec3 rayOrigin, glm::vec3 rayVector, std::vector<int>& intersectionIndeces, const Transform& transform);
	glm::vec2 CalculateTexturePosition(glm::vec3 intersection, int faceIndex, Transform transform);
	void Barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c, float& u, float& v, float& w);
	glm::vec2 BarycentricToCartesian(glm::vec2 a, glm::vec2 b, glm::vec2 c, float u, float v, float w);

	virtual ~Mesh();
protected:
private:
	static const unsigned int NUM_BUFFERS = 4;
	void operator=(const Mesh& mesh) {}
	Mesh(const Mesh& mesh) {}

	void InitMesh(const IndexedModel& model);

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	const IndexedModel* model;
	unsigned int m_numIndices;
};

#endif
