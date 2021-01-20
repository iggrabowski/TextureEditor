#pragma once

#include "Core/Window.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

namespace Pine {
	struct SFMLWindowSettings
	{
		sf::ContextSettings contextSettings;
		unsigned int width, height;
		bool VSync;
	};

	class SFMLWindow : Window {
	public:
		SFMLWindow(const SFMLWindowSettings& settings, const std::string& name);
		~SFMLWindow();
		
	private:
		virtual void Init();

	private:
		sf::RenderWindow* m_Window;
		SFMLWindowSettings m_Settings;
		std::string m_Name;
	};

}