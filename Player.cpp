#include "Player.hpp"
#include "CustomObjects.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

#include <iostream>//TODO remove me

#ifndef M_PI
#define M_PI 3.14159265
#endif

Player::Player( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation, GLint mMtxUniformLocation ) {

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor    = materialColorUniformLocation;
    _shaderProgramUniformLocations.model            = mMtxUniformLocation;

}

void Player::moveLeft(double s){
	double tmp = pos.z;
	tmp += s;
	if (fabs(tmp) >= 10)//Bounds Check
            return;
	pos.z = tmp;
}

void Player::moveRight(double s){
	double tmp = pos.z;
	tmp -= s;
	if (fabs(tmp) >= 10)//Bounds Check
            return;
	pos.z = tmp;
}

void Player::jump(){
	if (airborn){//no double jump... yet
			return;
	}
	airborn = true;
	accel = 1;//set accel upward to 10
}

void Player::drawMe( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) {
    frame();//increase frame counter
	glUseProgram( _shaderProgramHandle );
	t++;
    modelMtx = glm::rotate(glm::translate(modelMtx, glm::vec3 (0,0.05*(sin(M_PI/128*_frameI)+1),0)), static_cast<float>(M_PI), CSCI441::Y_AXIS);
    
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    CustomObjects::drawHorse(t);
	//TODO rebind modelMtx, it inhereats color from the Obstacles
	for(int i=0; i<4; i++){//update bounding Box
        bBox[i] = modelMtx * bBoxCont[i];//adjust the bounding box for the model
    }
}


void Player::frame(){//increment frame
	_frameI++;
	_frameI %= 256;
	if(airborn){//we are in the air
		float* tmp = &pos[1];//faster to use pointers
		(*tmp) += accel;
		if((*tmp) <= 0){//we have hit the ground
			(*tmp) = 0;
			accel = 0;
			airborn = false;	
		}
		else{	
			accel -= G;//change the acceleration
		}
	}

}

void Player::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.model, 1, GL_FALSE, &modelMtx[0][0] );

    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}
