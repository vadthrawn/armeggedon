#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <Box2D/Box2D.h>

#include <Box.h>
#include <Circle.h>

int main()
{
	b2World* world = new b2World(b2Vec2(0.0f, -9.8f));
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Armageddon", sf::Style::Default);

	std::list<Shapes *> shapeList;
	sf::Clock creationTimer;
	sf::Time createPause = sf::milliseconds(250);
	sf::Time createTime = creationTimer.getElapsedTime();

	Circle* circle;
	Box* movingBox = new Box(b2Vec2(20.0f, 25.0f), b2Vec2(1.0f, 1.0f), 44.0f, 1.0f, 0.25f, b2Vec2(0.0f, 0.0f), 1.0f, Shapes::shapeType::dyn, sf::Color::Red);
	Box* staticBox = new Box(b2Vec2(20.0f, 2.0f), b2Vec2(38.0f, 0.2f), 0.0f, 1.0f, 0.0f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::stat, sf::Color::Green);
	//Circle* circle = new Circle(0.5f, b2Vec2(15.0f, 25.0f), b2Vec2(0.0f, 0.0f), 0.0f, 1.0f, 0.75f, b2Vec2(1.0f, 0.0f), 0.0f, Shapes::shapeType::dyn, sf::Color::Yellow); 

	shapeList.push_front(movingBox);
	//shapeList.push_front(circle);
	shapeList.push_front(staticBox);

	for (std::list<Shapes*>::iterator iter = shapeList.begin(); iter != shapeList.end(); ++iter)
	{
		(*iter)->Init(world);
	}

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (createTime.asMilliseconds() + createPause.asMilliseconds() < sf::Time(creationTimer.getElapsedTime()).asMilliseconds())
			{
				circle = new Circle(0.5f, b2Vec2(sf::Mouse::getPosition(window).x * SCALE, (window.getSize().y - sf::Mouse::getPosition(window).y) * SCALE), b2Vec2(0.0f, 0.0f), 0.0f, 1.0f, 0.75f, b2Vec2(1.0f, 0.0f), 0.0f, Shapes::shapeType::dyn, sf::Color::Yellow);
				circle->Init(world);
				shapeList.push_front(circle);
				createTime = creationTimer.getElapsedTime();
			}
		}

		world->Step((1.f/980.f), 8, 3);
		window.clear(sf::Color::Black);

		for (std::list<Shapes*>::iterator iter = shapeList.begin(); iter != shapeList.end(); ++iter)
		{
			(*iter)->Draw(window);
		}

		window.display();
	}

	return 0;
};