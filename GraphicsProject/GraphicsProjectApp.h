#pragma once
#include "Shader.h"
#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Planet.h"
#include <vector>
#include "Mesh.h"
#include "OBJMesh.h"


class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void CreatePlanets();

	bool LoadShaderAndMeshLogic();

	void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	

protected:


	//shader
	aie::ShaderProgram m_simpleShader;
	//
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;

	glm::vec4 meshColor = {0,0,0,1};
	glm::vec4 bunnyColor = { 1,1,1,1 };
	std::vector<Planet*> Planets;


	aie::ShaderProgram m_bunnyShader;
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;

	
	
	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
};