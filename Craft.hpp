#ifndef CRAFT_HPP
#define CRAFT_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>

class Craft {
public:
    /// \desc creates a simple craft that gives the appearance of flight
    /// \param shaderProgramHandle shader program handle that the craft should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    Craft( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation, GLint mMtxUniformLocation );

    /// \desc draws the model craft for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to craft
    /// \param viewMtx camera view matrix to apply to craft
    /// \param projMtx camera projection matrix to apply to craft
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
    void drawMe( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );
    
    void frame();//increase frame number

    void turnLeft();//change the heading
    void turnRight();
    glm::vec3 pos = glm::vec3(5,0,0);//where the vehicle is in space
    GLfloat heading = 0.0;//current heading
    glm::vec3 fpPos = glm::vec3(0.0,0.4,0.3);//where the FP camera should be pointing in local space
private:
    //angle of the wheels
    
    GLfloat _headRot = 0.05;//rotation speed of heading
    /// \desc handle of the shader program to use when drawing the craft
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

    /// \desc angle to rotate our craft at
    GLfloat _rotateCraftAngle;

    /// \desc color BREAD
    glm::vec3 _breadColor;
    /// \desc color BREAD ACCENT
    glm::vec3 _accentColor;
    /// \desc color BUTTER
    glm::vec3 _butterColor;
    /// \desc amount to scale the craft's body by
    glm::vec3 _scaleBody;
    
    GLint _frameI=0;//frame index

    /// \desc draws just the craft's body
    /// \param modelMtx existing model matrix to apply to craft
    /// \param viewMtx camera view matrix to apply to craft
    /// \param projMtx camera projection matrix to apply to craft
    void _drawBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the nose of the craft
    /// \param modelMtx existing model matrix to apply to craft
    /// \param viewMtx camera view matrix to apply to craft
    /// \param projMtx camera projection matrix to apply to craft
    void _drawBack(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the tail of the craft
    /// \param modelMtx existing model matrix to apply to craft
    /// \param viewMtx camera view matrix to apply to craft
    /// \param projMtx camera projection matrix to apply to craft
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
