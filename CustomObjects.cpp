#include "CustomObjects.hpp"
// all custom objects auto setup buffers if the instance is null
CustomObjects &CustomObjects::instance()
{
    static CustomObjects customObjects;
    return customObjects;
}

void CustomObjects::clearObjects()
{
    // #TODO clear objects
}
void CustomObjects::setupShaders(GLuint shaderProgramHandle, GLuint posLoc, GLuint normalLoc)
{
    instance()._shaderProgramHandle = shaderProgramHandle;
    instance()._shaderAttribPos = posLoc;
    instance()._shaderAttribNormal = normalLoc;
}

// for environment and Chris
void CustomObjects::drawCube()
{
    if (instance()._cubeVAO == 0)
    {
        instance()._setupBuffersCube();
    }

    glBindVertexArray(instance()._cubeVAO);
    glDrawElements(GL_TRIANGLES, instance()._cubePointsCount, GL_UNSIGNED_SHORT, (void *)0);
}

// ground plane
void CustomObjects::drawPlane()
{
    if (instance()._modelPlane == nullptr)
    {
        instance()._setupBuffersPlane();
    }

    instance()._modelPlane->draw(instance()._shaderProgramHandle);
}

void CustomObjects::drawTeapot()
{
    if (instance()._modelTeapot == nullptr)
    {
        instance()._setupBuffersTeapot();
    }

    instance()._modelTeapot->draw(instance()._shaderProgramHandle);
}

// for thisisntme
void CustomObjects::drawCar()
{
    if (instance()._modelCar == nullptr)
    {
        instance()._setupBuffersCar();
    }
    instance()._modelCar->draw(instance()._shaderProgramHandle);
}
void CustomObjects::drawHorse(int frame)
{
    if (instance()._modelHorse.size() == 0)
    {
        instance()._setupBuffersHorse();
    }
    instance()._modelHorse[frame % 48]->draw(instance()._shaderProgramHandle);
}

// for thisisntme
void CustomObjects::drawWheel()
{
    if (instance()._modelWheel == nullptr)
    {
        instance()._setupBuffersWheel();
    }
    instance()._modelWheel->draw(instance()._shaderProgramHandle);
}

// for environment
void CustomObjects::drawTreeTop()
{
    if (instance()._modelTreeTop == nullptr)
    {
        instance()._setupBuffersTreeTop();
    }
    instance()._modelTreeTop->draw(instance()._shaderProgramHandle);
}

// for environment
void CustomObjects::drawTreeTrunk()
{
    if (instance()._modelTreeTrunk == nullptr)
    {
        instance()._setupBuffersTreeTrunk();
    }
    instance()._modelTreeTrunk->draw(instance()._shaderProgramHandle);
}

