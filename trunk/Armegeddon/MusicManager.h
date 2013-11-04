#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <SFML/Audio.hpp>

class MusicManager
{
public:
	MusicManager()
	{
		
	}

	void SetMusic(char* _fileName)
	{
		music.openFromFile(_fileName);
		music.setLoop(true);
		music.setVolume(50.0f);
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
	sf::Music music;
};

#endif