#include "Player.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

#ifndef M_PI
#define M_PI 3.14159265
#endif

Player::Player( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation, GLint mMtxUniformLocation ) {

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor    = materialColorUniformLocation;
    _shaderProgramUniformLocations.model            = mMtxUniformLocation;

    _breadColor = glm::vec3( 0.776f,0.537f,0.345f);
    _accentColor = glm::vec3( 0.952f,0.835f,0.647f);
    _butterColor = glm::vec3( 0.9f,0.9f,0.0f);
    _scaleBody = glm::vec3( 2.0f, 0.5f, 1.0f );
}

void Player::drawMe( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) {
    glUseProgram( _shaderProgramHandle );
	
    modelMtx = glm::rotate( modelMtx, heading, CSCI441::Y_AXIS );
    modelMtx = glm::translate( modelMtx, glm::vec3 (0,0.05*(sin(M_PI/128*_frameI)+1),0) );
    
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    _drawBody(modelMtx, viewMtx, projMtx);        // the body of our plane
    _drawBack(modelMtx, viewMtx, projMtx);        // the nose
    _drawFront(modelMtx, viewMtx, projMtx);        // the tail
}

void Player::turnLeft() {
    heading += _headRot;
    if( heading > 2.0f * M_PI ) heading -= 2.0f * M_PI;//clamp heading
}

void Player::turnRight() {
    heading -= _headRot;
    if( heading < 0.0f ) heading += 2.0f * M_PI;
}

void Player::_drawBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    GLfloat  h = 0.05;
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_breadColor[0]);
	
    CSCI441::drawSolidCylinder(0.3,0.3,h,100,100);
    modelMtx = glm::translate( modelMtx, glm::vec3 (0,h,0) );
    modelMtx = glm::rotate( modelMtx, (GLfloat)(-M_PI/2.0f), CSCI441::X_AXIS );
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
   
    CSCI441::drawSolidDisk(0, 0.3,100,100);
    
    modelMtx = glm::translate( modelMtx, glm::vec3 (0,0,0.005) );
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_accentColor[0]);
    CSCI441::drawSolidDisk(0, 0.25,100,100);//draw an accent circle
}


void Player::_drawBack(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    
	modelMtx = glm::translate( modelMtx, glm::vec3 (0,0,-0.2) );
    
	_computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_breadColor[0]);

    CSCI441::drawSolidCylinder(0.03,0.03,0.2,100,100);
	
    glm::mat4 tmp = glm::translate( modelMtx, glm::vec3 (0,0.2,0) );
    tmp = glm::rotate( tmp, (GLfloat)(-M_PI/2.0f), CSCI441::X_AXIS );
    _computeAndSendMatrixUniforms(tmp, viewMtx, projMtx);
    
    CSCI441::drawSolidDisk(0, 0.03,100,100);//add a top
	
    modelMtx = glm::translate( modelMtx, glm::vec3 (0,0.15,0) );
    modelMtx = glm::rotate( modelMtx, (GLfloat)(-M_PI/2.0f), CSCI441::X_AXIS );
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_accentColor[0]);
    CSCI441::drawSolidCylinder(0.02,0.02,0.1,100,100);//add exaughst pipe
}

void Player::_drawFront(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
	
    modelMtx = glm::translate( modelMtx, glm::vec3 (0,0,0.2) );
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_butterColor[0]);

    CSCI441::drawSolidCone( 0.02, 0.125, 16, 16 );
}

void Player::frame(){//increment frame
	_frameI++;
	_frameI %= 256;
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
