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
		sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Armageddon", sf::Style::Default); 
		world = new b2World(b2Vec2(0.0f, 0.0f));

		Player::Instance();

		std::list<Shapes *> shapeList;
		sf::Clock creationTimer;
		sf::Time createPause = sf::milliseconds(250);
		sf::Time createTime = creationTimer.getElapsedTime();

		float planetRad = 2.0f;
		Circle* planet = new Circle(planetRad, b2Vec2(20.0f, 15.0f), 0.0f, 1.0f, 0.25f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::stat, sf::Color::White, 5.0f, 30.0f);
		planet->Init(world);

		Player::Instance()->Init(b2Vec2(1.0f, 1.0f), planet->GetRadius(), planet->GetPos(), world);  

		shapeList.push_front(planet);
		shapeList.push_front(Player::Instance()->GetPlayerBox());

		while (window.isOpen())
		{
			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			world->Step((1.f/980.f), 8, 3);
			window.clear(sf::Color::Black);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (createTime.asMilliseconds() + createPause.asMilliseconds() < sf::Time(creationTimer.getElapsedTime()).asMilliseconds())
				{
					float radius = 1.0f;	

					circle = new Circle(radius, b2Vec2(sf::Mouse::getPosition(window).x * SCALE, (window.getSize().y - sf::Mouse::getPosition(window).y) * SCALE), 0.0f, 1.0f, 0.15f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::dyn, sf::Color::Yellow, 0.0f, 0.0f);
					circle->Init(world);
					shapeList.push_front(circle);
					createTime = creationTimer.getElapsedTime();
				}
			}

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

		window.display();
		}
	}

private:

    b2World* world;
    Circle* circle;
};

#endif