#include "mesh.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
//#include <chrono>

Mesh::Mesh(const std::string& fileName)
{
	model = OBJModel(fileName).ToIndexedModel();

	Mesh::InitMesh(*this->model);
}

void Mesh::InitMesh(const IndexedModel& model)
{
	this->model = &model;
	m_numIndices = model.indices.size();

	//send mesh data to the vertex buffer
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel* model = new IndexedModel();

	for (unsigned int i = 0; i < numVertices; i++)
	{
		model->positions.push_back(*vertices[i].GetPos());
		model->texCoords.push_back(*vertices[i].GetTexCoord());
		model->normals.push_back(*vertices[i].GetNormal());
	}

	for (unsigned int i = 0; i < numIndices; i++)
		model->indices.push_back(indices[i]);
	this->model = model;
	InitMesh(*this->model);
}

Mesh::~Mesh()
{
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);

	//glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);
}


bool Mesh::RayIntersectsTriangle(glm::vec3 rayOrigin,
	glm::vec3 rayVector,
	std::vector<unsigned int> indeces,
	glm::vec3& outIntersectionPoint,
	const Transform& transform)
{
	const float EPSILON = 0.0000001;
		
	glm::mat4 model = transform.GetModel();
		
	glm::vec4 v0(this->model->positions[indeces[0]].x, this->model->positions[indeces[0]].y, this->model->positions[indeces[0]].z, 1);
	glm::vec4 v1(this->model->positions[indeces[1]].x, this->model->positions[indeces[1]].y, this->model->positions[indeces[1]].z, 1);
	glm::vec4 v2(this->model->positions[indeces[2]].x, this->model->positions[indeces[2]].y, this->model->positions[indeces[2]].z, 1);
	v0 = model * v0;
	v1 = model * v1;
	v2 = model * v2;
	glm::vec3 vertex0(v0.x, v0.y, v0.z);
	glm::vec3 vertex1(v1.x, v1.y, v1.z);
	glm::vec3 vertex2(v2.x, v2.y, v2.z);

	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = glm::cross(rayVector, edge2);
	a = glm::dot(edge1, h);

	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.
	f = 1.0 / a;
	s = rayOrigin - vertex0;
	u = f * glm::dot(s, h);

	if (u < 0.0 || u > 1.0)
		return false;
	q = glm::cross(s, edge1);
	v = f * glm::dot(rayVector, q);
	if (v < 0.0 || u + v > 1.0)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * glm::dot(edge2, q);
	if (t > EPSILON) // ray intersection
	{
		outIntersectionPoint = rayOrigin + rayVector * t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
		return false;
}

std::vector<glm::vec3> Mesh::GetMouseIntersections(glm::vec3 rayOrigin, glm::vec3 rayVector, std::vector<int>& intersectionIndeces, const Transform& transform)
{
	std::vector<glm::vec3> intersections;

	int i = 0;
	do {
		glm::vec3 intersection;
		std::vector<unsigned int> indeces;
		indeces.push_back(model->indices[i]);
		indeces.push_back(model->indices[i+1]);
		indeces.push_back(model->indices[i+2]);

		//auto start = std::chrono::high_resolution_clock::now();
		if(RayIntersectsTriangle(rayOrigin,
			rayVector,
			indeces,
			intersection,
			transform))
		{
		intersections.push_back(intersection);
		intersectionIndeces.push_back(i);
		}

		//std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);

		i+=3;
	} while (i < model->indices.size());

	return intersections;
}

glm::vec2 Mesh::CalculateTexturePosition(glm::vec3 intersection, int faceIndex, Transform transform)
{
	//calculate crosssection
	glm::mat4 m = transform.GetModel();
	glm::vec4 v0(model->positions[model->indices[faceIndex]].x, model->positions[model->indices[faceIndex]].y, model->positions[model->indices[faceIndex]].z, 1);
	glm::vec4 v1(model->positions[model->indices[faceIndex+1]].x, model->positions[model->indices[faceIndex+1]].y, model->positions[model->indices[faceIndex+1]].z, 1);
	glm::vec4 v2(model->positions[model->indices[faceIndex+2]].x, model->positions[model->indices[faceIndex+2]].y, model->positions[model->indices[faceIndex+2]].z, 1);
	v0 = m * v0;
	v1 = m * v1;
	v2 = m * v2;
	glm::vec3 a(v0.x, v0.y, v0.z);
	glm::vec3 b(v1.x, v1.y, v1.z);
	glm::vec3 c(v2.x, v2.y, v2.z);
	/*
	glm::vec3 a = this->model->positions[model->indices[faceIndex]];
	glm::vec3 b = this->model->positions[model->indices[faceIndex+1]];
	glm::vec3 c = this->model->positions[model->indices[faceIndex+2]];*/
	glm::vec3 cross = glm::cross(b - a, c - a);

	//do the projection on one of the axis
	glm::vec2 a2, b2, c2, p2;

	if (glm::abs(cross.x) >= glm::abs(cross.y) && glm::abs(cross.x) >= glm::abs(cross.z)) {
		a2 = glm::vec2(a.y, a.z);
		b2 = glm::vec2(b.y, b.z);
		c2 = glm::vec2(c.y, c.z);
		p2 = glm::vec2(intersection.y, intersection.z);
	}
	else if (glm::abs(cross.y) >= glm::abs(cross.x) && glm::abs(cross.y) >= glm::abs(cross.z)) {
		a2 = glm::vec2(a.x, a.z);
		b2 = glm::vec2(b.x, b.z);
		c2 = glm::vec2(c.x, c.z);
		p2 = glm::vec2(intersection.x, intersection.z);
	}
	else {
		a2 = glm::vec2(a.x, a.y);
		b2 = glm::vec2(b.x, b.y);
		c2 = glm::vec2(c.x, c.y);
		p2 = glm::vec2(intersection.x, intersection.y);
	}

	//get barycentric coordinates
	float u, v, w;
	Barycentric(p2, a2, b2, c2, u, v, w);
	
	glm::vec2 tex_coord_a = model->texCoords[model->indices[faceIndex]];
	glm::vec2 tex_coord_b = model->texCoords[model->indices[faceIndex+1]];
	glm::vec2 tex_coord_c = model->texCoords[model->indices[faceIndex+2]];

	glm::vec2 hover_tex_coords = BarycentricToCartesian(tex_coord_a, tex_coord_b, tex_coord_c, u, v, w);

	//std::cout << "x: " << hover_tex_coords.x << " || y: " << hover_tex_coords.y << " [" << faceIndex << "]" << std::endl;

	return hover_tex_coords;
}

void Mesh::Barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c, float& u, float& v, float& w)
{
	glm::vec2 v0 = b - a, v1 = c - a, v2 = p - a;
	float den = v0.x * v1.y - v1.x * v0.y;
	v = (v2.x * v1.y - v1.x * v2.y) / den;
	w = (v0.x * v2.y - v2.x * v0.y) / den;
	u = 1.0f - v - w;
}

glm::vec2 Mesh::BarycentricToCartesian(glm::vec2 a, glm::vec2 b, glm::vec2 c, float u, float v, float w)
{
	glm::vec2 p;

	p.x = a.x * u + b.x * v + c.x * w;
	p.y = a.y * u + b.y * v + c.y * w;

	return p;
}