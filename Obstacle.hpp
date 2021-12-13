#ifndef OB_HPP
#define OB_HPP

#include <glm/gtc/matrix_transform.hpp>

#include "Player.hpp"
class Obstacle{//A class to track Obstacles
public:
	
	glm::vec3 pos;//position
	double speed;//speed

	Obstacle(glm::vec3 pos, glm::mat4 model, glm::vec3 color, double speed, double h);
	
	bool step(bool pause);
	glm::mat4 modelMatrix;
	glm::vec3 color;
	bool collide(Player*);
private:
	bool offScreen();//check if the position is off screen and delete
	double bound = 20;//World Bounds
	double h; 

	glm::vec4 bBoxCont [4] = {//points for its bounding box
    	glm::vec4(0,-0.5,-0.5,1),
    	glm::vec4(0,-0.5,0.5,1),
    	glm::vec4(0,0.5,0.5,1),
    	glm::vec4(0,0.5,-0.5,1)};
	glm::vec4 bBox [4];
};

#endif
