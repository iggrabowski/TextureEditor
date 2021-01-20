#pragma once

#include "SFMLWindow.h"

Pine::SFMLWindow::SFMLWindow(const SFMLWindowSettings& settings, const std::string& name)
{
	m_Settings = settings;
	m_Name = name;
}

Pine::SFMLWindow::~SFMLWindow()
{
	delete m_Window;
}

void Pine::SFMLWindow::Init()
{
	m_Window = new sf::RenderWindow(
		sf::VideoMode(m_Settings.width, m_Settings.height),
		m_Name,
		sf::Style::Resize | sf::Style::Close,
		m_Settings.contextSettings
	);
}
