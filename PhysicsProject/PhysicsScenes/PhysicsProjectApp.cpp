#include "PhysicsProjectApp.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include <Gizmos.h>
#include "Spring.h"
#include "imgui.h"

PhysicsProjectApp::PhysicsProjectApp()
{

}

PhysicsProjectApp::~PhysicsProjectApp()
{

}

bool PhysicsProjectApp::startup()
{
	// Increases 2D line coun to maximise the number of objects we can draw.
	aie::Gizmos::create(2550U, 2550U, 65535U, 65535U);
	m_2dRenderer = new aie::Renderer2D();
	m_physicsScene = new PhysicsScene();
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);
	memeTexture = new aie::Texture("../bin/textures/glen.png");
	player1win = new aie::Texture("../bin/textures/Player1Win.png");
	player2win = new aie::Texture("../bin/textures/Player2Win.png");

	Setup();
	// Lower the valu, the more accurate the simulation will be;
	// but it will increase the processing time required.
	// If it is too high it causes the sim to stutter and reduce stability.
	m_physicsScene->SetTimeStep(0.01f);
	aie::Application::setBackgroundColour(37.0f / 255.0f, 146.0f / 255.0f, 36.0f / 255.0f, 1.0f);
	return true;
}

void PhysicsProjectApp::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
}

