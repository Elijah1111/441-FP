#ifndef MP_CUSTOM_OBJECTS_HPP
#define MP_CUSTOM_OBJECTS_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "HelperStructs.hpp"
#include "CSCI441/ModelLoader.hpp"

class CustomObjects
{
    CustomObjects() = default;
    static CustomObjects &instance();

public:
    static void setupShaders(GLuint shaderProgramHandle, GLuint posLoc, GLuint normalLoc);
    static void drawCube();
    // obj file imports
    static void drawPlane();
    static void drawCar();
    static void drawTreeTop();
    static void drawTreeTrunk();
    static void drawWheel();
    static void clearObjects();
    static void drawHorse(int frame);

private:
    void _setupBuffersCube();
    void _setupBuffersPlane();
    void _setupBuffersCar();
    void _setupBuffersHorse();
    void _setupBuffersTreeTop();
    void _setupBuffersTreeTrunk();
    void _setupBuffersWheel();

    GLuint _cubeVAO;
    GLuint _cubePointsCount;
    GLuint _shaderProgramHandle;
    GLuint _shaderAttribPos;
    GLuint _shaderAttribNormal;

    CSCI441::ModelLoader *_modelPlane;
    CSCI441::ModelLoader *_modelCar;
    CSCI441::ModelLoader *_modelTreeTop;
    CSCI441::ModelLoader *_modelTreeTrunk;
    CSCI441::ModelLoader *_modelWheel;
    std::vector<CSCI441::ModelLoader *> _modelHorse;
};

#endif // MP_CUSTOM_OBJECTS_HPP