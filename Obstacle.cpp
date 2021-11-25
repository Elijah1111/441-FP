#include "Obstacle.hpp"

Obstacle::Obstacle(glm::vec3 pos, glm::mat4 model, glm::vec3 color, double s){
		this->speed = s;
		this->pos = pos;
		this->modelMatrix = model;
		this->color = color;
}


bool Obstacle::step(){//step along by speed
	glm::vec3 tmp = this->pos;

	tmp[2] += speed;

	this ->pos = tmp;
	return offScreen();//check if off screen
}

bool Obstacle::offScreen(){
	double p = this->pos[2];

	if(p >= bound + 5)//delete when it gets out of bounds
		return true;
	return false;		
}
