#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SFML/Audio.hpp>

class SoundManager
{
public:
	SoundManager()
	{
		
	}

	void SetSound(char* _fileName)
	{
		buffer.loadFromFile(_fileName);
		sound.setBuffer(buffer);
		sound.setVolume(75.0f);
	}

	void SetMusic(char* _fileName)
	{
		music.openFromFile(_fileName);
		music.setLoop(true);
		music.setVolume(50.0f);
	}

	void PlaySound()
	{
		sound.play();
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
	sf::Sound sound;
	sf::SoundBuffer buffer;
	sf::Music music;
};

#endif