void PhysicsProjectApp::update(float deltaTime)
{
	aie::Gizmos::clear();
	m_physicsScene->Update(deltaTime);
	if (!m_gameFinish)
	{
		//this is because lines overwrite everything and when the game is finished the rotations lines appear
		m_physicsScene->Draw();

		//this was to draw pool cue but i couldnt get the movement working
		//get the mouse position
		aie::Input* input = aie::Input::getInstance();
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		m_mousePos.x = (float)xScreen;
		m_mousePos.y = (float)yScreen;
		glm::vec2 scaledmousepos = ScreenToWorld(m_mousePos);


		//mouse line to ball (cue)
		static float lengthOf = 15;

		static const float initalCueLength = 20.0f;



		//if you swap the scaledmousepos and whiteballpos it makes the q on the opposite side to mouse. might be useful
		glm::vec2 direction = scaledmousepos - m_whiteBall->GetPosition();

		glm::vec2 displacementDirection = glm::normalize(direction);
		glm::vec2 displacementFromSphere = glm::normalize(direction) * lengthOf;

		glm::vec2 sphereDisplacement = displacementFromSphere + m_whiteBall->GetPosition();
		glm::vec2 mouseDisplacement = displacementFromSphere * 23.0f + m_whiteBall->GetPosition();

		//by resetting each frame the loop still checks each ball for its velocity
		//so if it goes all the way through and does not change m_canshoot then we can shoot
		m_canShoot = true;
		for (auto m_actor : m_physicsScene->GetActors())
		{
			Sphere* sphere1 = dynamic_cast<Sphere*>(m_actor);
			if (sphere1 == nullptr)
				continue;

			if (glm::length(sphere1->GetVelocity()) > 1.0f)
				m_canShoot = false;

		}

		if (m_canShoot && !m_whiteSunk)
		{
			m_hideQ = false;
		}
		if (m_whiteSunk && m_canShoot)
		{
			if (scaledmousepos.x > -45.0f)
				scaledmousepos.x = -45.0f;

			if (scaledmousepos.x < -85.0f)
				scaledmousepos.x = -85.0f;

			if (scaledmousepos.y > 45.0f)
				scaledmousepos.y = 45.0f;

			if (scaledmousepos.y < -45.0f)
				scaledmousepos.y = -45.0f;


			bool canPlace = true;
			m_whiteBall->SetPosition(scaledmousepos);
			for (auto m : m_physicsScene->GetActors())
			{
				if (m == m_whiteBall)
					continue;

				Sphere* sphere1 = dynamic_cast<Sphere*>(m);

				if (sphere1 != nullptr)
				{
					float dist = glm::distance(sphere1->GetPosition(), m_whiteBall->GetPosition());
					float penetration = sphere1->GetRadius() + m_whiteBall->GetRadius() - dist;
					if (penetration > 0)
					{
						canPlace = false;
					}
				}
			}

			if (input->isMouseButtonDown(0) && canPlace == true)
			{
				m_whiteBall->SetTrigger(false);
				m_whiteSunk = false;
				m_setWhiteFrame = true;
			}
		}

		if (!m_hideQ && !m_whiteSunk && m_canShoot)
		{
			aie::Gizmos::add2DLine(mouseDisplacement, sphereDisplacement, { 1,1,1,1 });
		}

		if (m_setWhiteFrame)
		{
			timer++;
			if (timer > 15)
			{
				m_setWhiteFrame = false;
				timer = 0;
				m_hideQ = false;
			}
		}
		if (input->isMouseButtonDown(0) && m_shooting == false && m_holdingMouse == false && m_setWhiteFrame == false && m_canShoot == true && !m_whiteSunk)
		{
			m_shooting = true;
			m_holdingMouse = true;
		}

		if (m_shooting)
		{
			if (m_holdingMouse)
			{
				m_shotPower += 20.0f;
				if (lengthOf < 40.0f)
				{
					lengthOf += 0.3f;

				}
			}

			if (m_holdingMouse == false)
			{
				lengthOf /= 2;
				if (lengthOf <= 2)
				{
					if (m_shotPower > 2000.0f)
						m_shotPower = 2000.0f;

					m_whiteBall->ApplyForce(-displacementDirection * m_shotPower, m_whiteBall->GetPosition() + displacementDirection * m_whiteBall->GetRadius() + 4.0f);
					lengthOf = 15;
					m_shooting = false;
					m_hideQ = true;
					m_shotPower = 0.0f;
					m_player1sTurn = !m_player1sTurn;
				}
			}
		}
		if (m_blackSunk == true)
		{
			m_gameFinish = true;
			if (m_player1sTurn)
				std::cout << "Player ones turn" << std::endl;
			else
				std::cout << "Player Twos turn" << std::endl;


			bool allVelocityCheck = true;
			for (auto actor : m_physicsScene->GetActors())
			{
				Sphere* sphere1 = dynamic_cast<Sphere*>(actor);
				if (sphere1 == nullptr)
					continue;

				if (glm::length(sphere1->GetVelocity()) > 1.0f)
					allVelocityCheck = false;
			}
			if (allVelocityCheck == true && m_whiteSunk == true)
			{
				if (m_player1sTurn)
				{
					m_WinMessage = "Player Two wins, Player One sunk the white with black";
					player2won = true;
				}
				else
				{
					m_WinMessage = "Player One wins, Player Two sunk the white with black";
					player1won = true;
				}
			}
			if (allVelocityCheck == true && m_whiteSunk == false)
			{
				//check for which player is what color
				//check color amount (if not correct color then lost)
				//if players color they win 
				bool updatedThisFrame = false;
				if (m_player1sTurn == true)
				{
					if (m_redSunk == 0 && m_blueSunk == 0)
					{
						player2won = true;
						updatedThisFrame = true;
					}
					if (m_player1Red && m_redSunk == 7)
					{
						player1won = true;
						updatedThisFrame = true;
					}
					if (!m_player1Red && m_blueSunk == 7)
					{
						player1won = true;
						updatedThisFrame = true;
					}
					if (m_player1Red && m_redSunk < 7)
					{
						player2won = true;
						updatedThisFrame = true;
					}
					if (!m_player1Red && m_blueSunk < 7)
					{
						player2won = true;
						updatedThisFrame = true;
					}
				}
				if (m_player1sTurn == false)
				{
					if (m_redSunk == 0 && m_blueSunk == 0)
					{
						player1won = true;
						updatedThisFrame = true;
					}
					if (!m_player1Red && m_redSunk == 7)
					{
						player2won = true;
						updatedThisFrame = true;
					}
					if (m_player1Red && m_blueSunk == 7)
					{
						player2won = true;
						updatedThisFrame = true;
					}
					if (m_player1Red && m_blueSunk < 7)
					{
						player1won = true;
						updatedThisFrame = true;
					}
					if (!m_player1Red && m_blueSunk < 7)
					{
						player1won = true;
						updatedThisFrame = true;
					}
				}

			}
		}
		if (input->isMouseButtonUp(0))
			m_holdingMouse = false;
		aie::Gizmos::add2DLine(glm::vec2(-40, 50), glm::vec2(-40, -50), glm::vec4(0, 0, 0, 1));
		if (input->isKeyDown(aie::INPUT_KEY_RIGHT_BRACKET))
			spawn = true;
		if (input->isMouseButtonDown(2))
		{
			Reset();
			Setup();
		}
	}
	if (m_gameFinish)
	{
		if (player1won)
			winTexture = player1win;
		else
			winTexture = player2win;
		WinScreen();
	}
	
}

