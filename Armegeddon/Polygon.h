#ifndef POLYGON_H
#define POLYGON_H

#include <Shapes.h>

class Polygon : public Shapes
{
public:
	Polygon(b2Vec2 _vertices[], int _vertSize, b2Vec2 _pos, float _angle, float _density,
		float _restitution, b2Vec2 _linVel, float _angVel, shapeType _type, sf::Color _color, float _gravity, float _gravWellRad) : 
			Shapes(_pos, _angle, _density, _restitution, _linVel, _angVel, _type, _color, _gravity, _gravWellRad)
	{
		  vertSize = _vertSize;
      vertices = new b2Vec2[vertSize];

      for (int i = 0; i < vertSize; ++i)
        vertices[i].Set(_vertices[i].x, _vertices[i].y);
	}

	void Init(b2World* world)
	{
		Shapes::Init(world);
		body = world->CreateBody(&bodyDef);

		polyShape.Set(vertices, vertSize);

		fixtureDef.shape = &polyShape;
		body->CreateFixture(&fixtureDef);

		shape = new sf::ConvexShape;
		shape->setFillColor(color);
    shape->setPointCount(vertSize);
    //shape->setOrigin(-vertices[0].x / SCALE, -vertices[0].y / SCALE);

    for (int i = 0; i < vertSize; ++i)
      shape->setPoint(i, sf::Vector2f(vertices[i].x / SCALE, -vertices[i].y / SCALE));
	}

	void Draw(sf::RenderWindow &window)
	{
		shape->setPosition(body->GetPosition().x / SCALE, window.getSize().y - (body->GetPosition().y / SCALE));
		shape->setRotation(body->GetAngle() * RADTODEG);

		window.draw(*shape);
	}

protected:
  int vertSize;
  b2Vec2 size;
  b2Vec2* vertices;
  b2PolygonShape polyShape;
	sf::ConvexShape* shape;
};

#endif