#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <Box2D/Box2D.h>

#include <Box.h>
#include <Circle.h>
#include <Polygon.h>

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
  
	  std::list<Shapes *> shapeList;
	  sf::Clock creationTimer;
	  sf::Time createPause = sf::milliseconds(250);
	  sf::Time createTime = creationTimer.getElapsedTime();

    Circle* planet = new Circle(2.0f, b2Vec2(20.0f, 15.0f), 0.0f, 1.0f, 0.25f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::stat, sf::Color::White, 5.0f, 30.0f);
    Circle* planet2 = new Circle(1.0f, b2Vec2(10.0f, 5.0f), 0.0f, 1.0f, 0.25f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::stat, sf::Color::White, 2.5f, 15.0f);
    Box* object1 = new Box(b2Vec2(1.0f, 1.0f), b2Vec2(5.0f, 25.0f), 0.0f, 1.0f, 0.25f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::dyn, sf::Color::Red, 0.0f, 0.0f);
    Box* object2 = new Box(b2Vec2(1.0f, 1.0f), b2Vec2(35.0f, 25.0f), 0.0f, 1.0f, 0.5f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::dyn, sf::Color::Blue, 0.0f, 0.0f);
    Circle* object3 = new Circle(1.0f, b2Vec2(10.0f, 5.0f), 0.0f, 1.0f, 0.75f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::dyn, sf::Color::Yellow, 0.0f, 0.0f);
  
    shapeList.push_front(planet);
    shapeList.push_front(planet2);
    shapeList.push_front(object1);
    shapeList.push_front(object2);
    shapeList.push_front(object3);

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

		  world->Step((1.f/980.f), 8, 3);
		  window.clear(sf::Color::Black);

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
};