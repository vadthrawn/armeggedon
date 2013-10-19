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
		counter = 0;
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

		if (type == Shapes::shapeType::kin)
			texture.loadFromFile("Textures/gradius1.png");
	}

	virtual void Draw(sf::RenderWindow* window){}

    void Attracted_To(Shapes* rhs)
    {
		float planetRadius = rhs->GetBody()->GetFixtureList()->GetShape()->m_radius;
		b2Vec2 bodyPos = this->GetBody()->GetPosition();
		b2Vec2 planetPos = rhs->GetPos();

		b2Vec2 planetDistance = b2Vec2(0.0f, 0.0f);
		planetDistance += bodyPos;
		planetDistance -= planetPos;

		float finalDistance = planetDistance.Length();

		if (finalDistance <= planetRadius * rhs->GetGravWellRadius())
		{
			planetDistance.x = planetDistance.x * -1;
			planetDistance.y = planetDistance.y * -1;

			float vecSum = abs(planetDistance.x) + abs(planetDistance.y);
			planetDistance.x = planetDistance.x * ((1 / vecSum) * planetRadius / finalDistance);
			planetDistance.y = planetDistance.y * ((1 / vecSum) * planetRadius / finalDistance);

			this->GetBody()->ApplyForce(planetDistance, this->GetPos());

			//float mod = 1.0f;
			//if (float sum = abs(this->GetBody()->GetLinearVelocity().x) + abs(this->GetBody()->GetLinearVelocity().y) > mod)
			//{
				//this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x * (mod / mod), this->GetBody()->GetLinearVelocity().y * (mod / sum)));
			//}
		}
	}

	b2Body* GetBody()
	{
		return body;
	}

	sf::Color GetColor()
	{
		return color;
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

	void SetAngle(float _angle)
	{
		angle += _angle;

		if (angle > 359.9f)
			angle = 0.00f;
		else if (angle < 0.00f)
			angle = 359.9f;
		
		body->SetTransform(body->GetPosition(), -angle * DEGTORAD);
	}

	float GetAngle()
	{
		return angle;
	}

	Shapes::shapeType& GetType()
	{
		return type;
	}

	b2FixtureDef GetFixture()
	{
		return fixtureDef;
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
	int counter;
};

#endif