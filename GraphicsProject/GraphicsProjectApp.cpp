#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

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
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);
	CreatePlanets();
	return true;
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

	// quit if we press escape
	Gizmos::addSphere(glm::vec3(0, 0, 0), 2.0f, 40, 40, glm::vec4(253.0f / 255.0f, 184.0f / 255.0f, 19.0f / 255.0f, 1.0f));

	for(auto P : Planets)
	{
		P->Update(deltaTime);
		P->Draw();
	}
	
	
	
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

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}