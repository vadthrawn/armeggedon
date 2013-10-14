#ifndef BOX_H
#define BOX_H

#include <Shapes.h>

class Box : public Shapes
{
public:
	Box(b2Vec2 _size, b2Vec2 _pos, float _angle, float _density,
		float _restitution, b2Vec2 _linVel, float _angVel, shapeType _type, sf::Color _color, float _gravity, float _gravWellRad) : 
			Shapes(_pos, _angle, _density, _restitution, _linVel, _angVel, _type, _color, _gravity, _gravWellRad)
	{
		  size = _size;
	}

	void Init(b2World* world)
	{
		Shapes::Init(world);
		body = world->CreateBody(&bodyDef);

		polyShape.SetAsBox(size.x / 2, size.y / 2, b2Vec2(0.0f, -2.5f), 0.0f);

		fixtureDef.shape = &polyShape;
		body->CreateFixture(&fixtureDef);

		shape = new sf::RectangleShape;
		shape->setSize(sf::Vector2f(size.x / SCALE, size.y / SCALE));
		shape->setOrigin(size.x / SCALE / 2, size.y / SCALE / 2);
		shape->setFillColor(color);
	}

	void Draw(sf::RenderWindow* window)
	{
		shape->setPosition(body->GetPosition().x / SCALE, window->getSize().y - (body->GetPosition().y / SCALE));
		shape->setRotation(angle);

		window->draw(*shape);
	}

  b2Body* GetBody()
  {
    return body;
  }

protected:
  b2Vec2 size;
	sf::RectangleShape* shape;
  b2PolygonShape polyShape;
};

#endif