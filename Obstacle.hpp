#ifndef OB_HPP
#define OB_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Obstacle{//A class to track Obstacles
public:
	static double bound;//World Bounds
	
	glm::vec3 pos;//position
	double speed;//speed

	Obstacle(glm::vec3 pos, glm::mat4 model, glm::vec3 color, double speed);
	
	bool step();
	glm::mat4 modelMatrix;
	glm::vec3 color;
private:
	bool offScreen();//check if the position is off screen and delete
};

double Obstacle::bound = 20;
#endif
