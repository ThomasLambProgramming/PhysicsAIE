#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "imgui.h"
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp() {

}

GraphicsProjectApp::~GraphicsProjectApp() {
	
}

bool GraphicsProjectApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, (float)getWindowWidth() / 
		(float)getWindowHeight(), 0.1f, 1000.0f);
	//CreatePlanets();
	return LoadShaderAndMeshLogic();
}

void GraphicsProjectApp::shutdown() {

	Gizmos::destroy();
}

void GraphicsProjectApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	
	static float colorWheel[4] = {1,1,1,1};
	static float bunnycolorWheel[4] = { 1,1,1,1 };
	ImGui::Begin("Debug");
	ImGui::ColorEdit4("Color", colorWheel);
	ImGui::ColorEdit4("BunnyColor", bunnycolorWheel);

	


	
	ImGui::End();
	meshColor = { colorWheel[0],colorWheel[1],colorWheel[2],colorWheel[3] };
	bunnyColor = { bunnycolorWheel[0], bunnycolorWheel[1],bunnycolorWheel[2],bunnycolorWheel[3] };
	
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}
void GraphicsProjectApp::CreatePlanets()
{
	Planet* Mercury = new Planet(4.0f, 2.1f, 0.5f, glm::vec4(1, 1, 1, 1), glm::vec3(2, 2, 2));
	Planets.push_back(Mercury);

	Planet* Venus = new Planet(5.0f, 1.2f, 0.5f, glm::vec4(1, 1, 1, 1), glm::vec3(2, 2, 2));
	Planets.push_back(Venus);

	Planet* Earth = new Planet(6.5f, 3.3f, 0.5f, glm::vec4(1, 1, 1, 1), glm::vec3(2, 2, 2));
	Planets.push_back(Earth);

	Planet* Mars = new Planet(7.0f, 1.4f, 0.5f, glm::vec4(1, 1, 1, 1), glm::vec3(2, 2, 2));
	Planets.push_back(Mars);

	Planet* Jupiter = new Planet(8.5f, 0.5f, 0.5f, glm::vec4(1, 1, 1, 1), glm::vec3(2, 2, 2));
	Planets.push_back(Jupiter);

	Planet* Saturn = new Planet(9.0f, 0.6f, 0.5f, glm::vec4(1, 1, 1, 1), glm::vec3(2, 2, 2));
	Planets.push_back(Saturn);

	Planet* Uranus = new Planet(10.0f, 0.7f, 0.5f, glm::vec4(1, 1, 1, 1), glm::vec3(2, 2, 2));
	Planets.push_back(Jupiter);

	Planet* Neptune = new Planet(11.0f, 0.8f, 0.5f, glm::vec4(1, 1, 1, 1), glm::vec3(2, 2, 2));
	Planets.push_back(Saturn);
	
}


void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	DrawShaderAndMeshes(m_projectionMatrix, m_viewMatrix);
		
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}
bool GraphicsProjectApp::LoadShaderAndMeshLogic()
{
#pragma  region Quad

	//load the vertex shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	//load the fragment shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_simpleShader.link())
	{
		printf("yeah the fragment or vertex failed good job this didnt load");
		return false;
	}

	//m_quadMesh.InitialiseQuad();
	/*Mesh::Vertex vertices[6];
	vertices[0].position = { -0.5f, 0.f, 0.5f, 1.0f };
	vertices[1].position = { 0.5f, 0.f, 0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0.f,-0.5f, 1.0f };

	vertices[3].position = { -0.5f, 0.f, -0.5f, 1.0f };
	vertices[4].position = { 0.5f, 0.f, 0.5f, 1.0f };
	vertices[5].position = { 0.5f, 0.f,-0.5f, 1.0f };*/

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.f, 0.5f, 1.0f };
	vertices[1].position = { 0.5f, 0.f, 0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0.f,-0.5f, 1.0f };
	vertices[3].position = { 0.5f, 0.f,-0.5f, 1.0f }; 
	unsigned int indices[6] = { 0,1,2,2,1,3 };

	
	m_quadMesh.Initialise(4, vertices, 6 , indices);
	
	m_quadTransform = {
		10,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0,0,1
	};
	
#pragma  endregion 

#pragma region  Bunny
	m_bunnyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	//load the fragment shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_bunnyShader.link())
	{
		printf("yeah the fragment or vertex failed good job this didnt load");
		return false;
	}
	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE bunny didnt load u idiot");
		return false;
	}
	m_bunnyTransform = {
		0.5f,0,0,0,
		0,0.5f,0,0,
		0,0,0.5f,0,
		0,0,0,1
	};
#pragma endregion 
	
	return true;
}

void GraphicsProjectApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
{
	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);
	
	
#pragma  region Quad
	//bind the shader 
	m_simpleShader.bind();
	//bind the transform of the mesh
	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform;	
	m_simpleShader.bindUniform("ProjectionViewModel",pvm);
	m_simpleShader.bindUniform("color", meshColor);
	m_quadMesh.Draw();
#pragma  endregion 

#pragma region Bunny
	m_bunnyShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
	m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
	m_bunnyShader.bindUniform("color", bunnyColor);

	//draw bunny wow this tutorial took alot out of me, im so tired plz just vulkan take me away

	m_bunnyMesh.draw();
#pragma  endregion 
	
	
}