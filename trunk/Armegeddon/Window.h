#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Window
{
public:
	static sf::RenderWindow &Instance ()
	{
		static sf::RenderWindow& window = sf::RenderWindow(sf::VideoMode(800, 600, 32), "Armageddon", sf::Style::Default);
		return window;
	}

private:
	Window() {};
	Window (sf::RenderWindow const&);
	Window& operator=(sf::RenderWindow const&);
};

#endif