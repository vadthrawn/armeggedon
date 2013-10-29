#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SFML/Audio.hpp>

class SoundManager
{
public:
	SoundManager()
	{
		
	}

	void SetExplosion(char* _fileName)
	{
		buffer.loadFromFile(_fileName);
		explosion.setBuffer(buffer);
		explosion.setVolume(75.0f);
	}

	void SetMusic(char* _fileName)
	{
		music.openFromFile(_fileName);
		music.setLoop(true);
		music.setVolume(50.0f);
	}

	void PlayExplosion()
	{
		explosion.play();
	}

	void StartMusic()
	{
		music.play();
	}

	void StopMusic()
	{
		music.stop();
	}

private:
	sf::Sound explosion;
	sf::SoundBuffer buffer;
	sf::Music music;
};

#endif