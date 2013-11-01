#ifndef ARMAGEDDON_H
#define ARMAGEDDON_H

#include <Box.h>
#include <Circle.h>
#include <Polygon.h>
#include <Player.h>
#include <Window.h>
#include <World.h>
#include <Background.h>
#include <SoundManager.h>

#include <list>

#define _USE_MATH_DEFINES
#include <math.h>

class Armageddon : public b2ContactListener
{
public:
	//Game constructor
	Armageddon()
	{ 
		//Initialize the pre game loop items.
		Init();

		while (Window::Instance().isOpen())
		{
			sf::Event event;

			while (Window::Instance().pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					Window::Instance().close();

				if (event.type == sf::Event::Resized)
					Window::Instance().setSize(sf::Vector2u(800, 600));
			}

			World::Instance().Step((1.0f/240.0f), 8, 3);
			Window::Instance().clear(sf::Color::Black);
			
			//Remove any destroyed bullets and debris
			RemoveDestroyedItems();

			//Read and take care of any user inputs.
			HandleControls();

			//Spawn any debris objects that the game needs.
			CreateDebris();
			
			//Draw the game elements.
			Draw();

			//Check if any bullets are outside the boundries of the game window.  Is so, we recollect the bullet for later use.
			BulletRangeCheck();

			Window::Instance().display();
		}
	}

	//Game destructor
	~Armageddon()
	{
		delete background;
		delete soundManager;

		for (std::vector<Shapes*>::iterator iter = bulletList.begin(); iter != bulletList.end(); iter++)
			delete (*iter);

		bulletList.clear();

		for (std::vector<Shapes*>::iterator iter = debrisList.begin(); iter != debrisList.end(); iter++)
			delete (*iter);

		debrisList.clear();

		for (std::vector<particle*>::iterator iter = particleList.begin(); iter != particleList.end(); iter++)
			delete (*iter);

		particleList.clear();

		for (std::vector<particle*>::iterator iter = planetParticles.begin(); iter != planetParticles.end(); iter++)
			delete (*iter);

		planetParticles.clear();
	}

private:
	//Make a struct for particle effect explosions.
	struct particle
	{
		b2Vec2 position;
		sf::Time deathTime;
		sf::Time creationTime;
		sf::CircleShape shape;
	};

    Circle planet;
    std::vector<Shapes*> bulletList, debrisList;
	std::vector<particle*> particleList, planetParticles;
	sf::Clock currentTimer;
	sf::Time bulletPause, bulletTime, debrisPause, debrisTime;
	sf::Texture explosionTexture;
	b2Vec2 windowSize;
	Background* background;
	SoundManager* soundManager;

	//A function to handle all pregame operations befoer the game loop begins
	void Init()
	{
		//Create the player character
		Player::Instance();
		
		//Seed the random generator.
		srand((unsigned)time(0));
		
		//Set this class to listen for collisions
		World::Instance().SetContactListener(this);
		
		//Create the background;
		background = new Background("Textures/starfield.png");
		
		//Create the sound manager in preparation to use sounds.
		soundManager = new SoundManager();
		soundManager->SetExplosion("Sounds/explosion-01.wav");
		
		//Set the bullet timer and pause between bullet shots
		bulletPause = sf::milliseconds(250);
		bulletTime = currentTimer.getElapsedTime();

		//Set the debris timer and pause between spawning debris
		debrisPause = sf::milliseconds(2000);
		debrisTime = currentTimer.getElapsedTime();
		
		//Create the planet, initialize its values to the world, and set its gravity well radius based on the window size
		planet = Circle(2.0f, b2Vec2((Window::Instance().getSize().x * SCALE) / 2.0f, (Window::Instance().getSize().y * SCALE) / 2.0f), 0.0f, 1.0f, 0.25f, b2Vec2(0.0f, 0.0f),
			0.0f, Shapes::shapeType::stat, sf::Color::White, "Textures/planet.png", 3, "planet");

		planet.Init(&World::Instance());

		planet.SetGravWellRadius(sqrt(pow(planet.GetPosition().x - ((Window::Instance().getSize().x * SCALE) + 1.0f), 2) +
			pow(planet.GetPosition().y - ((Window::Instance().getSize().y * SCALE) + 1.0f), 2)));

		//Initialize the player object in relation to the planet and world.
		Player::Instance().Init(b2Vec2(1.5f, 1.5f), planet.GetRadius(), planet.GetPosition(), &World::Instance());

		//Create 8 bullets and place them in a vector list then initialize the objects.
		for (int i = 0; i < 8; i++)
			bulletList.push_back(new Circle(0.10f, b2Vec2(Window::Instance().getSize().x + 0.40f * i, Window::Instance().getSize().y + 10.0f),
				0.0f, 1.0f, 0.25f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::kin, sf::Color::White, "", 0, "bullet"));

		for (std::vector<Shapes*>::iterator iter = bulletList.begin(); iter != bulletList.end(); iter++)
			(*iter)->Init(&World::Instance());

		//Create the space debris and place them in a vector list then initialize the objects.
		for (int i = 0; i < 1; i++)
			debrisList.push_back(new Circle(1.0f, b2Vec2(Window::Instance().getSize().x + 4.0f * i, Window::Instance().getSize().y + 20.0f),
				0.0f, 1.0f, 0.25f, b2Vec2(0.0f, 0.0f), 0.0f, Shapes::shapeType::dyn, sf::Color::Yellow, "", 0, "debris"));

		for (std::vector<Shapes*>::iterator iter = debrisList.begin(); iter != debrisList.end(); iter++)
			(*iter)->Init(&World::Instance());

		//Create the explosion texture to be used in our explosion particle system.
		explosionTexture.loadFromFile("Textures/explosion.png");
	}

