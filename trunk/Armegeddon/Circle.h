#ifndef CIRCLE_H
#define CIRCLE_H

#include <Shapes.h>

class Circle : public Shapes
{
public:
	Circle(float _radius, b2Vec2 _pos, b2Vec2 _size, float _angle, float _density,
		float _restitution, b2Vec2 _linVel, float _angVel, shapeType _type, sf::Color _color) :
			Shapes(_pos, _size, _angle, _density, _restitution, _linVel, _angVel, _type, _color)
	{
		radius = _radius;
	}

	void Init(b2World* world)
	{
		Shapes::Init(world);
		body = world->CreateBody(&bodyDef);

		circleShape.m_p.SetZero();
		circleShape.m_radius = radius;

		fixtureDef.shape = &circleShape;
		body->CreateFixture(&fixtureDef);

		shape = new sf::CircleShape;
		shape->setRadius(radius / SCALE);
		shape->setOrigin(radius / SCALE, radius / SCALE);
		shape->setFillColor(color);
	}

	void Draw(sf::RenderWindow &window)
	{
		shape->setPosition(body->GetPosition().x / SCALE, window.getSize().y - (body->GetPosition().y / SCALE));
		shape->setRotation(body->GetAngle() * RADTODEG);

		window.draw(*shape);
	}

protected:
	float radius;
	b2Body* body;
	b2CircleShape circleShape;
	sf::CircleShape* shape;
};

#endif