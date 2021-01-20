#include "stencil.h"

#include <chrono>

Stencil::Stencil(const std::string& fileName)
{
    if (!tex.loadFromFile(fileName))
    {
        std::cout << "Texture not loaded!" << std::endl;
    }
}

sf::Image Stencil::PutStaple(sf::Image& targetTexture, glm::vec2 position)
{
    //text.tex.update(tex, position.x, position.y);
    /*sf::RenderTexture rt;
    
    stencil.setPosition(position.x, position.y);

    rt.clear();
    rt.create(text.tex.getSize().x, text.tex.getSize().y);
    rt.draw(currentTexture);

    sf::Texture out = rt.getTexture();*/
    //text.tex.copyToImage().saveToFile("./res/output");

    sf::Image targetImage = targetTexture;
    sf::Image stencilImage = tex.copyToImage();

    int offsetX = position.x * targetImage.getSize().x - stencilImage.getSize().x / 2;
    int offsetY = position.y * targetImage.getSize().y - stencilImage.getSize().y / 2;

    for (int i = 0; i < stencilImage.getSize().x; i++) {
        for (int j = 0; j < stencilImage.getSize().y; j++) {
            if (IsPixelInImage(i + offsetX, j + offsetY, targetImage.getSize())) {
                targetImage.setPixel(i + offsetX, j + offsetY, Multiply(targetImage.getPixel(i + offsetX, j + offsetY), stencilImage.getPixel(i, j)));
            }
        }
    }

    return targetImage;
    //stencilImage.pix
}

bool Stencil::IsPixelInImage(int x, int y, sf::Vector2u size)
{
    return x < size.x&& y < size.y&& y >= 0 && x >= 0;
}

sf::Color Stencil::Multiply(sf::Color color1, sf::Color color2)
{
    sf::Color outColor;

    outColor.r = (unsigned int)((float)color1.r / 255.0 * (1.0 - (((float)color2.a) / 255.0) * (1.0 - (float)color2.r / 255.0)) * 255.0);
    outColor.g = (unsigned int)((float)color1.g / 255.0 * (1.0 - (((float)color2.a) / 255.0) * (1.0 - (float)color2.g / 255.0)) * 255.0);
    outColor.b = (unsigned int)((float)color1.b / 255.0 * (1.0 - (((float)color2.a) / 255.0) * (1.0 - (float)color2.b / 255.0)) * 255.0);
    
    outColor.a = color1.a;
    return outColor;
}