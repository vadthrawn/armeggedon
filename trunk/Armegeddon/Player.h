#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

#include <Shapes.h>

class Player
{
public:
	static Player *Instance ()
	{
		static Player *player = new Player();
		return player;
	}

	void Init(const b2Vec2 _size, const float& _planetRad, const b2Vec2& _planetPos,  b2World* _world)
	{
		radius = _planetRad;
		angle = 30.0f;

		b2Vec2* vertices = new b2Vec2[4];

		vertices[0] = b2Vec2(-_size.x / 2, radius + _size.y);
		vertices[1] = b2Vec2(-_size.x / 2, radius - _size.y);
		vertices[2] = b2Vec2(_size.x / 2, radius - _size.y);
		vertices[3] = b2Vec2(_size.x / 2, radius + _size.y);

		playerBox = new Polygon(vertices, 4, _planetPos, angle, 1.0f, 0.0f, b2Vec2(0.0f, 0.0f),
			0.0f, Shapes::shapeType::kin, sf::Color::White, 0.0f, 0.0f);

		//box = new Box(size, b2Vec2(_planetPos.x + size.x / 2, _planetPos.y + radius + size.y / 2), angle, 1.0f, 0.0f, b2Vec2(0.0f, 0.0f),
			//0.0f, Shapes::shapeType::kin, sf::Color::White, 0.0f, 0.0f);
		playerBox->Init(_world);
	}

	Polygon* GetPlayerBox()
	{
		return playerBox;
	}

private:
	Player() {};
	Player (Player const&);
	Player& operator=(Player const&);
 
	b2Vec2 size;
	float angle, radius;
	Polygon* playerBox;
};

#endif