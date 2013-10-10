#ifndef SHAPES_H
#define SHAPES_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <stdio.h>
#include <list>

#define DEGTORAD 0.0174532925f
#define RADTODEG 57.2957795f
#define SCALE .05f

class Shapes
{
public:
	enum shapeType {stat, dyn, kin};

	Shapes() { }

	Shapes(b2Vec2 _pos, b2Vec2 _size, float _angle, float _density, float _restitution, b2Vec2 _linVel, float _angVel, shapeType _type, sf::Color _color)
	{
		pos = _pos;
		size = _size;
		angle = _angle;
		density = _density;
		restitution = _restitution;
		linVel = _linVel;
		angVel = _angVel;
		type = _type;
		color = _color;
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
		bodyDef.angle = angle * DEGTORAD;
		bodyDef.angularVelocity = angVel * DEGTORAD;
		bodyDef.linearVelocity = linVel;

		fixtureDef.density = density;
		fixtureDef.restitution = restitution;
	}

	virtual void Draw(sf::RenderWindow &window) {}

protected:
	b2Vec2 pos, size, linVel;
	float angle, density, angVel, restitution;
	shapeType type;
	b2BodyDef bodyDef;
	b2PolygonShape polyShape;
	b2FixtureDef fixtureDef;
	sf::Color color;
};

#endif