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
		planet = new Circle(planetRad, b2Vec2(20.0f, 15.0f), 0.0f, 1.0f, 0.25f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::stat, sf::Color::White, 0.0f, 25.0f);
		planet->Init(world);

		circle = new Circle(1.0f, b2Vec2(41.0f, 31.0f), 0.0f, 1.0f, 0.25f, b2Vec2((20.0f - 41.0f) * .01f, (15.0f - 31.0f) * .01f), 1.0f, Shapes::shapeType::dyn, sf::Color::Yellow, 0.0f, 0.0f);
		circle->Init(world);

		Player::Instance()->Init(b2Vec2(1.5f, 1.5f), planet->GetRadius(), planet->GetPos(), world);

		shapeList.push_front(planet);
		shapeList.push_front(Player::Instance()->GetPlayerBox());
		shapeList.push_front(circle);

		circle = new Circle(1.0f, b2Vec2(-1.0f, 15.0f), 0.0f, 1.0f, 0.5f, b2Vec2((20.0f - (-1.0f)) * .01f, (15.0f - 15.0f) * .01f), 1.0f, Shapes::shapeType::dyn, sf::Color::Yellow, 0.0f, 0.0f);
		circle->Init(world);
		shapeList.push_front(circle);

		while (window->isOpen())
		{
			sf::Event event;

			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window->close();
			}

			world->Step((1.0f/240.0f), 8, 3);
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
		b2Vec2 getMousePosition = b2Vec2((sf::Mouse::getPosition(*window).x - (window->getSize().x / 2.0f)) * SCALE, (-(sf::Mouse::getPosition(*window).y - (window->getSize().y / 2.0f))) * SCALE); 
		b2Vec2 bulletDirection = getMousePosition;

		float shotAngle = 0.0f;

		if (GetPlayerToClickRadius(getMousePosition.x, getMousePosition.y) != 0)
			shotAngle = acos((pow(GetPlayerToClickRadius(getMousePosition.x, getMousePosition.y), 2)
				+ pow(GetPlayerToClickRadius(getMousePosition.x, getMousePosition.y), 2)
				- pow(GetDistanceFromVectorToClick(getMousePosition.x, getMousePosition.y), 2))
				/ (2 * pow(GetPlayerToClickRadius(getMousePosition.x, getMousePosition.y), 2))) * RADTODEG;

		RotatePosition(GetPlayerX(), GetPlayerY(), (360.0f - GetPlayerAngle()) * DEGTORAD, getMousePosition);

		if (getMousePosition.x < 0)
			shotAngle = -shotAngle;

		if (shotAngle < -70.0f)
			Player::Instance()->GetPlayerBox()->SetTextureMultiplier(0);
		else if (shotAngle < -50.0f)
			Player::Instance()->GetPlayerBox()->SetTextureMultiplier(1);
		else if (shotAngle < -30.0f)
			Player::Instance()->GetPlayerBox()->SetTextureMultiplier(2);
		else if (shotAngle < -10.0f)
			Player::Instance()->GetPlayerBox()->SetTextureMultiplier(3);
		else if (shotAngle < 10.0f)
			Player::Instance()->GetPlayerBox()->SetTextureMultiplier(4);
		else if (shotAngle < 30.0f)
			Player::Instance()->GetPlayerBox()->SetTextureMultiplier(5);
		else if (shotAngle < 50.0f)
			Player::Instance()->GetPlayerBox()->SetTextureMultiplier(6);
		else if (shotAngle < 70.0f)
			Player::Instance()->GetPlayerBox()->SetTextureMultiplier(7);
		else
			Player::Instance()->GetPlayerBox()->SetTextureMultiplier(8);

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (createTime.asMilliseconds() + createPause.asMilliseconds() < sf::Time(creationTimer.getElapsedTime()).asMilliseconds())
				{
					float rad = 0.10f;
					bulletDirection = b2Vec2((bulletDirection.x - GetPlayerX()) / GetPlayerToClickRadius(bulletDirection.x, bulletDirection.y), (bulletDirection.y - GetPlayerY())/ GetPlayerToClickRadius(bulletDirection.x, bulletDirection.y));

					circle = new Circle(rad, b2Vec2((window->getSize().x / 2) * SCALE + GetPlayerX(), (window->getSize().y / 2) * SCALE + GetPlayerY()), 0.0f, 1.0f, 0.25f,
						bulletDirection, 0.0f, Shapes::shapeType::kin, sf::Color::White, 0.0f, 0.0f);

					circle->Init(world);
					shapeList.push_back(circle);
					createTime = creationTimer.getElapsedTime();

					printf("Shot Angle: %f\n", shotAngle);
					printf("MouseRotateX: %f MouseRotateY: %f\n", getMousePosition.x, getMousePosition.y);
					printf("BulletX: %f BulletY: %f\n", bulletDirection.x, bulletDirection.y);
					printf("PlayerX: %f PlayerY: %f\n", GetPlayerX(), GetPlayerY());
					printf("Player Angle: %f\n\n", GetPlayerAngle());
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
		return (DistanceFromPlanetToPlayer() * sin(Player::Instance()->GetPlayerBox()->GetAngle() * DEGTORAD));
	}

	float GetPlayerY()
	{
		return (DistanceFromPlanetToPlayer() * cos(Player::Instance()->GetPlayerBox()->GetAngle() * DEGTORAD));
	}

	float GetPlayerToClickRadius(const float clickX, const float clickY)
	{
		return (sqrt(pow(clickX - GetPlayerX(), 2) + pow(clickY - GetPlayerY(), 2)));
	}

	float GetPlayerAngle()
	{
		return (Player::Instance()->GetPlayerBox()->GetAngle());
	}

	b2Vec2 GetPlayerVector(const float &clickX, const float &clickY)
	{
		return (b2Vec2(GetPlayerX() + (sin(GetPlayerAngle() * DEGTORAD) * GetPlayerToClickRadius(clickX, clickY)),
			GetPlayerY() + (cos(GetPlayerAngle() * DEGTORAD) * GetPlayerToClickRadius(clickX, clickY))));
	}

	float GetDistanceFromVectorToClick(const float &clickX, const float &clickY)
	{
		return (sqrt(pow(GetPlayerVector(clickX, clickY).x - clickX, 2) + pow(GetPlayerVector(clickX, clickY).y - clickY, 2)));
	}

	void RotatePosition(const float &playerX, const float &playerY, const float &angle, b2Vec2 &mousePosition)
	{
		float newX = ((mousePosition.x - playerX) * cos(angle)) - ((playerY - mousePosition.y) * sin(angle));
		float newY = ((playerY - mousePosition.y) * cos(angle)) - ((mousePosition.x - playerX) * sin(angle));

		mousePosition.x = newX; //+ playerX;
		mousePosition.y = -newY; //+ playerY;
	}
};

#endif