#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Rigidbody.h"
#include "PhysicsScene.h"
#include "Texture.h"
#include <vector>

class Sphere;
class Box;
class PhysicsProjectApp : public aie::Application {
public:

	PhysicsProjectApp();
	virtual ~PhysicsProjectApp();

	void PhysicsProjectApp::ImGuiLoad();
	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	void WinCheck();
	virtual void draw();

	/*void DrawRect();
	void SphereAndPlane();
	void SpringTest(int a_amount);
	void TriggerTest();*/
	void Reset();
	void Setup();
	void WinScreen();
	glm::vec2 ScreenToWorld(glm::vec2 a_screenPos);


protected:
	
	int m_blueSunk = 0;
	int m_redSunk = 0;

	aie::Texture m_winScreen;

	bool m_whiteSunk = false;
	bool m_blackSunk = false;
	std::string m_WinMessage;
	bool m_gameFinish = false;
	bool m_canShoot = true;

	bool player1won = false;
	bool player2won = false;

	//same thing as player1red just to keep track of whos turn is whos without
	//using alot of string checks
	bool m_player1sTurn = true;
	//check for if player ones color without using string
	//if true or false we can tell what the other player is 
	bool m_player1Red = false;
	bool m_firstBallSunk = false;
	bool m_shooting = false;
	//this is so the pool cue doesnt shoot quickly
	bool m_hideQ = false;
	bool m_holdingMouse = false;

	//this indicates that the white ball was placed this frame.
	//i wanted to have the set with left click but it would also fire the white ball
	//so a check / timer to not fire had to be placed.
	bool m_setWhiteFrame = false;
	int timer = 0;

	bool turnStarted = false;

	float m_shotPower = 0.0f;
	
	Sphere* m_whiteBall;
	Sphere* m_blackBall;

	glm::vec2 m_mousePos;
	
	bool clickedonce = false;
	bool isMouseDown = false;


	const float m_aspectRatio = 16.0f / 9.0f;
	const float m_extents = 100.0f;
	
	aie::Renderer2D* m_2dRenderer;
	aie::Font*	m_font;
	aie::Texture* winTexture;
	aie::Texture* player1win;
	aie::Texture* player2win;



	bool spawn = false;
	aie::Texture* memeTexture;
	PhysicsScene* m_physicsScene;
};