void PhysicsProjectApp::WinScreen()
{	
	ImGui::SetNextWindowPos(ImVec2(530, 400));
	ImGui::SetNextWindowSize(ImVec2(220, 240));
	ImGui::Begin("Debug");
	if (ImGui::Button("Reset", ImVec2(200.f, 200.f)))
	{
		Reset();
		Setup();
	}
	ImGui::End();
}

void PhysicsProjectApp::Reset()
{
	delete m_physicsScene;
	m_physicsScene = new PhysicsScene();
	m_whiteSunk = false;
	m_blackSunk = false;
	m_gameFinish = false;
	m_canShoot = true;
	player1won = false;
	player2won = false;
	m_player1sTurn = false;
	m_player1Red = false;
	m_shooting = false;
	m_hideQ = false;
	m_holdingMouse = false;
	m_setWhiteFrame = false;
	clickedonce = false;
	isMouseDown = false;
	spawn = false;
}
void PhysicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	// begin drawing sprites
	m_2dRenderer->begin();
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.0f, 1.0f));
	if (m_gameFinish)
		m_2dRenderer->drawSprite(winTexture,1280 /2,720 /2, 1280, 720);

	if (spawn)
		m_2dRenderer->drawSprite(memeTexture, 1280 / 2, 720 / 2, 1280, 720);
	char fps[32];
	sprintf_s(fps, 32, "fps: %i", getFPS());
	
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->end();
}


