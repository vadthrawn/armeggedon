#ifndef ARMAGEDDON_H
#define ARMAGEDDON_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <Box2D/Box2D.h>

#include <Box.h>
#include <Circle.h>
#include <Polygon.h>
#include <Player.h>

#include <list>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>

class Armageddon
{
public:

	Armageddon()
	{
		window = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Armageddon", sf::Style::Default); 
		world = new b2World(b2Vec2(0.0f, 0.0f));

		Player::Instance();

		createPause = sf::milliseconds(250);
		createTime = creationTimer.getElapsedTime();

		float planetRad = 2.0f;
		planet = new Circle(planetRad, b2Vec2(20.0f, 15.0f), 0.0f, 1.0f, 0.25f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::stat, sf::Color::White, 1.0f, 30.0f);
		planet->Init(world);

		Player::Instance()->Init(b2Vec2(1.5f, 1.5f), planet->GetRadius(), planet->GetPos(), world);  

		shapeList.push_front(planet);
		shapeList.push_front(Player::Instance()->GetPlayerBox());

		while (window->isOpen())
		{
			sf::Event event;

			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window->close();
			}

			world->Step((1.f/30.f), 8, 3);
			window->clear(sf::Color::Black);

      //Read and take care of any user inputs.
      HandleControls();

			for (std::list<Shapes*>::iterator iter = shapeList.begin(); iter != shapeList.end(); ++iter)
			{
				if ((*iter)->GetType() == Shapes::shapeType::stat)
				{
					for (std::list<Shapes*>::iterator iter2 = shapeList.begin(); iter2 != shapeList.end(); ++iter2)
					{
						if ((*iter2)->GetType() == Shapes::shapeType::dyn)
							(*iter2)->Attracted_To((*iter));
					}
				}

				(*iter)->Draw(window);
			}

		window->display();
		}
	}

private:

    b2World* world;
    Circle* circle, *planet;
    std::list<Shapes *> shapeList;
	sf::Clock creationTimer;
	sf::Time createPause, createTime;
    sf::RenderWindow* window;

    void HandleControls()
    {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (createTime.asMilliseconds() + createPause.asMilliseconds() < sf::Time(creationTimer.getElapsedTime()).asMilliseconds())
				{
					float rad = 1.0f;	

					circle = new Circle(rad, b2Vec2(sf::Mouse::getPosition(*window).x * SCALE, (window->getSize().y - sf::Mouse::getPosition(*window).y) * SCALE), 0.0f, 1.0f, 0.15f,
						b2Vec2(1.0f, 0.0f), 0.0f, Shapes::shapeType::dyn, sf::Color::Yellow, 0.0f, 0.0f);

					circle->Init(world);
					shapeList.push_front(circle);
					createTime = creationTimer.getElapsedTime();

					float clickX = (sf::Mouse::getPosition(*window).x - (window->getSize().x / 2.0f)) * SCALE;
					float clickY = (-(sf::Mouse::getPosition(*window).y - (window->getSize().y / 2.0f))) * SCALE;

					float planet_To_Player = planet->GetRadius() + Player::Instance()->GetSize().y / 2;

					float playerX = DistanceFromPlanetToPlayer() * sin(Player::Instance()->GetPlayerBox()->GetAngle() * DEGTORAD);
					float playerY = DistanceFromPlanetToPlayer() * cos(Player::Instance()->GetPlayerBox()->GetAngle() * DEGTORAD);

					float radius = sqrt(pow(clickX - playerX, 2) + pow(clickY - playerY, 2));

					float playerAngle = Player::Instance()->GetPlayerBox()->GetAngle();

					float playerVecX = playerX + (sin(playerAngle * DEGTORAD) * radius);
					float playerVecY = playerY + (cos(playerAngle * DEGTORAD) * radius);
					float distance_PlayerVec_To_Click = sqrt(pow(playerVecX - clickX, 2) + pow(playerVecY - clickY, 2));
					float shotAngle = acos((pow(radius, 2) + pow(radius, 2) - pow(distance_PlayerVec_To_Click, 2)) / (2 * pow(radius, 2))) * RADTODEG;
          
					printf("Angle: %f\n", shotAngle);
				}
			}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{ 
			Player::Instance()->GetPlayerBox()->SetAngle(-0.1f);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			Player::Instance()->GetPlayerBox()->SetAngle(0.1f);
		}
	}

	float DistanceFromPlanetToPlayer()
	{
		return (planet->GetRadius() + Player::Instance()->GetSize().y / 2.0f);
	}

	float GetPlayerX()
	{

	}

	float GetPlayerY()
	{

	}
};

#endif