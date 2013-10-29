#ifndef WORLD_H
#define WORLD_H

#include <Box2D/Box2D.h>

class World
{
public:
	static b2World &Instance ()
	{
		static b2World& world = b2World(b2Vec2(0.0f, 0.0f));
		return world;
	}

private:
	World() {};
	World (b2World const&);
	World& operator=(b2World const&);
};

#endif