void PhysicsProjectApp::Setup()
{
	//6 spheres for each pocket, 4 boxes for collisions
	//22 balls one white 1 black and apparently other ones but ill just say 10 and 10 for stripes and colors

	m_whiteBall = new Sphere(glm::vec2(-50, 0), glm::vec2(0, 0), 6, 4, glm::vec4(1, 1, 1, 1));
	m_whiteBall->SetElasticity(1);
	m_whiteBall->SetDebug(true);
	m_physicsScene->AddActor(m_whiteBall);
	
	{
		Box* temp = new Box(glm::vec2(0, -55), glm::vec2(0, 0), 0, 10, 95, 5);
		temp->SetKinematic(true);
		temp->SetElasticity(1.0f);
		m_physicsScene->AddActor(temp);
	}
	{
		Box* temp = new Box(glm::vec2(0, 55), glm::vec2(0, 0), 0, 10, 95, 5);
		temp->SetKinematic(true);
		temp->SetElasticity(1.0f);
		m_physicsScene->AddActor(temp);
	}
	{
		Box* temp = new Box(glm::vec2(100, 0), glm::vec2(0, 0), 0, 10, 5, 85);
		temp->SetKinematic(true);
		temp->SetElasticity(1.0f);
		m_physicsScene->AddActor(temp);
	}
	{
		Box* temp = new Box(glm::vec2(-100, 0), glm::vec2(0, 0), 0, 10, 5, 85);
		temp->SetKinematic(true);
		temp->SetElasticity(1.0f);
		m_physicsScene->AddActor(temp);
	}

	for (int i = 0; i < 6; i++)
	{
		Sphere* temp;
		if (i >= 3)
			temp = new Sphere(glm::vec2(-95 + (i - 3) * 95, -50), glm::vec2(0, 0), 2, 6, glm::vec4(1, 0, 0, 1));
		else
			temp = new Sphere(glm::vec2(-95 + i * 95, 50), glm::vec2(0, 0), 2, 6, glm::vec4(1, 0, 0, 1));

		temp->SetKinematic(true);
		temp->SetTrigger(true);
		temp->triggerEnter = [&](PhysicsObject* other)
		{
			if (other == m_whiteBall)
			{
				m_whiteSunk = true;
				m_whiteBall->SetTrigger(true);
				m_whiteBall->SetPosition(glm::vec2(200.0f, 200.0f));
				m_whiteBall->SetVelocity(glm::vec2(0, 0));
				m_whiteBall->SetRotation(0.0f);
				m_whiteBall->SetAngularVel(0.0f);
				return;
			}

			if (other->GetShapeID() != BOX)
			{
				Sphere* pSphere = dynamic_cast<Sphere*>(other);
				if (pSphere == nullptr)
					return;

				glm::vec4 tempColor = pSphere->GetColour();
				if (tempColor == glm::vec4(0, 0, 0, 1))
				{
					std::cout << "Black Sunk" << std::endl;

					m_blackSunk = true;
				}
				else if (tempColor == glm::vec4(1, 0, 0, 1))
				{
					std::cout << "Red Sunk" << std::endl;
					if (m_redSunk == 0 && m_blueSunk == 0)
					{
						if (!m_player1sTurn == true)
							m_player1Red = true;
						else
							m_player1Red = false;
					}
					m_redSunk++;
				}
				else
				{
					std::cout << "Blue Sunk" << std::endl;
					if (m_redSunk == 0 && m_blueSunk == 0)
					{
						if (!m_player1sTurn == true)
							m_player1Red = true;
						else
							m_player1Red = false;
					}
					m_blueSunk++;
				}
				m_physicsScene->RemoveActor(other);
			}


		};
		temp->SetColor(glm::vec4(1, 1, 1, 1));
		m_physicsScene->AddActor(temp);
	}


	int xOffset = 9;
	int yOffset = 9;
	for (int i = 0; i < 6; i++)
	{
		float yStart = -5 + yOffset * i * 0.5f;
		int z = 0;
		while (z < i)
		{
			glm::vec4 color = { 0,0,0,1 };
			if (i % 2 == 0)
				color = glm::vec4(0, 0, 1, 1);
			else
				color = glm::vec4(1, 0, 0, 1);
			
			if (i == 3 && z == 1)
			{
				color = glm::vec4(0, 0, 0, 1);
				Sphere* temp = new Sphere(glm::vec2(30 + xOffset * i, yStart - yOffset * z), glm::vec2(0, 0), 25, 4, color);
				temp->SetElasticity(1);
				m_physicsScene->AddActor(temp);
				m_blackBall = temp;
				m_blackBall->SetDebug(true);
				z++;
				continue;
			}
			if (i == 5 && z == 2)
			{
				color = glm::vec4(0, 0, 1, 1);
			}
			
			Sphere* temp = new Sphere(glm::vec2(30 + xOffset * i, yStart - yOffset * z), glm::vec2(0,0), 5, 4, color);
			temp->SetElasticity(1);
			temp->SetDebug(true);
			m_physicsScene->AddActor(temp);
			z++;
		}
	}



}

