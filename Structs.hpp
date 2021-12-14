#ifndef FP_STRUCTS_HPP
#define FP_STRUCTS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct ShaderMaterial
{
    GLint ambient;
    GLint diffuse;
    GLint specular;
    GLint shininess;
};

struct Light
{
    glm::vec3 position;
    glm::vec3 direction;
    GLfloat angle;
    GLfloat falloff;
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLboolean visible;
};

struct ShaderLight
{
    GLint position;
    GLint direction;
    GLint angle;
    GLint falloff;
    GLint constant;
    GLint linear;
    GLint quadratic;

    GLint ambient;
    GLint diffuse;
    GLint specular;
    GLint visible;
};

const Material WHITE_PLASTIC = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.55f, 0.55f, 0.55f),
    glm::vec3(0.7f, 0.7f, 0.7f), 0.25f};

const Material GRAY_FLAT = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.25f, 0.25f, 0.25f),
    glm::vec3(0.0f, 0.0f, 0.0f), 0.1f};

const Material FULL_GREEN = {
    glm::vec3(0.0, 10.0, 0.0),
    glm::vec3(0.0, 0.0, 0.0),
    glm::vec3(0.0, 0.0, 0.0), 0.1f};

const Material FULL_RED = {
    glm::vec3(10.0, 0.0, 0.0),
    glm::vec3(0.0, 0.0, 0.0),
    glm::vec3(0.0, 0.0, 0.0), 0.1f};

const Material EMERALD = {
    glm::vec3(0.0215, 0.1745, 0.0215),
    glm::vec3(0.07568, 0.61424, 0.07568),
    glm::vec3(0.633, 0.727811, 0.633), 0.6f};

#endif // FP_HELPER_STRUCTS