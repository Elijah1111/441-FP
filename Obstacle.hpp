#ifndef OB_HPP
#define OB_HPP

#include <glm/gtc/matrix_transform.hpp>


class Obstacle{//A class to track Obstacles
public:
	
	glm::vec3 pos;//position
	double speed;//speed

	Obstacle(glm::vec3 pos, glm::mat4 model, glm::vec3 color, double speed, double h);
	
	bool step();
	glm::mat4 modelMatrix;
	glm::vec3 color;
private:
	bool offScreen();//check if the position is off screen and delete
	double bound = 20;//World Bounds
};

#endif
