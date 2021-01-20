#pragma once
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include "texture.h"
#include <iostream>

class Stencil {
public:
	Stencil();
	Stencil(const std::string& fileName);

	sf::Image PutStaple(sf::Image& targetTexture, glm::vec2 position);

private:
	bool IsPixelInImage(int x, int y, sf::Vector2u size);
	sf::Color Multiply(sf::Color color1, sf::Color color2);

	sf::Texture tex;
};