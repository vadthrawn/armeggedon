#ifndef CIRCLE_H
#define CIRCLE_H

#include <Shapes.h>

class Circle : public Shapes
{
public:
	~Circle() { }

	Circle() { }

	Circle(float _radius, b2Vec2 _pos, float _angle, float _density, float _restitution, b2Vec2 _linVel,
		float _angVel, shapeType _type, sf::Color _color, char* _textureFile, int _HP, char* _desc) :
			Shapes(_pos, _angle, _density, _restitution, _linVel, _angVel, _type, _color, _textureFile, _HP, _desc)
	{
		circleShape.m_radius = _radius;
	}

	void Init(b2World* world, sf::Time _creationTime)
	{
		Shapes::Init(world, _creationTime);
		body = world->CreateBody(&bodyDef);

		circleShape.m_p.SetZero();

		fixtureDef.shape = &circleShape;
		body->CreateFixture(&fixtureDef);

		shape = sf::CircleShape();
		shape.setRadius(circleShape.m_radius / SCALE);
		shape.setOrigin(circleShape.m_radius / SCALE, circleShape.m_radius / SCALE);
		shape.setFillColor(color);

		if (isTextured)
		{
			shape.setTexture(&texture);

			if (fixtureDef.userData == "planet")
				textureCoordinates = sf::IntRect(0, 0, 133, 134);
			else if (fixtureDef.userData == "debris")
				textureCoordinates = sf::IntRect(0, 0, 128, 128);
		}
	}

	void Draw(sf::RenderWindow* window)
	{
		shape.setPosition(body->GetPosition().x / SCALE, window->getSize().y - (body->GetPosition().y / SCALE));
		shape.setRotation(-(body->GetAngle() * RADTODEG));

		if (isTextured)
			shape.setTextureRect(textureCoordinates);

		window->draw(shape);
	}

	float& GetRadius()
	{
		return circleShape.m_radius;
	}

protected:
	b2CircleShape circleShape;
	sf::CircleShape shape;
};

#endif