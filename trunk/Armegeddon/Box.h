#ifndef BOX_H
#define BOX_H

#include <Shapes.h>

class Box : public Shapes
{
public:
	Box(b2Vec2 _size, b2Vec2 _pos, float _angle, float _density, float _restitution, b2Vec2 _linVel,
		float _angVel, shapeType _type, sf::Color _color, char* _textureFile, bool _isAlive) : 
			Shapes(_pos, _angle, _density, _restitution, _linVel, _angVel, _type, _color, _textureFile, _isAlive)
	{
		polyShape.SetAsBox(_size.x / 2, _size.y / 2, b2Vec2(0.0f, -2.5f), 0.0f);

		shape = new sf::RectangleShape;
		shape->setSize(sf::Vector2f(_size.x / SCALE, _size.y / SCALE));
		shape->setOrigin(_size.x / SCALE / 2, _size.y / SCALE / 2);
		shape->setFillColor(_color);
	}

	void Init(b2World* world)
	{
		Shapes::Init(world);
		body = world->CreateBody(&bodyDef);

		fixtureDef.shape = &polyShape;
		body->CreateFixture(&fixtureDef);

		shape->setFillColor(color);
	}

	void Draw(sf::RenderWindow* window)
	{
		shape->setPosition(body->GetPosition().x / SCALE, window->getSize().y - (body->GetPosition().y / SCALE));
		shape->setRotation(angle);

		window->draw(*shape);
	}

	b2Body* GetBody()
	{
		return body;
	}

protected:
	sf::RectangleShape* shape;
	b2PolygonShape polyShape;
};

#endif