#ifndef PLAYER_H
#define PLAYER_H

#include <Shapes.h>

class Player
{
public:
	~Player() { }

	static Player &Instance()
	{
		static Player& player = Player();
		return player;
	}

	void Init(const b2Vec2 _size, const float& _planetRad, const b2Vec2& _planetPos,  b2World* _world)
	{
		size = _size;
		b2Vec2* vertices = new b2Vec2[4];

		vertices[0] = b2Vec2(-_size.x / 2, _planetRad + _size.y);
		vertices[1] = b2Vec2(-_size.x / 2, _planetRad);
		vertices[2] = b2Vec2(_size.x / 2, _planetRad);
		vertices[3] = b2Vec2(_size.x / 2, _planetRad + _size.y);

		playerBox = Polygon(vertices, 4, _planetPos, 0.0f, 1.0f, 0.0f, b2Vec2(0.0f, 0.0f),
			0.0f, Shapes::shapeType::kin, sf::Color::White, "Textures/gradius1.png", true, "player");

		delete[] vertices;

		playerBox.Init(_world);
	}

	Polygon& GetPlayerBox()
	{
		return playerBox;
	}

  b2Vec2& GetSize()

  {
    return size;
  }

private:
	Player() {};
	Player (Player const&);
	Player& operator=(Player const&);

	b2Vec2 size;
	Polygon playerBox;
};

#endif