// better cube with more verticies than 8 so we can get nicer lighting
void CustomObjects::_setupBuffersCube()
{
    GLfloat CORNER_POINT = 0.5f;
    Vertex vertices[] = {
        // top face
        {CORNER_POINT, CORNER_POINT, CORNER_POINT,
         0.0f, 1.0f, 0.0f},
        {-CORNER_POINT, CORNER_POINT, CORNER_POINT,
         0.0f, 1.0f, 0.0f},
        {CORNER_POINT, CORNER_POINT, -CORNER_POINT,
         0.0f, 1.0f, 0.0f},
        {-CORNER_POINT, CORNER_POINT, -CORNER_POINT,
         0.0f, 1.0f, 0.0f},
        // bottom face
        {CORNER_POINT, -CORNER_POINT, CORNER_POINT,
         0.0f, -1.0f, 0.0f},
        {-CORNER_POINT, -CORNER_POINT, CORNER_POINT,
         0.0f, -1.0f, 0.0f},
        {CORNER_POINT, -CORNER_POINT, -CORNER_POINT,
         0.0f, -1.0f, 0.0f},
        {-CORNER_POINT, -CORNER_POINT, -CORNER_POINT,
         0.0f, -1.0f, 0.0f},
        // front face
        {CORNER_POINT, CORNER_POINT, CORNER_POINT,
         0.0f, 0.0f, 1.0f},
        {-CORNER_POINT, CORNER_POINT, CORNER_POINT,
         0.0f, 0.0f, 1.0f},
        {CORNER_POINT, -CORNER_POINT, CORNER_POINT,
         0.0f, 0.0f, 1.0f},
        {-CORNER_POINT, -CORNER_POINT, CORNER_POINT,
         0.0f, 0.0f, 1.0f},
        // back face
        {CORNER_POINT, CORNER_POINT, -CORNER_POINT,
         0.0f, 0.0f, -1.0f},
        {-CORNER_POINT, CORNER_POINT, -CORNER_POINT,
         0.0f, 0.0f, -1.0f},
        {CORNER_POINT, -CORNER_POINT, -CORNER_POINT,
         0.0f, 0.0f, -1.0f},
        {-CORNER_POINT, -CORNER_POINT, -CORNER_POINT,
         0.0f, 0.0f, -1.0f},
        // right face
        {CORNER_POINT, CORNER_POINT, CORNER_POINT,
         1.0f, 0.0f, 0.0f},
        {CORNER_POINT, CORNER_POINT, -CORNER_POINT,
         1.0f, 0.0f, 0.0f},
        {CORNER_POINT, -CORNER_POINT, CORNER_POINT,
         1.0f, 0.0f, 0.0f},
        {CORNER_POINT, -CORNER_POINT, -CORNER_POINT,
         1.0f, 0.0f, 0.0f},
        // left face
        {-CORNER_POINT, CORNER_POINT, CORNER_POINT,
         -1.0f, 0.0f, 0.0f},
        {-CORNER_POINT, CORNER_POINT, -CORNER_POINT,
         -1.0f, 0.0f, 0.0f},
        {-CORNER_POINT, -CORNER_POINT, CORNER_POINT,
         -1.0f, 0.0f, 0.0f},
        {-CORNER_POINT, -CORNER_POINT, -CORNER_POINT,
         -1.0f, 0.0f, 0.0f}};

    unsigned short indices[] = {
        0, 1, 2, 3, 2, 1,
        4, 5, 6, 7, 6, 5,
        8, 9, 10, 11, 10, 9,
        12, 13, 14, 15, 14, 13,
        16, 17, 18, 19, 18, 17,
        20, 21, 22, 23, 22, 21};
    _cubePointsCount = 36;

    glGenVertexArrays(1, &_cubeVAO);
    glBindVertexArray(_cubeVAO);

    GLuint vbods[2]; // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(_shaderAttribPos);
    glVertexAttribPointer(_shaderAttribPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(_shaderAttribNormal);
    glVertexAttribPointer(_shaderAttribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(GL_FLOAT) * 3));
}

void CustomObjects::_setupBuffersTeapot()
{
    _modelTeapot = new CSCI441::ModelLoader();
    _modelTeapot->loadModelFile("models/teapot.obj");
    _modelTeapot->setAttributeLocations(instance()._shaderAttribPos, instance()._shaderAttribNormal);
}

void CustomObjects::_setupBuffersPlane()
{
    _modelPlane = new CSCI441::ModelLoader();
    _modelPlane->loadModelFile("models/subdivided_plane.obj");
    _modelPlane->setAttributeLocations(instance()._shaderAttribPos, instance()._shaderAttribNormal);
}

void CustomObjects::_setupBuffersHorse()
{
    for (int i = 1; i <= 48; i++)
    {
        _modelHorse.push_back(new CSCI441::ModelLoader());
        string fileNum = to_string(i);
        if (i < 10)
        {
            fileNum = "0" + fileNum;
        }
        string fileString = "models/horse-gallop/horse-gallop-" + fileNum + ".obj";
        char *charArr = &fileString[0];
        _modelHorse[i - 1]->loadModelFile(charArr);
        _modelHorse[i - 1]->setAttributeLocations(instance()._shaderAttribPos, instance()._shaderAttribNormal);
    }
}

void CustomObjects::_setupBuffersCar()
{
    _modelCar = new CSCI441::ModelLoader();
    _modelCar->loadModelFile("models/vehiclenormclean.obj");
    _modelCar->setAttributeLocations(instance()._shaderAttribPos, instance()._shaderAttribNormal);
}

void CustomObjects::_setupBuffersTreeTop()
{
    _modelTreeTop = new CSCI441::ModelLoader();
    _modelTreeTop->loadModelFile("models/tree_top.obj");
    _modelTreeTop->setAttributeLocations(instance()._shaderAttribPos, instance()._shaderAttribNormal);
}
void CustomObjects::_setupBuffersTreeTrunk()
{
    _modelTreeTrunk = new CSCI441::ModelLoader();
    _modelTreeTrunk->loadModelFile("models/tree_trunk.obj");
    _modelTreeTrunk->setAttributeLocations(instance()._shaderAttribPos, instance()._shaderAttribNormal);
}
void CustomObjects::_setupBuffersWheel()
{
    _modelWheel = new CSCI441::ModelLoader();
    _modelWheel->loadModelFile("models/JR-152.obj");
    _modelWheel->setAttributeLocations(instance()._shaderAttribPos, instance()._shaderAttribNormal);
}