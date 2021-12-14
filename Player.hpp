#ifndef CRAFT_HPP
#define CRAFT_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

#include <iostream> //TODO remove me

#include "Player.hpp"
#include "CustomObjects.hpp"
#include "Structs.hpp"

class Player
{
public:
    /// \desc creates a simple player that gives the appearance of flight
    /// \param shaderProgramHandle shader program handle that the player should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    Player(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normMtxUniformLocation, ShaderMaterial materialUniformLocation, GLint modelMtxUniformLocation);

    /// \desc draws the model player for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to player
    /// \param viewMtx camera view matrix to apply to player
    /// \param projMtx camera projection matrix to apply to player
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
    void drawMe(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx);

    void moveLeft(double);  // move left
    void moveRight(double); // move right

    glm::vec3 pos = glm::vec3(5, 0, 0);          // where the player is in space
    glm::vec4 bBox[4] = {glm::vec4(0, 0, 0, 0)}; // changing Bounding Box
private:
    const float G = 0.1;  // gravity
    bool airborn = false; // have we jumped?
    float accel = 0.0;    // acceleration
    int t = 0;            // animation frame

    /// \desc handle of the shader program to use when drawing the player
    GLuint _shaderProgramHandle;
    /// \desc stores the uniform locations needed for the plan information
    struct ShaderProgramUniformLocations
    {
        /// \desc location of the precomputed ModelViewProjection matrix
        GLint mvpMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normMtx;
        /// \desc location of the material diffuse color
        ShaderMaterial material;

        GLint modelMtx;
    } _shaderProgramUniformLocations;

    GLint _frameI = 0;        // frame index
    glm::vec4 bBoxCont[4] = { // Control points for its bounding box
        glm::vec4(-0.5, 1.0, -0.5, 1),
        glm::vec4(0.5, 1.0, 0.5, 1),
        glm::vec4(0, 0, 0.5, 1),
        glm::vec4(0, 0, -0.5, 1)};

    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
    void _sendMaterial(Material material) const;
};

#endif // CRAFT_HPP
