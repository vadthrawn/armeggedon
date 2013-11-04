#ifndef POLYGON_H
#define POLYGON_H

#include <Shapes.h>

class Polygon : public Shapes
{
public:
	~Polygon() { }

	Polygon() { }

	Polygon(b2Vec2 _vertices[], int _vertSize, b2Vec2 _pos, float _angle, float _density,
		float _restitution, b2Vec2 _linVel, float _angVel, shapeType _type, sf::Color _color, char* _textureFile, int _HP, char* _desc) : 
			Shapes(_pos, _angle, _density, _restitution, _linVel, _angVel, _type, _color, _textureFile, _HP, _desc)
	{
		vertSize = _vertSize;
		vertices = new b2Vec2[vertSize];
		textureSize = std::vector<int>(17, 18);
		textureMultiplier = 0;

		for (int i = 0; i < vertSize; ++i)
			vertices[i].Set(_vertices[i].x, _vertices[i].y);
	}

	void Init(b2World* world, sf::Time _creationTime)
	{
		Shapes::Init(world, _creationTime);
		body = world->CreateBody(&bodyDef);

		polyShape.Set(vertices, vertSize);

		fixtureDef.shape = &polyShape;
		body->CreateFixture(&fixtureDef);

		shape = sf::ConvexShape();
		shape.setFillColor(color);
		shape.setPointCount(vertSize);

    for (int i = 0; i < vertSize; ++i)
      shape.setPoint(i, sf::Vector2f(vertices[i].x / SCALE, -vertices[i].y / SCALE));

	delete[] vertices;

    if (isTextured)
      shape.setTexture(&texture);
	}

	void Draw(sf::RenderWindow* window)
	{
		shape.setPosition(body->GetPosition().x / SCALE, window->getSize().y - (body->GetPosition().y / SCALE));
		shape.setRotation(-(body->GetAngle() * RADTODEG));

		shape.setTextureRect(sf::IntRect(227 + (24 * textureMultiplier), 63, textureSize.front(), textureSize.back()));

		window->draw(shape);
	}

	sf::ConvexShape& GetShape()
	{
		return shape;
	}

	void SetTextureMultiplier(int _textureMultiplier)
	{
		textureMultiplier = _textureMultiplier;
	}

protected:
	int vertSize, textureMultiplier;
	b2Vec2* vertices;
	b2PolygonShape polyShape;
	sf::ConvexShape shape;
	std::vector<int> textureSize;
};

#endif