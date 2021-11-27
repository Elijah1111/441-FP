#ifndef CRAFT_HPP
#define CRAFT_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>

class Player {
public:
    /// \desc creates a simple player that gives the appearance of flight
    /// \param shaderProgramHandle shader program handle that the player should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    Player( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation, GLint mMtxUniformLocation );

    /// \desc draws the model player for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to player
    /// \param viewMtx camera view matrix to apply to player
    /// \param projMtx camera projection matrix to apply to player
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
	void drawMe( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );
    
    
	void moveLeft(double);//move left
	void moveRight(double);//move right
	
	void jump();//jump

	void frame();//increase frame number

    glm::vec3 pos = glm::vec3(5,0,0);//where the player is in space
private:
    const float G = 0.1;//gravity 
	bool airborn   = false;//have we jumped?
	float accel   = 0.0;//acceleration 

    /// \desc handle of the shader program to use when drawing the player
    GLuint _shaderProgramHandle;
    /// \desc stores the uniform locations needed for the plan information
    struct ShaderProgramUniformLocations {
        /// \desc location of the precomputed ModelViewProjection matrix
        GLint mvpMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normalMtx;
        /// \desc location of the material diffuse color
        GLint materialColor;

        GLint model;
    } _shaderProgramUniformLocations;

    /// \desc color BREAD
    glm::vec3 _breadColor;
    /// \desc color BREAD ACCENT
    glm::vec3 _accentColor;
    /// \desc color BUTTER
    glm::vec3 _butterColor;
    /// \desc amount to scale the player's body by
    glm::vec3 _scaleBody;//TODO this does nothing
    
    GLint _frameI=0;//frame index

    /// \desc draws just the player's body
    /// \param modelMtx existing model matrix to apply to player
    /// \param viewMtx camera view matrix to apply to player
    /// \param projMtx camera projection matrix to apply to player
    void _drawBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the nose of the player
    /// \param modelMtx existing model matrix to apply to player
    /// \param viewMtx camera view matrix to apply to player
    /// \param projMtx camera projection matrix to apply to player
    void _drawBack(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the tail of the player
    /// \param modelMtx existing model matrix to apply to player
    /// \param viewMtx camera view matrix to apply to player
    /// \param projMtx camera projection matrix to apply to player
    void _drawFront(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
};


#endif //CRAFT_HPP
