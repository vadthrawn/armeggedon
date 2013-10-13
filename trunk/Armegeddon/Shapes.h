#ifndef SHAPES_H
#define SHAPES_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <stdio.h>

#define DEGTORAD 0.0174532925f
#define RADTODEG 57.2957795f
#define SCALE .05f

class Shapes
{
public:
	enum shapeType {stat, dyn, kin};

	Shapes() { }

	Shapes(b2Vec2 _pos, float _angle, float _density, float _restitution, b2Vec2 _linVel, float _angVel, shapeType _type, sf::Color _color, float _gravity, float _gravWellRad)
	{
		pos = _pos;
		angle = _angle;
		density = _density;
		restitution = _restitution;
		linVel = _linVel;
		angVel = _angVel;
		type = _type;
		color = _color;
		gravity = _gravity;
		gravWellRad = _gravWellRad;
	}

	virtual void Init(b2World* world)
	{
		switch(type)
		{
		case stat:
			bodyDef.type = b2_staticBody;
			break;
		case dyn:
			bodyDef.type = b2_dynamicBody;
			break;
		case kin:
			bodyDef.type = b2_kinematicBody;
			break;
		}

		bodyDef.position.Set(pos.x, pos.y);
		bodyDef.angle = -angle * DEGTORAD;
		bodyDef.angularVelocity = angVel * DEGTORAD;
		bodyDef.linearVelocity = linVel;

		fixtureDef.density = density;
		fixtureDef.restitution = restitution;

    if (type == Shapes::shapeType::stat)
      texture.loadFromFile("Textures/planet.png");
	}

	virtual void Draw(sf::RenderWindow &window) {}

  void Attracted_To(Shapes* rhs)
  {
    float distBetweenX = rhs->GetBody()->GetPosition().x - body->GetPosition().x;
    float distBetweenY = rhs->GetBody()->GetPosition().y - body->GetPosition().y;
    
    float totalDistance = sqrt(pow(distBetweenX, 2) + pow(distBetweenY, 2));

    if (totalDistance > rhs->GetGravWellRadius())
      distBetweenX = distBetweenY = 0.0f;

    b2Vec2 gravPercent = b2Vec2(distBetweenX / rhs->GetGravWellRadius(), distBetweenY / rhs->GetGravWellRadius());
    b2Vec2 gravForce = b2Vec2(rhs->GetGravity() * gravPercent.x, rhs->GetGravity() * gravPercent.y);
    
    body->ApplyForce(gravForce, rhs->GetBody()->GetWorldCenter());
  }

  b2Body* GetBody()
  {
    return body;
  }

  float& GetGravWellRadius()
  {
    return gravWellRad;
  }

  float& GetGravity()
  {
    return gravity;
  }

  b2Vec2& GetPos()
  {
	  return pos;
  }

  Shapes::shapeType& GetType()
  {
    return type;
  }

protected:
	b2Vec2 pos, linVel;
	float angle, density, angVel, restitution, gravity, gravWellRad;
	shapeType type;
	b2BodyDef bodyDef;
	b2Body* body;
	b2FixtureDef fixtureDef;
	sf::Color color;
	sf::Texture texture;
};

#endif