	//A function the handle the controls for the game.
    void HandleControls()
    {
		b2Vec2 getMousePosition = b2Vec2((sf::Mouse::getPosition(Window::Instance()).x - (Window::Instance().getSize().x / 2.0f)) * SCALE, (-(sf::Mouse::getPosition(Window::Instance()).y - (Window::Instance().getSize().y / 2.0f))) * SCALE); 
		b2Vec2 bulletDirection = getMousePosition;

		float shotAngle = 0.0f;

		if (GetPlayerToClickRadius(getMousePosition.x, getMousePosition.y) != 0)
			shotAngle = acos((pow(GetPlayerToClickRadius(getMousePosition.x, getMousePosition.y), 2)
				+ pow(GetPlayerToClickRadius(getMousePosition.x, getMousePosition.y), 2)
				- pow(GetDistanceFromVectorToClick(getMousePosition.x, getMousePosition.y), 2))
				/ (2 * pow(GetPlayerToClickRadius(getMousePosition.x, getMousePosition.y), 2))) * RADTODEG;

		RotatePosition(GetPlayerX(), GetPlayerY(), (360.0f - GetPlayerAngle()) * DEGTORAD, getMousePosition);

		if (getMousePosition.x < 0)
			shotAngle = -shotAngle;

		if (shotAngle < -70.0f)
			Player::Instance().GetPlayerBox().SetTextureMultiplier(0);
		else if (shotAngle < -50.0f)
			Player::Instance().GetPlayerBox().SetTextureMultiplier(1);
		else if (shotAngle < -30.0f)
			Player::Instance().GetPlayerBox().SetTextureMultiplier(2);
		else if (shotAngle < -10.0f)
			Player::Instance().GetPlayerBox().SetTextureMultiplier(3);
		else if (shotAngle < 10.0f)
			Player::Instance().GetPlayerBox().SetTextureMultiplier(4);
		else if (shotAngle < 30.0f)
			Player::Instance().GetPlayerBox().SetTextureMultiplier(5);
		else if (shotAngle < 50.0f)
			Player::Instance().GetPlayerBox().SetTextureMultiplier(6);
		else if (shotAngle < 70.0f)
			Player::Instance().GetPlayerBox().SetTextureMultiplier(7);
		else
			Player::Instance().GetPlayerBox().SetTextureMultiplier(8);

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				//Check to see if the player has recently fired a shot and if the shot is within the 220 degree arc over the player object.
				if (bulletTime.asMilliseconds() + bulletPause.asMilliseconds() < sf::Time(currentTimer.getElapsedTime()).asMilliseconds() && shotAngle >= -110.0f && shotAngle <= 110.0f)
				{
					bulletDirection = b2Vec2((bulletDirection.x - GetPlayerX()) / GetPlayerToClickRadius(bulletDirection.x, bulletDirection.y), (bulletDirection.y - GetPlayerY())/ GetPlayerToClickRadius(bulletDirection.x, bulletDirection.y));

					//Go through the bullet list and see if you can find any bullets that are not being used (isAlive)
					for (std::vector<Shapes*>::iterator iter = bulletList.begin(); iter != bulletList.end(); iter++)
					{
						if (!((*iter)->GetHP() > 0))
						{
							//A bullet has been found.  Set it as alive along with its new position and linear velocity.
							(*iter)->SetHP(1);
							(*iter)->SetLinearVelocity(bulletDirection);
							(*iter)->SetPosition(b2Vec2((Window::Instance().getSize().x / 2) * SCALE + GetPlayerX(), (Window::Instance().getSize().y / 2) * SCALE + GetPlayerY()));

							//Get the current time that the bullet was fired.
							bulletTime = currentTimer.getElapsedTime();
							break;
						}
					}
				}
			}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{ 
			Player::Instance().GetPlayerBox().SetAngle(-0.1f);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			Player::Instance().GetPlayerBox().SetAngle(0.1f);
		}
	}

	//A function to handle drawing the elements in the game window.
	void Draw()
	{
		//Draw the background first.
		background->Draw(&Window::Instance());

		//Check which debris items are alive.  If they are alive, have gravity to the planet affect it and draw the debris.
		for (std::vector<Shapes*>::iterator iter = debrisList.begin(); iter != debrisList.end(); ++iter)
		{
			if ((*iter)->GetHP() > 0)
			{
				(*iter)->Attracted_To(&planet);
				(*iter)->Draw(&Window::Instance());
			}
		}

		//Check which bullets are alive.  If they are alive, draw them.
		for(std::vector<Shapes*>::iterator iter = bulletList.begin(); iter != bulletList.end(); iter++)
		{
			if ((*iter)->GetHP() > 0)
				(*iter)->Draw(&Window::Instance());
		}

		//Draw the player and planet.
		Player::Instance().GetPlayerBox().Draw(&Window::Instance());
		planet.Draw(&Window::Instance());

		//Check to see if any particles need to be destroyed.  Draw any that are not.  The explosion particles will disipate over time.
		//After 2 seconds they will be destroyed.
		for (std::vector<particle*>::iterator iter = particleList.begin(); iter != particleList.end(); )
		{
			if (currentTimer.getElapsedTime().asMilliseconds() > (*iter)->deathTime.asMilliseconds())
			{
				DestroyParticle(iter);
				iter = particleList.begin();
			}

			else
			{
				(*iter)->shape.setFillColor(sf::Color(255, 255, 255, ((((float)(*iter)->deathTime.asMilliseconds() - (float)currentTimer.getElapsedTime().asMilliseconds()) / 2000.0f) * 255)));
				Window::Instance().draw((*iter)->shape, sf::BlendAdd);
				iter++;
			}
		}
		
		//Iterate through the planet particles.  They will fade over time.  When the death time is reached we will recycle the particle
		//giving it a new death time of 3 seconds and a new position to display.
		for (std::vector<particle*>::iterator iter = planetParticles.begin(); iter != planetParticles.end(); iter++)
		{
			if (currentTimer.getElapsedTime().asMilliseconds() > (*iter)->deathTime.asMilliseconds())
			{
				(*iter)->deathTime = currentTimer.getElapsedTime() + sf::milliseconds(3000);
				b2Vec2 newPosition = GetPlanetParticlePosition();
				(*iter)->shape.setPosition(newPosition.x / SCALE, newPosition.y / SCALE);
			}

			(*iter)->shape.setFillColor(sf::Color(255, 255, 255, ((((float)(*iter)->deathTime.asMilliseconds() - (float)currentTimer.getElapsedTime().asMilliseconds()) / 3000.0f) * 255)));
			Window::Instance().draw((*iter)->shape, sf::BlendAdd);
		}
	}

	//A function to recollect bullets after they have either collided with an object of left the window space.
	void CollectBullet(Shapes* bullet, int listPlace)
	{
		bullet->SetHP(0);
		bullet->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		bullet->SetPosition(b2Vec2(Window::Instance().getSize().x + 0.40f * listPlace, Window::Instance().getSize().y + 10.0f));
	}

	//A function to check if the bullets have left the play field.
	void BulletRangeCheck()
	{
		int i = 0;

		for (std::vector<Shapes*>::iterator iter = bulletList.begin(); iter != bulletList.end(); iter++)
		{
			if ((*iter)->GetHP() > 0)
			{
				if ((*iter)->GetPosition().x < -1.0f || (*iter)->GetPosition().x > Window::Instance().getSize().x * SCALE + 1.0f ||
					(*iter)->GetPosition().y < -1.0f || (*iter)->GetPosition().y > Window::Instance().getSize().y * SCALE + 1.0f)
						CollectBullet(*iter, i);
			}

			i++;
		}
	}

	//A function to create a new debris object in the game.
	void CreateDebris()
	{
		if (currentTimer.getElapsedTime().asMilliseconds() > debrisTime.asMilliseconds() + debrisPause.asMilliseconds())
		{
			for (std::vector<Shapes*>::iterator iter = debrisList.begin(); iter != debrisList.end(); iter++)
			{
				if (!((*iter)->GetHP() > 0))
				{
					int random = rand() % 360;
					b2Vec2 debrisPoint = GetPointAroundGravityWell(planet.GetGravWellRadius(), planet.GetPosition(), random);
					(*iter)->SetPosition(debrisPoint);

					b2Vec2 debrisVec = b2Vec2((-(debrisPoint.x - planet.GetPosition().x) / planet.GetGravWellRadius() / 4.0f), (-(debrisPoint.y - planet.GetPosition().y)) / planet.GetGravWellRadius() / 4.0f);
					(*iter)->SetLinearVelocity(debrisVec);

					(*iter)->SetHP(2);
					break;
				}
			}
			
			debrisTime = currentTimer.getElapsedTime();
		}
	}

	//A function to remove any destroyed bullets and debris
	void RemoveDestroyedItems()
	{
		int i = 0;

		for (std::vector<Shapes*>::iterator iter = debrisList.begin(); iter != debrisList.end(); iter++)
		{
			if ((*iter)->IsDestroyed())
			{
				(*iter)->SetHP(0);
				(*iter)->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				(*iter)->SetPosition(b2Vec2(Window::Instance().getSize().x + 4.0f * i, Window::Instance().getSize().y + 20.0f));
				(*iter)->SetIsDestroyed(false);
				soundManager->PlayExplosion();
				break;
			}

			i++;
		}

		i = 0;

		for (std::vector<Shapes*>::iterator iter = bulletList.begin(); iter != bulletList.end(); iter++)
		{
			if ((*iter)->IsDestroyed())
			{
				(*iter)->SetHP(0);
				(*iter)->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				(*iter)->SetPosition(b2Vec2(Window::Instance().getSize().x + 0.40f * i, Window::Instance().getSize().y + 10.0f));
				(*iter)->SetIsDestroyed(false);
				break;
			}

			i++;
		}
	}

	//Getters
	float GetDistanceFromPlanetToPlayer()
	{
		return (planet.GetRadius() + Player::Instance().GetSize().y / 2.0f);
	}

	float GetPlayerX()
	{
		return (GetDistanceFromPlanetToPlayer() * sin(Player::Instance().GetPlayerBox().GetAngle() * DEGTORAD));
	}

	float GetPlayerY()
	{
		return (GetDistanceFromPlanetToPlayer() * cos(Player::Instance().GetPlayerBox().GetAngle() * DEGTORAD));
	}

	float GetPlayerToClickRadius(const float clickX, const float clickY)
	{
		return (sqrt(pow(clickX - GetPlayerX(), 2) + pow(clickY - GetPlayerY(), 2)));
	}

	float GetPlayerAngle()
	{
		return (Player::Instance().GetPlayerBox().GetAngle());
	}

	b2Vec2 GetPlayerVector(const float &clickX, const float &clickY)
	{
		return (b2Vec2(GetPlayerX() + (sin(GetPlayerAngle() * DEGTORAD) * GetPlayerToClickRadius(clickX, clickY)),
			GetPlayerY() + (cos(GetPlayerAngle() * DEGTORAD) * GetPlayerToClickRadius(clickX, clickY))));
	}

	float GetDistanceFromVectorToClick(const float &clickX, const float &clickY)
	{
		return (sqrt(pow(GetPlayerVector(clickX, clickY).x - clickX, 2) + pow(GetPlayerVector(clickX, clickY).y - clickY, 2)));
	}

	//A function to get the point in relation to the player object.
	void RotatePosition(const float &playerX, const float &playerY, const float &angle, b2Vec2 &mousePosition)
	{
		float newX = ((mousePosition.x - playerX) * cos(angle)) - ((playerY - mousePosition.y) * sin(angle));
		float newY = ((playerY - mousePosition.y) * cos(angle)) - ((mousePosition.x - playerX) * sin(angle));

		mousePosition.x = newX; //+ playerX;
		mousePosition.y = -newY; //+ playerY;
	}

	//A function to get the point around the outside edge of the gravity well given angle, radius, and origin point.
	b2Vec2 GetPointAroundGravityWell(const float &_radius, const b2Vec2 &_origin, const int &_angle)
	{
		return (b2Vec2(_origin.x + _radius * sin(_angle * DEGTORAD), _origin.y + _radius * cos(_angle * DEGTORAD)));
	}

	//Box2D overload function to check which fixtures have begun to touch.  This is used for collision detection in the game.
	void BeginContact(b2Contact* contact)
	{
		char* fixtureA = (char*)contact->GetFixtureA()->GetUserData();
		char* fixtureB = (char*)contact->GetFixtureB()->GetUserData();

		//We need to determine if the fixture is a bullet or debris.  As those are the only mobile items in the game it will be one of the other.
		//First we handle if the fixture is a bullet.
		if (fixtureA == "bullet")
		{
			for (std::vector<Shapes*>::iterator iter = bulletList.begin(); iter != bulletList.end(); iter++)
			{
				if ((*iter)->GetBody() == contact->GetFixtureA()->GetBody())
				{
					(*iter)->SetHP((*iter)->GetHP() - 1);
					
					if (!((*iter)->GetHP() > 0))
					{
						(*iter)->SetIsDestroyed(true);

						//Make 6 particles to show the bullet has exploded.
						for (int i = 0; i < 6; i++)
						{
							particleList.push_back(CreateParticle(0.50f, (*iter)->GetPosition(), rand() % 360, currentTimer.getElapsedTime() + sf::milliseconds(2000)));
						}
					}

					break;
				}
			}
		}

		//Handle if the fixture is debris.  We will have to check to see if it hit the player or the planet.
		else if (fixtureA == "debris")
		{
			for (std::vector<Shapes*>::iterator iter = debrisList.begin(); iter != debrisList.end(); iter++)
			{
				if ((*iter)->GetBody() == contact->GetFixtureA()->GetBody())
				{
					(*iter)->SetHP((*iter)->GetHP() - 1);

					//If the debris hit the player, planet, or it's hit points went to to 0 or below, we destory that debris.
					if (!((*iter)->GetHP() > 0) || fixtureB == "planet" || fixtureB == "player")
					{
						(*iter)->SetIsDestroyed(true);

						//Make 6 particles to show the debris has exploded.
						for (int i = 0; i < 6; i++)
						{
							particleList.push_back(CreateParticle(1.50f, (*iter)->GetPosition(), rand() % 360, currentTimer.getElapsedTime() + sf::milliseconds(2000)));
						}

						//Check to see if the debris hit the planet or player.  If it did we deduct 1 hit point from the planet
						//and create explosion effects to show the planet is damaged.
						if (fixtureB == "planet" || fixtureB == "player")
						{
							planet.SetHP(planet.GetHP() - 1);

							for (int i = 0; i < 6; i++)
							{
								planetParticles.push_back(CreateParticle(((float)(planet.GetHP() - 3) / 3.0f), GetPlanetParticlePosition(), rand() % 360, currentTimer.getElapsedTime() + sf::milliseconds(3000)));
							}
						}
					}

					break;
				}
			}
		}

		//Check to see if the fixture is a bullet.  If it is we will destory it leaving explosion effects in its place.
		if (fixtureB == "bullet")
		{
			for (std::vector<Shapes*>::iterator iter = bulletList.begin(); iter != bulletList.end(); iter++)
			{
				if ((*iter)->GetBody() == contact->GetFixtureB()->GetBody())
				{
					(*iter)->SetHP((*iter)->GetHP() - 1);

					if (!((*iter)->GetHP() > 0))
					{
						(*iter)->SetIsDestroyed(true);

						//Make 6 particles to show the bullet has exploded.
						for (int i = 0; i < 6; i++)
						{
							particleList.push_back(CreateParticle(0.50f, (*iter)->GetPosition(), rand() % 360, currentTimer.getElapsedTime() + sf::milliseconds(2000)));
						}
					}

					break;
				}
			}
		}

		//Check to see if the fixture is debris.  If it is we need to check if it is destroyed by hitting the player, planet,
		//or its hit points have reached 0.
		else if (fixtureB == "debris")
		{
			for (std::vector<Shapes*>::iterator iter = debrisList.begin(); iter != debrisList.end(); iter++)
			{
				if ((*iter)->GetBody() == contact->GetFixtureB()->GetBody())
				{
					(*iter)->SetHP((*iter)->GetHP() - 1);

					if (!((*iter)->GetHP() > 0) || fixtureA == "planet" || fixtureA == "player")
					{
						(*iter)->SetIsDestroyed(true);

						//Make 6 particles to show the debris has exploded.
						for (int i = 0; i < 6; i++)
							particleList.push_back(CreateParticle(1.50f, (*iter)->GetPosition(), rand() % 360, currentTimer.getElapsedTime() + sf::milliseconds(2000)));

						//Check to see if the debris hit the player or the planet.  If so deduct 1 hit point from the planet
						//and create explosion particles to show the planet is damaged.
						if (fixtureA == "planet" || fixtureA == "player")
						{
							planet.SetHP(planet.GetHP() - 1);

							for (int i = 0; i < 6; i++)
							{
								planetParticles.push_back(CreateParticle(((float)(planet.GetHP() - 3) / 3.0f), GetPlanetParticlePosition(), rand() % 360, currentTimer.getElapsedTime() + sf::milliseconds(3000)));
							}
						}
					}

					break;
				}
			}
		}
	}

	//A function to create individual explosion particles when something explodes.
	particle* CreateParticle(float _radius, b2Vec2 _position, float _rotation, sf::Time _deathTime)
	{
		particle* p = new particle;
		p->creationTime = currentTimer.getElapsedTime();
		p->deathTime = _deathTime;
		p->position = _position;

		p->shape = sf::CircleShape();
		p->shape.setRadius(_radius / SCALE);
		p->shape.setOrigin(_radius / SCALE, _radius / SCALE);
		p->shape.setRotation(_rotation);
		p->shape.setPosition(_position.x / SCALE, Window::Instance().getSize().y - (_position.y / SCALE));
		
		p->shape.setTexture(&explosionTexture);

		return p;
	}

	//A function to destroy pointers made to individual particles.
	void DestroyParticle(std::vector<particle*>::iterator iter)
	{
		if (currentTimer.getElapsedTime().asMilliseconds() > (*iter)->deathTime.asMilliseconds())
		{
			particle* p;
			p = (*iter);
			particleList.erase(iter);
			delete p;
		}
	}

	//A function to get a new random position within the planet area for the planet explosion particle.
	b2Vec2 GetPlanetParticlePosition()
	{
		float percentWithinPlanetForX = (float)(rand() % 100) / 100.0f;
		float percentWithinPlanetForY;

		if (rand() % 2 > 0)
			percentWithinPlanetForX = -percentWithinPlanetForX;

		float particleXPosition = planet.GetRadius() * percentWithinPlanetForX;
		float particleYPosition = sqrt(pow(planet.GetRadius(), 2) - pow(abs(particleXPosition), 2));

		if (((int)(abs(particleXPosition)) > 0))
			percentWithinPlanetForY = (float)(rand() % (int)(particleXPosition * 100)) / 100.0f;
		else
			percentWithinPlanetForY = (float)(rand() % 100) / 100.0f;

		particleYPosition = particleYPosition * percentWithinPlanetForY;

		if (rand() % 2 > 0)
			particleYPosition = -particleYPosition;

		particleXPosition = ((Window::Instance().getSize().x / 2) * SCALE) + particleXPosition;
		particleYPosition = ((Window::Instance().getSize().y / 2) * SCALE) + particleYPosition;

		return b2Vec2(particleXPosition, particleYPosition);
	}
};

#endif