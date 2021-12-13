#include "Obstacle.hpp"
#include "iostream"
void print(glm::vec4 p){//prints a vector
    std::cout<<p[0]<<' '<<p[1]<<' '<<p[2]<<'\n';
}

Obstacle::Obstacle(glm::vec3 pos, glm::mat4 model, glm::vec3 color, double s, double h){
		this->speed = s;
		this->pos = pos;
		this->modelMatrix = model;
		this->color = color;
		this->h = h;
}

bool Obstacle::step(bool pause){//step along by speed
	if (!pause)	{
			glm::vec3 tmp = this->pos;

			tmp[2] -= speed;

			this->pos = tmp;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0,0,speed));
			for(int i=0; i<4; i++){
				bBox[i] = modelMatrix * bBoxCont[i];//adjust the bounding box for the model
			}
			
			
			return offScreen();//check if off screen
	}
	return false;
}
bool Obstacle::offScreen(){
	double p = this->pos[2];
	if(p <= -bound - 5)//delete when it gets out of bounds
		return true;
	return false;		
}

bool Obstacle::collide(Player* p){//did the player collide with me?
	glm::vec4* playerPos = p->bBox;		
	for(int i=0; i<4; i++){
		glm::vec3 tmp = playerPos[i];//grab x,y
		if(tmp.z >= bBox[0].z and tmp.z <= bBox[1].z 
						and tmp.y <= bBox[2].y and tmp.y >= bBox[0].y){
				return true;//A point is in bounds of the obstacle
		}
	}
	return false;
}
