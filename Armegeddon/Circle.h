#ifndef CIRCLE_H
#define CIRCLE_H

#include <Shapes.h>

class Circle : public Shapes
{
public:
	Circle(float _radius, b2Vec2 _pos, float _angle, float _density,
		float _restitution, b2Vec2 _linVel, float _angVel, shapeType _type, sf::Color _color, float _gravity, float _gravWellRad) :
			Shapes(_pos, _angle, _density, _restitution, _linVel, _angVel, _type, _color, _gravity, _gravWellRad)
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

    if (type == Shapes::shapeType::stat)
      shape->setTexture(&texture);
	}

	void Draw(sf::RenderWindow* window)
	{
		shape->setPosition(body->GetPosition().x / SCALE, window->getSize().y - (body->GetPosition().y / SCALE));
		shape->setRotation(angle);

    shape->setTextureRect(sf::IntRect(0, 0, 133, 134));

		window->draw(*shape);
	}

  float GetRadius()
  {
    return radius;
  }

protected:
	float radius;
	b2CircleShape circleShape;
	sf::CircleShape* shape;
};

#endif