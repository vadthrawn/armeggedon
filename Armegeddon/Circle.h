#ifndef CIRCLE_H
#define CIRCLE_H

#include <Shapes.h>

class Circle : public Shapes
{
public:
	Circle(float _radius, b2Vec2 _pos, float _angle, float _density, float _restitution, b2Vec2 _linVel,
		float _angVel, shapeType _type, sf::Color _color, char* _textureFile, bool _isAlive, char* _desc) :
			Shapes(_pos, _angle, _density, _restitution, _linVel, _angVel, _type, _color, _textureFile, _isAlive, _desc)
	{
		circleShape.m_radius = _radius;
	}

	void Init(b2World* world)
	{
		Shapes::Init(world);
		body = world->CreateBody(&bodyDef);

		circleShape.m_p.SetZero();

		fixtureDef.shape = &circleShape;
		body->CreateFixture(&fixtureDef);

		shape = new sf::CircleShape;
		shape->setRadius(circleShape.m_radius / SCALE);
		shape->setOrigin(circleShape.m_radius / SCALE, circleShape.m_radius / SCALE);
		shape->setFillColor(color);

		if (isTextured)
			shape->setTexture(&texture);
	}

	void Draw(sf::RenderWindow* window)
	{
		shape->setPosition(body->GetPosition().x / SCALE, window->getSize().y - (body->GetPosition().y / SCALE));
		shape->setRotation(angle);

		shape->setTextureRect(sf::IntRect(0, 0, 133, 134));

		window->draw(*shape);
	}

	float& GetRadius()
	{
		return circleShape.m_radius;
	}

protected:
	b2CircleShape circleShape;
	sf::CircleShape* shape;
};

#endif