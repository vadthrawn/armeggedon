#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>

class Background
{

public:
	Background() { }

	Background(char* _file)
	{
		texture.loadFromFile(_file);
		rectShape = sf::RectangleShape();
		rectShape.setSize(sf::Vector2f((float)Window::Instance().getSize().x, (float)Window::Instance().getSize().y));
		rectShape.setPosition(0.0f, 0.0f);
		rectShape.setTexture(&texture);
	}

	void Draw(sf::RenderWindow* window)
	{
		window->draw(rectShape);
	}

private:
	sf::Texture texture;
	sf::RectangleShape rectShape;
};

#endif