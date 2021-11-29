#ifndef MP_HELPER_STRUCTS_HPP
#define MP_HELPER_STRUCTS_HPP

//#include "Light.hpp"
//#include "Material.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>
enum CameraView
{
    ArcBall,
    FreeCam,
    FirstPerson,
    NUMBER_OF_VIEWS
};

enum HeroName
{
    CHRIS,
    THIS_ISNT_ME,
    NUMBER_OF_HEROS
};

struct ShaderUniformLocs
{
    GLuint modelMtx;
    GLuint viewMtx;
    GLuint projMtx;
    GLuint viewPos;
//    MaterialUniformLocs material;
//    DirLightUniformLocs dirLight;
//    PointLightUniformLocs pointLight;
//    SpotLightUniformLocs spotLight;
};

struct ShaderAttribLocs
{
    GLuint vPos;
    GLuint vNormal;
};

struct Vertex
{
    GLfloat x, y, z;
    GLfloat nx, ny, nz;
};

struct Angle
{
    GLfloat theta;
    GLfloat phi;
};

struct MVP
{
    glm::mat4 modelMtx;
    glm::mat4 viewMtx;
    glm::mat4 projMtx;
};

#endif // MP_HELPER_STRUCTS