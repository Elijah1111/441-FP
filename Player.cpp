#include "Player.hpp"

#ifndef M_PI
#define M_PI 3.14159265
#endif

Player::Player(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normMtxUniformLocation, ShaderMaterial materialUniformLocation, GLint modelMtxUniformLocation)
{

	_shaderProgramHandle = shaderProgramHandle;
	_shaderProgramUniformLocations.material = materialUniformLocation;
	_shaderProgramUniformLocations.mvpMtx = mvpMtxUniformLocation;
	_shaderProgramUniformLocations.normMtx = normMtxUniformLocation;
	_shaderProgramUniformLocations.modelMtx = modelMtxUniformLocation;
}

void Player::moveLeft(double s)
{
	double tmp = pos.z;
	tmp += s;
	if (fabs(tmp) >= 10) // Bounds Check
		return;
	pos.z = tmp;
}

void Player::moveRight(double s)
{
	double tmp = pos.z;
	tmp -= s;
	if (fabs(tmp) >= 10) // Bounds Check
		return;
	pos.z = tmp;
}

void Player::jump()
{
	if (airborn)
	{ // no double jump... yet
		return;
	}
	airborn = true;
	accel = 1; // set accel upward to 10
}

void Player::drawMe(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx)
{
	frame(); // increase frame counter
	glUseProgram(_shaderProgramHandle);
	t++;
	modelMtx = glm::rotate(glm::translate(modelMtx, glm::vec3(0, 0.05 * (sin(M_PI / 128 * _frameI) + 1), 0)), static_cast<float>(M_PI), CSCI441::Y_AXIS);

	_computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
	_sendMaterial(EMERALD);
	CustomObjects::drawHorse(t);
	// TODO rebind modelMtx, it inhereats color from the Obstacles
	for (int i = 0; i < 4; i++)
	{									  // update bounding Box
		bBox[i] = modelMtx * bBoxCont[i]; // adjust the bounding box for the model
	}
}

void Player::frame()
{ // increment frame
	_frameI++;
	_frameI %= 256;
	if (airborn)
	{						  // we are in the air
		float *tmp = &pos[1]; // faster to use pointers
		(*tmp) += accel;
		if ((*tmp) <= 0)
		{ // we have hit the ground
			(*tmp) = 0;
			accel = 0;
			airborn = false;
		}
		else
		{
			accel -= G; // change the acceleration
		}
	}
}

void Player::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const
{
	// precompute the Model-View-Projection matrix on the CPU
	glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
	// then send it to the shader on the GPU to apply to every vertex
	glProgramUniformMatrix4fv(_shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0]);
	glProgramUniformMatrix4fv(_shaderProgramHandle, _shaderProgramUniformLocations.modelMtx, 1, GL_FALSE, &modelMtx[0][0]);

	glm::mat3 normalMtx = glm::mat3(glm::transpose(glm::inverse(modelMtx)));
	glProgramUniformMatrix3fv(_shaderProgramHandle, _shaderProgramUniformLocations.normMtx, 1, GL_FALSE, &normalMtx[0][0]);
}

void Player::_sendMaterial(Material material) const
{
	glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.material.ambient, 1, &material.ambient[0]);
	glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.material.diffuse, 1, &material.diffuse[0]);
	glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.material.specular, 1, &material.specular[0]);
	glProgramUniform1f(_shaderProgramHandle, _shaderProgramUniformLocations.material.shininess, material.shininess);
}