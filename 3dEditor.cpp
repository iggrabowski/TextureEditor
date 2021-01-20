//#define GLEW_STATIC
#include <GL/glew.h>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "obj_loader.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "controls.h"
#include "stencil.h"

#define ARGV

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;

int main(int argc, char** argv)
{
	if (argc != 4) {
		std::cout 
			<< "ERROR: Invalid number of arguments.\n\n"
			<< "Make sure that the program is executed from the command line with correct number of argumens.\n\n"
			<< "Example usage:\n\n"
				<< "\t3dEditor.exe sample\\monkey.obj sample\\bricks.jpg sample\\brush.png\n"
			<<  std::endl;

		system("pause");
		return 1;
	}

	// Configure OpenGL Context through SFML
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2;
	settings.majorVersion = 4;
	settings.minorVersion = 0;
	settings.attributeFlags = sf::ContextSettings::Default;

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(DISPLAY_WIDTH, DISPLAY_HEIGHT), "OpenGL", sf::Style::Resize | sf::Style::Close, settings);
	window.setVerticalSyncEnabled(true);

	settings = window.getSettings();

	std::cout << "depth bits:" << settings.depthBits << std::endl;
	std::cout << "stencil bits:" << settings.stencilBits << std::endl;
	std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
	std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;

	// start glew
	//glewExperimental = GL_TRUE;
	glewInit();


	//Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	Mesh mesh(argv[1]);			//"./res/monkey3.obj"
	Texture texture(argv[2]);	//"res/epic5.jpg"
	Stencil staple(argv[3]);	//"res/arrow.png"


	/* DEBUG */
	/*Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0)),
	};

	unsigned int indices[] = { 0, 1, 2,
							  0, 2, 3,

							  6, 5, 4,
							  7, 6, 4,

							  10, 9, 8,
							  11, 10, 8,

							  12, 13, 14,
							  12, 14, 15,

							  16, 17, 18,
							  16, 18, 19,

							  22, 21, 20,
							  23, 22, 20
	};

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));*/
	/*Mesh mesh("./res/monkey.obj");	
	Texture texture("res/epic5.jpg");
	Stencil staple("res/arrow.png");*/


	Shader shader("res/basicShader");
	Transform transform;
	Camera camera(glm::vec3(0.0f, 0.0f, -2.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f);
	Controls controls(&window, &camera);

	bool isRunning = true;
	float counter = 0.0f;
	sf::Event event;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	///*-----------INITIALIZE UI-----------*/

	//sf::Font font;
	//if (!font.loadFromFile("./fonts/default.ttf"))
	//    std::cout << "FONT NOT LOADED" << std::endl;

	//sf::RectangleShape buttonRec(sf::Vector2f(100, 30));
	//buttonRec.setFillColor(sf::Color(100, 250, 50));
	//buttonRec.setPosition(sf::Vector2f(100, 100));


	//texture.Bind();

	while (isRunning)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// end the program
				isRunning = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape)
					isRunning = false;
				break;
			}
		}

		//making sure context is active
		window.setActive();

		// clear the buffers
		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//run controls update method
		controls.Update(transform, event);

		transform.UpdateModel();
		//transform.PrintModel();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			//get intersections with mouse ray
			std::vector<int> intersectionIndeces;
			std::vector<glm::vec3> intersections = mesh.GetMouseIntersections(camera.pos, controls.GetMouseRay(), intersectionIndeces, transform);

			//chose the closest hit
			float closestDistance = 0;
			int closestIntersectionIndex = 0;
			int index = 0;
			for (int i = 0; i < intersections.size(); i++) {
				//std::cout << "x: " << intersections[i].x << " || y: " << intersections[i].y << "|| z: " << intersections[i].z << std::endl;
				if (glm::distance(intersections[i], camera.pos) < closestDistance || i == 0) {
					closestDistance = glm::distance(intersections[i], camera.pos);
					closestIntersectionIndex = intersectionIndeces[i];
					index = i;
				}
			}

			if (intersections.size() > 0) {

				//Save file from stapled texture
				glm::vec2 texture_position = mesh.CalculateTexturePosition(intersections[index], closestIntersectionIndex, transform);
				//std::cout << "x: " << texture_position.x << " || y: " << texture_position.y << std::endl;
				texture.Update(texture_position.x, texture_position.y, staple.PutStaple(texture.tex, texture_position));
			}

		}

		//DEBUG
		//std::cout << intersections.size() << std::endl;

		shader.Bind();

		shader.Update(transform, camera);

		mesh.Draw();

		//--------------- SFML DRAWING ---------------//
		/*window.pushGLStates();


		window.draw(buttonRec);


		window.popGLStates();*/
		//--------------- SFML DRAWING ---------------//

		window.display();

		sf::sleep(sf::seconds(0.02f));
	}


	texture.tex.saveToFile("./output/image.png");

	window.close();

	return 0;
}
