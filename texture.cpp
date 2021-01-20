#include "texture.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

Texture::Texture(const std::string& fileName)
{
    int width, height, numComponents;
    //unsigned char* data = stbi_load((fileName).c    _str(), &width, &height, &numComponents, 4);

    // load texture
    //sf::Image img_data;
    if (!tex.loadFromFile(fileName))
    {
        std::cout << "Texture not loaded!" << std::endl;
    }
    //tex.loadFromImage(img_data);    

    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    sf::Vector2u texSize = tex.getSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSize.x, texSize.y,
        0, GL_RGBA, GL_UNSIGNED_BYTE, tex.getPixelsPtr());
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_texture);
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::Update(int offsetX, int offsetY, sf::Image img_data)
{
    tex = img_data;
    sf::Vector2u texSize = img_data.getSize();
    Bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texSize.x, texSize.y, GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr());
    //glBindTexture(GL_TEXTURE_2D, 0);
}
