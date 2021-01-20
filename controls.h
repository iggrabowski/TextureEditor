#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "transform.h"
#include "camera.h"
#include "stencil.h"

class Controls
{
public:
	Controls();
	Controls(sf::Window* window, Camera* camera);
	virtual ~Controls();
	void Update(Transform& transform, const sf::Event& event);
	sf::Vector2f GetNormalizedDeviceCoords(sf::Vector2i pos);
	sf::Vector2i GetCurrentMousePosition();
	glm::vec3 CalculateMouseRay();

	inline glm::vec3 GetMouseRay() {
		return currentMouseRay;
	}

private:
	sf::Window* window;
	sf::Vector2i mouseLastPostion;
	sf::Vector2i mouseCurrentPosition;
	glm::vec3 currentMouseRay;
	Camera* camera;

	bool alreadyScrolled;

	glm::vec3 toWorldCoords(glm::vec4 eyeCoords);
};

