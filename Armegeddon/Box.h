#ifndef BOX_H
#define BOX_H

#include <Shapes.h>

class Box : public Shapes
{
public:
	Box(b2Vec2 _pos, b2Vec2 _size, float _angle, float _density,
		float _restitution, b2Vec2 _linVel, float _angVel, shapeType _type, sf::Color _color) : 
			Shapes(_pos, _size, _angle, _density, _restitution, _linVel, _angVel, _type, _color)
	{
		
	}

	void Init(b2World* world)
	{
		Shapes::Init(world);
		body = world->CreateBody(&bodyDef);

		polyShape.SetAsBox(size.x / 2, size.y /2);

		fixtureDef.shape = &polyShape;
		body->CreateFixture(&fixtureDef);

		shape = new sf::RectangleShape;
		shape->setSize(sf::Vector2f(size.x / SCALE, size.y / SCALE));
		shape->setOrigin(size.x / SCALE / 2, size.y / SCALE / 2);
		shape->setFillColor(color);
	}

	void Draw(sf::RenderWindow &window)
	{
		shape->setPosition(body->GetPosition().x / SCALE, window.getSize().y - (body->GetPosition().y / SCALE));
		shape->setRotation(body->GetAngle() * RADTODEG);

		window.draw(*shape);
	}

protected:
	std::list<b2Vec2> vecList;
	b2Body* body;
	sf::RectangleShape* shape;
};

#endif