glm::vec2 PhysicsProjectApp::ScreenToWorld(glm::vec2 a_screenPos)
{
	glm::vec2 worldPos = a_screenPos;

	//we will move the center of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	//scale this according to extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}

///*void PhysicsProjectApp::DrawRect()
//{
//	Sphere* ball = new Sphere(glm::vec2(-70, -10), glm::vec2(0, 0), 1, 3, glm::vec4(1, 0, 0, 1));
//	ball->SetElasticity(0.5f);
//	ball->SetKinematic(true);
//	m_physicsScene->AddActor(ball);
//
//
//
//	m_physicsScene->AddActor(new Plane(glm::vec2(0.1f, 0.6f), -25));
//
//	/*Box* box1 = new Box(glm::vec2(-20, 0), glm::vec2(16, -4), 1, 4, 8, 4, glm::vec4(1, 0, 0, 1));
//	box1->SetRotation(0.5f);
//	m_physicsScene->AddActor(box1);*/
//
//	
//
//	
//}
//
//void PhysicsProjectApp::SphereAndPlane()
//{
//	Sphere* ball1;
//	ball1 = new Sphere(glm::vec2(-40, 40), glm::vec2(0), 3.0f, 1, glm::vec4(1, 0, 0, 1));
//	m_physicsScene->AddActor(ball1);
//	ball1->ApplyForce(glm::vec2(50, 30), glm::vec2(0));
//
//	Sphere* ball2;
//	ball2 = new Sphere(glm::vec2(40, 40), glm::vec2(0), 3.0f, 1, glm::vec4(0, 1, 0, 1));
//	m_physicsScene->AddActor(ball2);
//	ball2->ApplyForce(glm::vec2(-50, 30), glm::vec2(0));
//
//	Plane* plane = new Plane();
//	m_physicsScene->AddActor(plane);
//}
//
//void PhysicsProjectApp::SpringTest(int a_amount)
//{
//	Sphere* prev = nullptr;
//	for (int i = 0; i < a_amount; i++)
//	{
//		//this will need to spawn the sphere at the bottom of the previous one,
//		//to make a pendulum that is effected by gravity
//		Sphere* sphere = new Sphere(glm::vec2(i * 3, 30 - i * 5), glm::vec2(0), 10, 2, glm::vec4(0, 1, 0, 1));
//
//		if (i == 0)
//		{
//			sphere->SetKinematic(true);
//		}
//		
//		m_physicsScene->AddActor(sphere);
//		
//		if (prev)
//		{
//			m_physicsScene->AddActor(new Spring(sphere, prev, 10, 500));
//		}
//		prev = sphere;
//	}
//	Box* box = new Box(glm::vec2(0, -20), glm::vec2(0), 0.3f, 20, 8, 2);
//	box->SetKinematic(true);
//	m_physicsScene->AddActor(box);
//}
//
//
//void PhysicsProjectApp::TriggerTest()
//{
//	Sphere* ball1 = new Sphere(glm::vec2(40, 0), glm::vec2(-10,0), 4, 4, glm::vec4(1, 0, 0, 1));
//	Sphere* ball2 = new Sphere(glm::vec2(10, -20), glm::vec2(0), 4, 4, glm::vec4(0, 0, 1, 1));
//	ball2->SetKinematic(true);
//	ball2->SetTrigger(true);
//
//
//	m_physicsScene->AddActor(ball1);
//	m_physicsScene->AddActor(ball2);
//	m_physicsScene->AddActor(new Plane(glm::vec2(0,1),-30));
//	m_physicsScene->AddActor(new Plane(glm::vec2(1,0), -50));
//	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -50));
//	m_physicsScene->AddActor(new Box(glm::vec2(20,10),glm::vec2(40,0),0.5,4,8,4));        
//	m_physicsScene->AddActor(new Box(glm::vec2(-40, 10), glm::vec2(10, 0), 0.5, 4, 8, 4));
//
//	ball2->triggerEnter = [=](PhysicsObject* other) {std::cout << "REEEEEEEEEEEEEEEEEEEEEEEEE   " << other << std::endl; };
//	ball2->triggerExit = [=](PhysicsObject* other) {std::cout << "OOOOOOOOOOOOOOOOOOOOOOOOOO   " << other << std::endl; };
//}
//
//*/
