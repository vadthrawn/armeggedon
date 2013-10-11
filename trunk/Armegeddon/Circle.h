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
    texX = 0;
    texY = 0;

    count = 0;

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

	void Draw(sf::RenderWindow &window)
	{
		shape->setPosition(body->GetPosition().x / SCALE, window.getSize().y - (body->GetPosition().y / SCALE));
		shape->setRotation(body->GetAngle() * RADTODEG);

    if (count % 20 > 18)
      texX++;

    count++;

    shape->setTextureRect(sf::IntRect(texX, texY, 150, 150));

		window.draw(*shape);
	}

  float GetRadius()
  {
    return radius;
  }

protected:
	float radius;
	b2CircleShape circleShape;
	sf::CircleShape* shape;
  int texX, texY, count, countMax;

  //Temporary variables to simulate gravity
  
};

#endif