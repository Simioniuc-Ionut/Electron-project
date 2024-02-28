#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
using namespace sf;
class Button {
public:
	Button() { }
	Button(std::string t, sf::Vector2f size, int charSize, sf::Color bgColor, sf::Color textColor)
	{

		text.setString(t);
		text.setCharacterSize(charSize);

		button.setSize(size);
		button.setFillColor(bgColor);
	}
	void setFont(sf::Font& font) {
		text.setFont(font);
	}
	void setBackColor(sf::Color color){
		button.setFillColor(color);
	}
	void setTextColor(sf::Color color) {
		text.setFillColor(color);
	}
	void setCharacterSize(int size) {
		text.setCharacterSize(size);
	}
	void setPosition(sf::Vector2f pos) {
		button.setPosition(pos);

		float xPos = (pos.x + button.getGlobalBounds().width/2)-(text.getGlobalBounds().width/2);
		float yPos = (pos.y + button.getGlobalBounds().height / 2) - (text.getGlobalBounds().width / 2);
		text.setPosition({ xPos-100,yPos-25 });
	}
	void setTextPos(sf::Vector2f pos) {

		text.setPosition({ pos.x,pos.y});
	}
	void drawTo(sf::RenderWindow &window) {
		window.draw(button);
		window.draw(text);

	}
	bool isMouseOver(sf::RenderWindow& window) {
		float mouseX = sf::Mouse::getPosition(window).x;
		float mouseY = sf::Mouse::getPosition(window).y;

		float btnPosX = button.getPosition().x;
		float btnPosY = button.getPosition().y;

		float btnxPosWidth = button.getPosition().x + button.getLocalBounds().width;
		float btnyPosHeight = button.getPosition().y + button.getLocalBounds().height;

		if (mouseX < btnxPosWidth && mouseX>btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
			return true;
		}
		return false;
	}
	void setOutlineColor(sf::Color color, int thickness) {
		button.setOutlineColor(color);
		button.setOutlineThickness(thickness);
	}
private:
	sf::RectangleShape button;
	sf::Text text;
};