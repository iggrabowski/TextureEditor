#include "controls.h"

Controls::Controls()
{
    this->alreadyScrolled = false;
}

Controls::~Controls()
{
}

Controls::Controls(sf::Window *window, Camera* camera)
{
    this->camera = camera;
    this->window = window;
    this->alreadyScrolled = false;
}

void Controls::Update(Transform& transform, const sf::Event& event)
{
	GetCurrentMousePosition();

    //mouse events
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        glm::vec4 pos4(camera->pos.x, camera->pos.y, camera->pos.z, 1.0);
        //pos4 = glm::rotate((float)mouseLastPostion.x - mouseCurrentPosition.x, glm::vec3(0, 1, 0)) * pos4;
        //pos4 = glm::rotate((float)mouseCurrentPosition.y - mouseLastPostion.y, glm::vec3(1, 0, 0)) * pos4;
        //camera->pos.x = pos4.x;
        //camera->pos.y = pos4.y;
        //camera->pos.z = pos4.z;
        transform.GetRot()->y += mouseCurrentPosition.x - mouseLastPostion.x;
        transform.GetRot()->x += mouseLastPostion.y - mouseCurrentPosition.y;
        //counter += 0.01f;
    }

    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
    {
        // display number of ticks mouse wheel has moved
        camera->pos.z += event.mouseWheelScroll.delta / 10;
    }

	mouseLastPostion = mouseCurrentPosition;
    currentMouseRay = CalculateMouseRay();

    //DEBUG
    //std::cout << currentMouseRay.x << " " << currentMouseRay.y << " " << currentMouseRay.z << " " << std::endl;
}

sf::Vector2i Controls::GetCurrentMousePosition()
{
    return this->mouseCurrentPosition = sf::Mouse::getPosition(*window);
}


sf::Vector2f Controls::GetNormalizedDeviceCoords(sf::Vector2i pos)
{
    sf::Vector2f normCoords;
    normCoords.x = (2.0 * (float)pos.x) / (float)window->getSize().x - 1.0;
    normCoords.y = 1.0 - (2.0 * (float)pos.y) / (float)window->getSize().y;
    return normCoords;
}

glm::vec3 Controls::CalculateMouseRay()
{
    sf::Vector2f coords = GetNormalizedDeviceCoords(mouseCurrentPosition);
    glm::vec4 clipCoords(coords.x, coords.y, 1.0, 1.0);
    glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
    glm::mat4 inversedProjectionMatrix = glm::inverse(projectionMatrix); 
    glm::vec4 eyeCoords = inversedProjectionMatrix * clipCoords;
    glm::vec3 worldCoords = toWorldCoords(eyeCoords);

    return worldCoords;
}

glm::vec3 Controls::toWorldCoords(glm::vec4 eyeCoords)
{
    glm::mat4 inversedViewMatrix = glm::inverse(camera->GetViewMatrix());
    glm::vec4 rayWorld = inversedViewMatrix * eyeCoords;
    glm::vec3 mouseRay(rayWorld.x, rayWorld.y, rayWorld.z);

    mouseRay = glm::normalize(mouseRay);
    return mouseRay;
}

