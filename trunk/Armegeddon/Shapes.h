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

	Shapes(b2Vec2 _pos, float _angle, float _density, float _restitution, b2Vec2 _linVel, float _angVel,
		shapeType _type, sf::Color _color, char* _textureFile, bool _isAlive, char* _desc)
	{
		bodyDef.position.Set(_pos.x, _pos.y);
		angle = _angle;
		density = _density;
		restitution = _restitution;
		bodyDef.linearVelocity.Set(_linVel.x, _linVel.y);
		angVel = _angVel;
		type = _type;
		color = _color;
		gravWellRad = 0.0f;
		isAlive = _isAlive;
		desc = _desc;
		isDestroyed = false;
		fixtureDef.userData = desc;

		if (_textureFile != "")
		{
			texture.loadFromFile(_textureFile);
			isTextured = true;
		}

		else
			isTextured = false;
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

		bodyDef.allowSleep = true;
		bodyDef.angle = -angle * DEGTORAD;
		bodyDef.angularVelocity = angVel * DEGTORAD;

		fixtureDef.density = density;
		fixtureDef.restitution = restitution;
	}

	virtual void Draw(sf::RenderWindow* window){}

    void Attracted_To(Shapes* rhs)
    {
		float planetRadius = rhs->GetBody()->GetFixtureList()->GetShape()->m_radius;
		b2Vec2 bodyPos = this->GetBody()->GetPosition();
		b2Vec2 planetPos = rhs->GetPosition();

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

			this->GetBody()->ApplyForce(planetDistance, this->GetPosition());
		}
	}

	//Getters
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

	b2Vec2 GetPosition()
	{
		return body->GetPosition();
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

	bool& IsAlive()
	{
		return isAlive;
	}

	char* GetDesc()
	{
		return desc;
	}

	bool& IsDestroyed()
	{
		return isDestroyed;
	}

	//Setters
	void SetAngle(float _angle)
	{
		angle += _angle;

		if (angle > 359.9f)
			angle = 0.00f;
		else if (angle < 0.00f)
			angle = 359.9f;
		
		body->SetTransform(body->GetPosition(), -angle * DEGTORAD);
	}

	void SetPosition(b2Vec2 _pos)
	{
		body->SetTransform(_pos, 0.0f);
	}

	void SetIsAlive(bool _isAlive)
	{
		isAlive = _isAlive;
	}

	void SetLinearVelocity(b2Vec2 _linVel)
	{
		body->SetLinearVelocity(_linVel);
	}

	void SetGravWellRadius(float _gravWellRad)
	{
		gravWellRad = _gravWellRad;
	}

	void SetIsDestroyed(bool _isDestroyed)
	{
		isDestroyed = _isDestroyed;
	}

protected:
	float angle, density, angVel, restitution, gravity, gravWellRad;
	shapeType type;
	b2BodyDef bodyDef;
	b2Body* body;
	b2FixtureDef fixtureDef;
	sf::Color color;
	sf::Texture texture;
	char* desc;
	bool isAlive, isTextured, isDestroyed;
};

#endif