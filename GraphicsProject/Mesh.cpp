#include "Mesh.h"
#include <gl_core_4_4.h>
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}
void Mesh::Draw()
{
	
}
void Mesh::InitialiseQuad()
{
	//check init already has not been run
	assert(vao == 0);

	//make buffer
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0.f, 0.5f, 1.0f };
	vertices[1].position = {  0.5f, 0.f, 0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0.f,-0.5f, 1.0f };

	vertices[3].position = { -0.5f, 0.f, -0.5f, 1.0f };
	vertices[4].position = {  0.5f, 0.f, 0.5f, 1.0f };
	vertices[5].position = { 0.5f, 0.f,-0.5f, 1.0f };

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
}


