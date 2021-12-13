#include "Engine.hpp"
#include "CustomObjects.hpp"
#include <stdlib.h>
#include <CSCI441/objects.hpp>
#include<iostream>
//*************************************************************************************
//
// Helper Functions

#ifndef M_PI
#define M_PI 3.14159265
#endif

void print(glm::vec3 p){//prints a vector
	std::cout<<p[0]<<' '<<p[1]<<' '<<p[2]<<'\n';
}
/// \desc Simple helper function to return a random number between 0.0f and 1.0f.
GLfloat getRand() {
    return (GLfloat)rand() / (GLfloat)RAND_MAX;
}

double findDist(glm::vec3 P0, glm::vec3 P1){
	//a helper function to find distance between two points
	double x = pow(P0[0] - P1[0],2);
	double y = pow(P0[1] - P1[1],2);
	double z = pow(P0[2] - P1[2],2);
	
	float tmp = pow(x+y+z,0.5);
	std::cout<<"DISTANCE: "<<tmp<<"\n";
	return tmp;
}

//*************************************************************************************
//
// Public Interface

Engine::Engine(int OPENGL_MAJOR_VERSION, int OPENGL_MINOR_VERSION,
                         int WINDOW_WIDTH, int WINDOW_HEIGHT, const char* WINDOW_TITLE)
         : CSCI441::OpenGLEngine(OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE) {

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
}

Engine::~Engine() {
    delete _freeCam;
}

void Engine::handleKeyEvent(GLint key, GLint action) {
    if(key != GLFW_KEY_UNKNOWN){
	if (key == GLFW_KEY_0){
        	_keys[key] = ((action == GLFW_RELEASE));
	}
	if (key == GLFW_KEY_9){
        	_keys[key] = ((action == GLFW_RELEASE));
	}
    	else 
        	_keys[key] = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
    }
    if(action == GLFW_PRESS) {
        switch( key ) {
            // quit!
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                setWindowShouldClose();
                break;
            default: break; // suppress CLion warning
        }
    }
}

void Engine::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
        // update the left mouse button's state
        _leftMouseButtonState = action;

    }

    if( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        _rightPressed = true;
    } else {
        _rightPressed = false;
    }
}

void Engine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    // if the left mouse button is being held down while the mouse is moving
    if(_leftMouseButtonState == GLFW_PRESS) {
        // rotate the camera by the distance the mouse moved
    } 

    // update the mouse position
    _mousePosition = currMousePosition;
}

//*************************************************************************************
//
// Engine Setup

void Engine::_setupGLFW() {
    CSCI441::OpenGLEngine::_setupGLFW();

    // set our callbacks
    glfwSetKeyCallback(_window, keyboard_callback);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);
    glfwSetCursorPosCallback(_window, cursor_callback);
}

void Engine::_setupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                    // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	            // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// clear the frame buffer to black
}

void Engine::_setupShaders() {
    _lightingShaderProgram = new CSCI441::ShaderProgram("shaders/light.v.glsl", "shaders/light.f.glsl" );
    _lightingShaderUniformLocations.mvpMatrix      = _lightingShaderProgram->getUniformLocation("mvpMatrix");
    _lightingShaderUniformLocations.model      = _lightingShaderProgram->getUniformLocation("model");
    
    _lightingShaderUniformLocations.normMat  = _lightingShaderProgram->getUniformLocation("normMat");
    _lightingShaderUniformLocations.lDir  = _lightingShaderProgram->getUniformLocation("lDir");
    _lightingShaderUniformLocations.lCol  = _lightingShaderProgram->getUniformLocation("lCol");

    _lightingShaderUniformLocations.materialColor  = _lightingShaderProgram->getUniformLocation("materialColor");
    _lightingShaderUniformLocations.viewPos  = _lightingShaderProgram->getUniformLocation("viewPos");
    
    _lightingShaderAttributeLocations.vPos         = _lightingShaderProgram->getAttributeLocation("vPos");
    _lightingShaderAttributeLocations.vNorm         = _lightingShaderProgram->getAttributeLocation("vNorm");

    _lightingShaderUniformLocations.dirBool = _lightingShaderProgram->getUniformLocation("dirBool");

    _pointLight.pos = _lightingShaderProgram->getUniformLocation("pointPos");
    _pointLight.col = _lightingShaderProgram->getUniformLocation("pointCol");

    _spotlight.pos = _lightingShaderProgram->getUniformLocation("spotPos");
    _spotlight.col = _lightingShaderProgram->getUniformLocation("spotCol");
    _spotlight.dir = _lightingShaderProgram->getUniformLocation("spotDir");
    
	
	_textureShaderProgram = new CSCI441::ShaderProgram(
					"shaders/tex.v.glsl", "shaders/tex.f.glsl" );
    // query uniform locations
    _textureShaderUniformLocations.mvpMatrix = _textureShaderProgram->
			getUniformLocation("mvpMat");
    _textureShaderUniformLocations.texMap    = _textureShaderProgram->
			getUniformLocation("texMap");
    // query attribute locations
    _textureShaderAttributeLocations.vPos    = _textureShaderProgram->
			getAttributeLocation("aPos");//had to change the name was causing interference

	_textureShaderProgram->setProgramUniform(//set the static texmap
			_textureShaderUniformLocations.texMap,0);

    _bumpShaderProgram = new CSCI441::ShaderProgram("shaders/bump.v.glsl",
                                                    "shaders/bump.f.glsl");

    _bumpShaderUniformLocations.mvpMatrix = _bumpShaderProgram->getUniformLocation("mvpMatrix");
    _bumpShaderUniformLocations.model = _bumpShaderProgram->getUniformLocation("model");
    _bumpShaderUniformLocations.pPos = _bumpShaderProgram->getUniformLocation("pPos");
    _bumpShaderUniformLocations.pCol = _bumpShaderProgram->getUniformLocation("pCol");
    _bumpShaderUniformLocations.vPos = _bumpShaderProgram->getUniformLocation("vPos");
    _bumpShaderUniformLocations.texMap = _bumpShaderProgram->getUniformLocation("texMap");
    _bumpShaderUniformLocations.norMap = _bumpShaderProgram->getUniformLocation("norMap");

    _bumpShaderAttributeLocations.aPos = _bumpShaderProgram->getAttributeLocation("aPos");
    _bumpShaderAttributeLocations.aNormal = _bumpShaderProgram->getAttributeLocation("aNormal");
    _bumpShaderAttributeLocations.aTexCoord = _bumpShaderProgram->getAttributeLocation("aTexCoord");
    _bumpShaderAttributeLocations.aTangent = _bumpShaderProgram->getAttributeLocation("aTangent");

}

void Engine::_setupBuffers() {
    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos,
				   	_lightingShaderAttributeLocations.vNorm );

    _player = new Player(_lightingShaderProgram->getShaderProgramHandle(),
                       _lightingShaderUniformLocations.mvpMatrix,
                       _lightingShaderUniformLocations.normMat,
                       _lightingShaderUniformLocations.materialColor,
                       _lightingShaderUniformLocations.model);
    CustomObjects::setupShaders(
            _lightingShaderProgram->getShaderProgramHandle(),
            _lightingShaderAttributeLocations.vPos,
            _lightingShaderAttributeLocations.vNorm
            );
	_createGroundBuffers();
    _generateEnvironment();

    glGenVertexArrays( 1, &_faceVAO);
    glGenBuffers( 1, &_faceVBO);
}

void Engine::_createObstacle(){
  		double p = 25;//spawn point
		// translate to spot
		glm::mat4 transToSpotMtx = glm::translate( glm::mat4(1.0), glm::vec3(5.0f, 0.0f, -p) );

		// compute random height
		GLdouble height = powf(getRand(), 2.5)*4 + 1;//TODO tweak height to be more fair
		// scale to size
		glm::mat4 scaleToHeightMtx = glm::scale( glm::mat4(1.0), glm::vec3(1, height, 1) );

		// translate up to grid
		glm::mat4 transToHeight = glm::translate( glm::mat4(1.0), glm::vec3(0, height/2.0f, 0) );

		// compute full model matrix
		glm::mat4 modelMatrix = transToHeight * scaleToHeightMtx * transToSpotMtx;

		// compute random color
		glm::vec3 color( getRand(), getRand(), getRand() );
		// store building properties
		Obstacle ob(glm::vec3(0,0,p), modelMatrix, color, getRand()*0.20f + 0.05, height);
		_obs.emplace_back( ob );
}


void Engine::spawnControl(bool pause){
	if(pause)
			  return;//do not spawn when paused
	if(currentFrame - lastSpawnedFrame >= 100){//TODO adjust frame timing
			if(obstacleSlots > 0){
				GLdouble chance = getRand();
				if(chance > 0.4){//60% chance to spawn a obstacle
					_createObstacle();
					obstacleSlots--;
					lastSpawnedFrame = currentFrame;	
				}

			}
	}
}

void Engine::_createGroundBuffers() {
    struct Vertex {
        GLfloat x, y, z;
	    GLfloat xN, yN, zN; //normal
    };

    Vertex groundQuad[4] = {
            {-1.0f, 0.0f, -1.0f, 0.0f, 1.0, 0.0f},
            { 1.0f, 0.0f, -1.0f, 0.0f, 1.0, 0.0f},
            {-1.0f, 0.0f,  1.0f, 0.0f, 1.0, 0.0f},
            { 1.0f, 0.0f,  1.0f, 0.0f, 1.0, 0.0f}
    };

    GLushort indices[4] = {0,1,2,3};

    _numGroundPoints = 4;

    glGenVertexArrays(1, &_groundVAO);
    glBindVertexArray(_groundVAO);

    GLuint vbods[2];       // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundQuad), groundQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vPos);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vNorm);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vNorm, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(GL_FLOAT)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}


bool Engine::_stepBackground(glm::mat4* model,glm::mat4* tree, double* pos, bool type){

   (*pos) += _backgroundSpeed;
	
	(*model) = glm::translate(*model, glm::vec3(0,0,_backgroundSpeed));
   if (type)//move the trees other model aswell
		(*tree) = glm::translate(*tree, glm::vec3(0,0,_backgroundSpeed));

	if((*pos) >= 60){//delete when it gets out of bounds
			  return true;
	}
   return false;
}



void Engine::spawnBackground(){//spawn new background elements to fly by
	if(currentFrame - lastBackgroundFrame <= WORLD_SIZE/_backgroundSpeed + 10)
			  return;//TODO adjust timing
	lastBackgroundFrame = currentFrame;
	
	for(int i = -5; i > -WORLD_SIZE; i--) {//spawn some more trees and things
   	for(int j = -60; j > -WORLD_SIZE-60; j--) {
			  if( !(i % 5) && !(j % 5) && getRand() < 0.3f ) {
					 // translate to spot
					 glm::mat4 transToSpotMtx = glm::translate(
										  glm::mat4(1.0), glm::vec3(i, 0.0f, j) );

					 // compute random height
					 GLdouble height = powf(getRand(), 2.5)*10 + 1;
					 // scale to building size
					 //glm::mat4 scaleToHeightMtx = glm::scale(
					//					  glm::mat4(1.0), glm::vec3(1, height, 1) );

					 // translate up to grid
					 glm::mat4 transToHeight = glm::translate(
										  glm::mat4(1.0), glm::vec3(0, height/2.0f, 0) );

					 // compute full model matrix
					 glm::mat4 modelMatrix = transToHeight 
								* transToSpotMtx;

					 // compute random color
					 glm::vec3 color( getRand(), getRand(), getRand() );
					 // store building properties
					 BuildingData currentBuilding = {1, 1, (float) height, modelMatrix, color, static_cast<double>(j)};
					 _buildings.emplace_back( currentBuilding );
				}
				  
				  //Spawn Trees
			else if( !(i % 10) && !(j % 5) && getRand() < 0.4f) {
			glm::mat4 transToSpotMtx = glm::translate(
								 glm::mat4(1.0), glm::vec3(i, 0.0f, j) );

		  // compute random height
		  GLdouble trunkH = powf(getRand(), 2.5)*2 + 1;
		  GLdouble topH = powf(getRand(), 2.5)*3 + 1;
		  // scale to building size
		  glm::mat4 scaleToHeightMtx = glm::scale(
							  	glm::mat4(1.0), glm::vec3(1, trunkH, 1) );

		  // compute full model matrix
		  glm::mat4 trunkModMatrix = scaleToHeightMtx * transToSpotMtx;
		  
		// scale to trunk+top
		  scaleToHeightMtx = glm::scale(
							  	glm::mat4(1.0), glm::vec3(1, trunkH+topH, 1) );

		  // translate up to grid
		glm::mat4 transToHeight = glm::translate(
							 glm::mat4(1.0), glm::vec3(0, (trunkH), 0) );

		  // compute full model matrix
		  glm::mat4 topModMatrix = transToHeight * scaleToHeightMtx * transToSpotMtx;

		  // store building properties
		  TreeData tmp = {topModMatrix, trunkModMatrix,static_cast<double>(j)};
		  _trees.emplace_back(tmp);}}}

}


void Engine::_generateEnvironment() {
    //******************************************************************
    // parameters to make up our grid size and spacing, feel free to
    // play around with this
    const GLfloat GRID_WIDTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_LENGTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_SPACING_WIDTH = 1.0f;
    const GLfloat GRID_SPACING_LENGTH = 1.0f;
    // precomputed parameters based on above
    const GLfloat LEFT_END_POINT = -GRID_WIDTH / 2.0f - 5.0f;
    const GLfloat RIGHT_END_POINT = 0;
    const GLfloat BOTTOM_END_POINT = -GRID_LENGTH / 2.0f - 5.0f;
    const GLfloat TOP_END_POINT = GRID_LENGTH / 2.0f + 5.0f;
    //******************************************************************


    srand( time(0) );                                                   // seed our RNG
    // psych! everything's on a grid.
    for(int i = LEFT_END_POINT; i < RIGHT_END_POINT; i += GRID_SPACING_WIDTH) {
        for(int j = BOTTOM_END_POINT; j < TOP_END_POINT; j += GRID_SPACING_LENGTH) {
            if(abs(i)<10 and abs(j) <10)
		    		continue;//dont draw too close on the origin
	    if( !(i % 5) && !(j % 5) && getRand() < 0.3f ) {
                // translate to spot
                glm::mat4 transToSpotMtx = glm::translate( glm::mat4(1.0), glm::vec3(i, 0.0f, j) );

                // compute random height
                GLdouble height = powf(getRand(), 2.5)*10 + 1;
                // scale to building size
                glm::mat4 scaleToHeightMtx = glm::scale( glm::mat4(1.0), glm::vec3(1, height, 1) );

                // translate up to grid
                glm::mat4 transToHeight = glm::translate( glm::mat4(1.0), glm::vec3(0, height/2.0f, 0) );

                // compute full model matrix
                //glm::mat4 modelMatrix = transToHeight * scaleToHeightMtx * transToSpotMtx;
                glm::mat4 modelMatrix = transToHeight * transToSpotMtx;

                // compute random color
                glm::vec3 color( getRand(), getRand(), getRand() );
                // store building properties
                BuildingData currentBuilding = {1, 1, (float) height, modelMatrix, color, static_cast<double>(j)};
                _buildings.emplace_back( currentBuilding );
            }
	    else if( !(i % 10) && !(j % 10) && getRand() < 0.4f ) {
                // translate to spot
                glm::mat4 transToSpotMtx = glm::translate( glm::mat4(1.0), glm::vec3(i, 0.0f, j) );

                // compute random height
                GLdouble trunkH = powf(getRand(), 2.5)*2 + 1;
                GLdouble topH = powf(getRand(), 2.5)*3 + 1;
                // scale to building size
                glm::mat4 scaleToHeightMtx = glm::scale( glm::mat4(1.0), glm::vec3(1, trunkH, 1) );

                // compute full model matrix
                glm::mat4 trunkModMatrix = scaleToHeightMtx * transToSpotMtx;
                
		        // scale to trunk+top
                scaleToHeightMtx = glm::scale( glm::mat4(1.0), glm::vec3(1, trunkH+topH, 1) );

                // translate up to grid
		        glm::mat4 transToHeight = glm::translate( glm::mat4(1.0), glm::vec3(0, (trunkH), 0) );

                // compute full model matrix
                glm::mat4 topModMatrix = transToHeight * scaleToHeightMtx * transToSpotMtx;

                // store building properties
                TreeData tmp = {topModMatrix, trunkModMatrix,static_cast<double>(j)};
                _trees.emplace_back(tmp);
            }
        }
    }

}

void Engine::_setupScene() {
     // free camera
    _freeCam = new CSCI441::FreeCam();//setup Free cam, is this a problem?
    _freeCam->setPosition( _camStat.startPos );
    _freeCam->setTheta(_camStat.startRot[0]);
    _freeCam->setPhi( _camStat.startRot[1]);
    _freeCam->recomputeOrientation();
     
	
	// SETUP Point Light	
	glm::vec3 pos = glm::vec3(-WORLD_SIZE/2,10,-WORLD_SIZE/2);
    glm::vec3 col = glm::vec3(0,1,0);

	
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),
                        _pointLight.pos,
                        1,&pos[0]);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),
                        _pointLight.col,
                        1,&col[0]);
	
    glProgramUniform3fv(_bumpShaderProgram->getShaderProgramHandle(),
                        _bumpShaderUniformLocations.pPos,
                        1,&pos[0]);
    glProgramUniform3fv(_bumpShaderProgram->getShaderProgramHandle(),
                        _bumpShaderUniformLocations.pCol,
                        1,&col[0]);
    
	//SETUP SPOTLIGHT
    glm::vec3 pcol = glm::vec3(1, 0, 0);
    glm::vec3 pdir = glm::vec3(0,-1.0f,0);
    glm::vec3 ppos = glm::vec3(5,10, 0);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),
                        _spotlight.pos,
                        1,&ppos[0]);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),
                        _spotlight.col,
                        1,&pcol[0]);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),
                        _spotlight.dir,
                        1,&pdir[0]);

	//SETUP Global Light
    glm::vec3 lDir = glm::vec3(-1,-1,-1);
    glm::vec3 lCol = glm::vec3(1,1,1);
	glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),
			_lightingShaderUniformLocations.lDir,
			1,&lDir[0]);
	glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),
			_lightingShaderUniformLocations.lCol,
			1,&lCol[0]);
}

//*************************************************************************************
//
// Engine Cleanup

void Engine::_cleanupShaders() {
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    delete _lightingShaderProgram;
	delete _textureShaderProgram;
    delete _bumpShaderProgram;

}

void Engine::_cleanupBuffers() {
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs();
    glDeleteVertexArrays( 1, &_groundVAO );
    glDeleteVertexArrays( 1, &_skybox.VAO );

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs();
	glDeleteBuffers(1, &_skybox.VBO);

    fprintf( stdout, "[INFO]: ...deleting models..\n" );
    delete _player;
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void Engine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) {
	////BEGIN DRAW SKYBOX ////
	_textureShaderProgram->useProgram();
	glm::mat4 tmpModel = glm::rotate(glm::translate(glm::mat4(1.0f),glm::vec3(0,0,0)), currentFrame/3000.0f, glm::vec3(0.0f,1.0f,1.0f) );
	glm::mat4 mvpMtx = projMtx * viewMtx * tmpModel;
	_textureShaderProgram->setProgramUniform(
					_textureShaderUniformLocations.mvpMatrix, mvpMtx);
   	_computeAndSendMatrixUniforms(tmpModel, viewMtx, projMtx);
	
	CSCI441::setVertexAttributeLocations(_textureShaderAttributeLocations.vPos);
	
	CSCI441::drawSolidCubeTextured(WORLD_SIZE*10);
    //Draw Bump Map Things
    _bumpShaderProgram->useProgram();



    std::vector<BuildingData> tmpBuild;
    for(int i = 0; i < _buildings.size(); i++){
        BuildingData* b = &_buildings[i];
        mvpMtx = projMtx * viewMtx * b->modelMatrix;

        _bumpShaderProgram->setProgramUniform( _bumpShaderUniformLocations.vPos, _freeCam->getPosition());
        _bumpShaderProgram->setProgramUniform( _bumpShaderUniformLocations.mvpMatrix, mvpMtx);
        _bumpShaderProgram->setProgramUniform( _bumpShaderUniformLocations.model, b->modelMatrix);


        glUniform1i(_bumpShaderUniformLocations.texMap, 2);
        glUniform1i(_bumpShaderUniformLocations.norMap, 4);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, _tex);
        glActiveTexture(GL_TEXTURE0 + 4);
        glBindTexture(GL_TEXTURE_2D, _nor);
    
        _drawRecBumped(b->h, b->w, b->d);

        if(!_stepBackground(&b->modelMatrix, nullptr, &b->pos, false)){
            tmpBuild.emplace_back(*b);
        }

    }
    _buildings = tmpBuild;

    // use our lighting shader program
    _lightingShaderProgram->useProgram();
    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos,
				   	_lightingShaderAttributeLocations.vNorm );

    if(_rightPressed){
        glUniform1i(_lightingShaderUniformLocations.dirBool, 0);
    } else {
        glUniform1i(_lightingShaderUniformLocations.dirBool, 1);
    }

    //// BEGIN DRAWING THE GROUND PLANE ////
    glm::mat4 groundModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(WORLD_SIZE, 1.0f, WORLD_SIZE));
    _computeAndSendMatrixUniforms(groundModelMtx, viewMtx, projMtx);
	glm::vec3 planeColor = glm::vec3(0.5,0.5,0.5);
    glUniform3fv(_lightingShaderUniformLocations.materialColor, 1,&planeColor[0]);
    
    glBindVertexArray(_groundVAO);
    glDrawElements(GL_TRIANGLE_STRIP, _numGroundPoints, GL_UNSIGNED_SHORT, (void*)0);
    //// END DRAWING THE GROUND PLANE ////
    //// BEGIN DRAWING THE GREEN SUN////
    glm::mat4 tmp = glm::translate( glm::mat4(1.0), glm::vec3(-WORLD_SIZE/2, 
			    10.0f, -WORLD_SIZE/2) );
    _computeAndSendMatrixUniforms(tmp, viewMtx, projMtx);
    glm::vec3 green = glm::vec3(0,1,0);
    glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &green[0]);
    CSCI441::drawSolidSphere(1.0,100,100);

    //// END DRAWING THE GREEN SUN////
    
    //// BEGIN DRAWING THE RED CONE////
    tmp = glm::translate( glm::mat4(1.0), glm::vec3(5, 
			    7.80f, 0));
    _computeAndSendMatrixUniforms(tmp, viewMtx, projMtx);
    glm::vec3 red = glm::vec3(1,0,0);
    glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &red[0]);
    CSCI441::drawSolidCone(1.0,2.19,100,100);
    //// END DRAWING THE RED CONE////
    
    //// BEGIN DRAWING THE BUILDINGS ////
    /*
	 std::vector<BuildingData> tmpBuild;
    for(int i=0; i<_buildings.size(); i++ ) {
        BuildingData* cBuild = &_buildings[i];
			_computeAndSendMatrixUniforms(cBuild->modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &cBuild->color[0]);

        CSCI441::drawSolidCube(1.0);
		if(!_stepBackground(&cBuild->modelMatrix, nullptr, &cBuild->pos, false)){
				  tmpBuild.emplace_back(*cBuild);
		}
    }
    _buildings = tmpBuild;
    */
	 //// END DRAWING THE BUILDINGS ////
    //// BEGIN DRAWING THE TREES   ////
	std::vector<TreeData> tmpTree;
    for(int i=0; i<_trees.size(); i++ ) {
        TreeData* cTree = &_trees[i];
			_computeAndSendMatrixUniforms(cTree->trunkModMatrix, viewMtx, projMtx);
        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &cTree->brown[0]);
        CSCI441::drawSolidCylinder(0.5,0.5,1.0,10,10);
        
	_computeAndSendMatrixUniforms(cTree->topModMatrix, viewMtx, projMtx);
        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &cTree->green[0]);
        CSCI441::drawSolidCone(1.0,1.0,10,10);
		if(!_stepBackground(&cTree->trunkModMatrix, &cTree->topModMatrix, &cTree->pos, true)){
			tmpTree.emplace_back(*cTree);
		}
    }
    _trees = tmpTree;
	//// END DRAWING THE TREES ////
	//// BEGIN DRAWING OBSTACLES ////
	std::vector<Obstacle> tmpObs;
	for(int i=0; i<_obs.size(); i++) {
        Obstacle currentObs = _obs[i];
		//print(currentObs.pos);//very spammy
		_computeAndSendMatrixUniforms(currentObs.modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &currentObs.color[0]);

        CSCI441::drawSolidCube(1.0);
		if(!currentObs.step(pause)){
			tmpObs.push_back(currentObs);
		}
		else//delete this obstacle
			obstacleSlots++;
		if(currentObs.collide(_player)){//TODO do something with this
				//fprintf(stdout,"\nCollision\n\n");
		}
    	_obs[i] = currentObs;
	}
	_obs = tmpObs;
	//// END DRAWING OBSTACLES ////
	


    //// BEGIN SETING LOOK AT LOCAL///
    glm::vec3 viewPos;
	viewPos = _freeCam->getPosition();

    glUniform3fv(_lightingShaderUniformLocations.viewPos, 1, &viewPos[0]);
    //// END SETING LOOK AT LOCAL///

    //// BEGIN DRAWING THE MODELS ////
    glm::mat4 modelMtx(1.0f);
    
    glm::mat4 modelMtx0 = glm::translate( modelMtx, _player->pos );
    
    // DRAW PLAYER
    _player->drawMe( modelMtx0, viewMtx, projMtx );
    
    //// END DRAWING MODELS ////
   	
}

void Engine::_updateScene() {
		 spawnControl(pause);
		 spawnBackground();
		// player Move 
		if (!pause){//no moving when paused
				  if (_keys[GLFW_KEY_A]) {//move left
					  _player->moveLeft(_camStat.speed.x * 5);
				  }
				  if (_keys[GLFW_KEY_D]) {//Move Right
					  _player->moveRight(_camStat.speed.x * 5);
				  }
				  if (_keys[GLFW_KEY_W])
						  _player->jump();
		}
        // free cam
		if(_keys[GLFW_KEY_0]){
			_camStat.free = !_camStat.free;
			_keys[GLFW_KEY_0] = false;//consume that input
			if(!_camStat.free){//reset the camera stats
					_freeCam->setPosition( _camStat.startPos );
					_freeCam->setTheta(_camStat.startRot[0]);
					_freeCam->setPhi( _camStat.startRot[1]);
					_freeCam->recomputeOrientation();
			}

		}
		else if(_camStat.free){
				if( _keys[GLFW_KEY_SPACE] ) {
					// go backward if shift held down
					if( _keys[GLFW_KEY_LEFT_SHIFT] || _keys[GLFW_KEY_RIGHT_SHIFT] ) {
						_freeCam->moveBackward(_camStat.speed.x*5);
					}
					// go forward
					else {
						_freeCam->moveForward(_camStat.speed.x*5);
					}
				}
				// turn right
				if( _keys[GLFW_KEY_D] ) {
					_freeCam->rotate(_camStat.speed.y, 0.0f);
				}
				// turn left
				if( _keys[GLFW_KEY_A] ) {
					_freeCam->rotate(-_camStat.speed.y, 0.0f);
				}
				// pitch up
				if( _keys[GLFW_KEY_W] ) {
					_freeCam->rotate(0.0f, _camStat.speed.y);
				}
				// pitch down
				if( _keys[GLFW_KEY_S] ) {
					_freeCam->rotate(0.0f, -_camStat.speed.y);
				}
		}
		
		
		if(_keys[GLFW_KEY_9]){//spawn a obstale
			_keys[GLFW_KEY_9] = false;//consume that input
    		_createObstacle();	
		}
		if(_keys[GLFW_KEY_1]){
			_keys[GLFW_KEY_1] = false;//consume that input
			pause = !pause;
		}
		if (_keys[GLFW_KEY_2]){
			_keys[GLFW_KEY_2] = false;//consume that input
         spawnBackground();

		}
}

void Engine::run() {
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(_window) ) {	        // check if the window was instructed to be closed
        glDrawBuffer( GL_BACK );				        // work with our back frame buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        // Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
        // when using a Retina display the actual window can be larger than the requested window.  Therefore,
        // query what the actual size of the window we are rendering to is.
        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( _window, &framebufferWidth, &framebufferHeight );

        // update the viewport - tell OpenGL we want to render to the whole window
        glViewport( 0, 0, framebufferWidth, framebufferHeight );

        // set the projection matrix based on the window size
        // use a perspective projection that ranges
        // with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 1000].
        glm::mat4 projectionMatrix = glm::perspective( 45.0f, (GLfloat) framebufferWidth / (GLfloat) framebufferHeight, 0.001f, 1000.0f );

        // set up our look at matrix to position our camera

	    glm::mat4 viewMatrix;
        glm::mat4 viewMatrix2;
        viewMatrix = _freeCam->getViewMatrix();
        // draw everything to the window
        _renderScene(viewMatrix, projectionMatrix);
        
		_updateScene();
		frame();//adjust frame timing


        glfwSwapBuffers(_window);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
    }
}
void Engine::frame(){
	currentFrame++;
	currentFrame %= 100000000;//I dont want this overflowing if it comes to it
	if (!currentFrame){
		lastSpawnedFrame=0;//prevent issues when we do rewrap around
		lastBackgroundFrame=0;	
	}
}
//*************************************************************************************
//
// Private Helper FUnctions

void Engine::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.mvpMatrix, mvpMtx);
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.model, modelMtx);

    glm::mat3 normalMtx = glm::mat3(glm::transpose(glm::inverse(modelMtx)));
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.normMat, normalMtx);

}



void Engine::_setupTextures(){
			
		
		_skybox.handle = _loadAndRegisterTexture("");

        _tex = _loadAndRegisterFlatTexture("texture/StoneWall.jpg");
        _nor = _loadAndRegisterFlatTexture("texture/StoneWallNormal.jpg");

        std::cout << "_tex: " << _tex << "_nor: " << _nor << std::endl;
}
GLuint Engine::_loadAndRegisterTexture(const char* FILENAME) {
    // our handle to the GPU
    GLuint textureHandle = 0;
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP,textureHandle);
    
    // will hold image parameters after load
    GLint imageWidth, imageHeight, imageChannels;
    // load image from file
	for(unsigned int i=0; i< 6; i++){
			char fname[] = "./texture/skybox%d.png";
			sprintf(fname,fname,i);
			//fprintf(stdout,fname);	
			
			GLubyte* data = stbi_load( fname, &imageWidth, &imageHeight, &imageChannels, 0);
			
			// if data was read from file
			if( data ) {
				const GLint STORAGE_TYPE = (imageChannels == 4 ? GL_RGBA : GL_RGB);
					
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
						0,STORAGE_TYPE,
						imageWidth,imageHeight,0,
						STORAGE_TYPE,GL_UNSIGNED_BYTE,data);


				// release image memory from CPU - it now lives on the GPU
				stbi_image_free(data);
			} else {
				// load failed
				fprintf( stderr, "[ERROR]: Could not load texture map \"%s\"\n", FILENAME );
				stbi_image_free(data);
			}
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
    // return generated texture handle
	fprintf( stdout, "[INFO]: %s texture map read in with handle %d\n",
				   	FILENAME, textureHandle);
    return textureHandle;
}

GLuint Engine::_loadAndRegisterFlatTexture(const char* FILENAME) {
    GLuint texHandle = 0;

    stbi_set_flip_vertically_on_load(true);

    GLint imageWidth, imageHeight, imageChannels;

    GLubyte* data = stbi_load(FILENAME, &imageWidth, &imageHeight, &imageChannels, 0);

    if(data){
        const GLint STORAGE_TYPE = (imageChannels == 4 ? GL_RGBA : GL_RGB);

        glGenTextures(1, &texHandle);
        glBindTexture(GL_TEXTURE_2D, texHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, STORAGE_TYPE, imageWidth, imageHeight, 0, STORAGE_TYPE, GL_UNSIGNED_BYTE, data);
        fprintf(stdout, "[INFO]: %s texture map read with handle %d\n", FILENAME, texHandle);
    } else {
        fprintf(stdout, "[ERROR]: Failed to load texture: \"%s\"\n", FILENAME);
    }

    return texHandle;
}

void Engine::_drawRecBumped(float h, float w, float d){

    //std::cout << "h: " << h << " w: " << w << " d: " << d << std::endl;

    // [front/back][left/right][top/bottom]
    // ex: Front, left, bottom = flb
    glm::vec3 flb(-w/2, -h/2,  d/2);
    glm::vec3 flt(-w/2,  h/2,  d/2);
    glm::vec3 frb( w/2, -h/2,  d/2);
    glm::vec3 frt( w/2,  h/2,  d/2);
    glm::vec3 brb( w/2, -h/2, -d/2);
    glm::vec3 brt( w/2,  h/2, -d/2);
    glm::vec3 blb(-w/2, -h/2, -d/2);
    glm::vec3 blt(-w/2,  h/2, -d/2);
    
    //Texture coodrinates for each face (back = front, left = right, bottom = top
    glm::vec2 tcf0(0.0f, h);
    glm::vec2 tcf1(0.0f, 0.0f);
    glm::vec2 tcf2(w, h);
    glm::vec2 tcf3(w, 0.0f);

    glm::vec2 tcr0(0.0f, h);
    glm::vec2 tcr1(0.0f, 0.0f);
    glm::vec2 tcr2(d, h);
    glm::vec2 tcr3(d, 0.0f);

    glm::vec2 tct0(0.0f, d);
    glm::vec2 tct1(0.0f, 0.0f);
    glm::vec2 tct2(w, d);
    glm::vec2 tct3(w, 0.0f);

    //A local normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    //Tangents for 2 face triangle
    glm::vec3 ftan1, ftan2, btan1, btan2, ltan1, ltan2, rtan1, rtan2, ttan1, ttan2, bbtan1, bbtan2; 

    //Front Face
    glm::vec3 e1 = flt - flb;
    glm::vec3 e2 = frb - flb;
    glm::vec2 duv1 = tcf0 - tcf1; 
    glm::vec2 duv2 = tcf2 - tcf1;

    float f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    ftan1.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    ftan1.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    ftan1.z = f * (duv2.y * e1.z - duv1.y * e2.z);

    e1 = frb - frt;
    e2 = flt - frt;
    duv1 = tcf0 - tcf1; 
    duv2 = tcf2 - tcf1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    ftan2.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    ftan2.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    ftan2.z = f * (duv2.y * e1.z - duv1.y * e2.z);
    
    //Right Face
    e1 = frt - frb;
    e2 = brb - frb;
    duv1 = tcr0 - tcr1; 
    duv2 = tcr2 - tcr1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    rtan1.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    rtan1.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    rtan1.z = f * (duv2.y * e1.z - duv1.y * e2.z);

    e1 = brb - brt;
    e2 = frt - brt;
    duv1 = tcr0 - tcr1; 
    duv2 = tcr2 - tcr1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    rtan2.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    rtan2.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    rtan2.z = f * (duv2.y * e1.z - duv1.y * e2.z);

    //Back Face
    e1 = brt - brb;
    e2 = blb - brb;
    duv1 = tcf0 - tcf1; 
    duv2 = tcf2 - tcf1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    btan1.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    btan1.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    btan1.z = f * (duv2.y * e1.z - duv1.y * e2.z);

    e1 = blb - blt;
    e2 = brt - blt;
    duv1 = tcf0 - tcf1; 
    duv2 = tcf2 - tcf1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    btan2.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    btan2.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    btan2.z = f * (duv2.y * e1.z - duv1.y * e2.z);
    
    //Left Face
    e1 = blt - blb;
    e2 = flb - blb;
    duv1 = tcr0 - tcr1; 
    duv2 = tcr2 - tcr1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    ltan1.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    ltan1.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    ltan1.z = f * (duv2.y * e1.z - duv1.y * e2.z);

    e1 = flb - flt;
    e2 = blt - flt;
    duv1 = tcr0 - tcr1; 
    duv2 = tcr2 - tcr1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    ltan2.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    ltan2.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    ltan2.z = f * (duv2.y * e1.z - duv1.y * e2.z);

    //Top Face
    e1 = blt - flt;
    e2 = frt - flt;
    duv1 = tct0 - tct1;
    duv2 = tct2 - tct1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    ttan1.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    ttan1.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    ttan1.z = f * (duv2.y * e1.z - duv1.y * e2.z);
    
    e1 = frt - brt;
    e1 = blt - brt;
    duv1 = tct0 - tct1;
    duv2 = tct2 - tct1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    ttan2.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    ttan2.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    ttan2.z = f * (duv2.y * e1.z - duv1.y * e2.z);

    //Bottom Face
    e1 = flb - blb;
    e2 = brb - blb;
    duv1 = tct0 - tct1;
    duv2 = tct2 - tct1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    bbtan1.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    bbtan1.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    bbtan1.z = f * (duv2.y * e1.z - duv1.y * e2.z);
    
    e1 = brb - frb;
    e1 = flb - frb;
    duv1 = tct0 - tct1;
    duv2 = tct2 - tct1;

    f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    bbtan2.x = f * (duv2.y * e1.x - duv1.y * e2.x);
    bbtan2.y = f * (duv2.y * e1.y - duv1.y * e2.y);
    bbtan2.z = f * (duv2.y * e1.z - duv1.y * e2.z);
    
    //Defining each face's buffer to send to GPU
    float fVerts[] = {
        flt.x, flt.y, flt.z, nm.x, nm.y, nm.z, tcf0.x, tcf0.y, ftan1.x, ftan1.y, ftan1.z,
        flb.x, flb.y, flb.z, nm.x, nm.y, nm.z, tcf1.x, tcf1.y, ftan1.x, ftan1.y, ftan1.z,
        frb.x, frb.y, frb.z, nm.x, nm.y, nm.z, tcf3.x, tcf3.y, ftan1.x, ftan1.y, ftan1.z,

        flt.x, flt.y, flt.z, nm.x, nm.y, nm.z, tcf0.x, tcf0.y, ftan2.x, ftan2.y, ftan2.z,
        frb.x, frb.y, frb.z, nm.x, nm.y, nm.z, tcf3.x, tcf3.y, ftan2.x, ftan2.y, ftan2.z,
        frt.x, frt.y, frt.z, nm.x, nm.y, nm.z, tcf2.x, tcf2.y, ftan2.x, ftan2.y, ftan2.z
    };

    float rVerts[] = {
        frt.x, frt.y, frt.z, nm.x, nm.y, nm.z, tcr0.x, tcr0.y, rtan1.x, rtan1.y, rtan1.z,
        frb.x, frb.y, frb.z, nm.x, nm.y, nm.z, tcr1.x, tcr1.y, rtan1.x, rtan1.y, rtan1.z,
        brb.x, brb.y, brb.z, nm.x, nm.y, nm.z, tcr3.x, tcr3.y, rtan1.x, rtan1.y, rtan1.z,

        frt.x, frt.y, frt.z, nm.x, nm.y, nm.z, tcr0.x, tcr0.y, rtan2.x, rtan2.y, rtan2.z,
        brb.x, brb.y, brb.z, nm.x, nm.y, nm.z, tcr3.x, tcr3.y, rtan2.x, rtan2.y, rtan2.z,
        brt.x, brt.y, brt.z, nm.x, nm.y, nm.z, tcr2.x, tcr2.y, rtan2.x, rtan2.y, rtan2.z
    };

    float bVerts[] = {
        brt.x, brt.y, brt.z, nm.x, nm.y, nm.z, tcf0.x, tcf0.y, btan1.x, btan1.y, btan1.z,
        brb.x, brb.y, brb.z, nm.x, nm.y, nm.z, tcf1.x, tcf1.y, btan1.x, btan1.y, btan1.z,
        blb.x, blb.y, blb.z, nm.x, nm.y, nm.z, tcf3.x, tcf3.y, btan1.x, btan1.y, btan1.z,

        brt.x, brt.y, brt.z, nm.x, nm.y, nm.z, tcf0.x, tcf0.y, btan2.x, btan2.y, btan2.z,
        blb.x, blb.y, blb.z, nm.x, nm.y, nm.z, tcf3.x, tcf3.y, btan2.x, btan2.y, btan2.z,
        blt.x, blt.y, blt.z, nm.x, nm.y, nm.z, tcf2.x, tcf2.y, btan2.x, btan2.y, btan2.z
    };

    float lVerts[] = {
        blt.x, blt.y, blt.z, nm.x, nm.y, nm.z, tcr0.x, tcr0.y, ltan1.x, ltan1.y, ltan1.z,
        blb.x, blb.y, blb.z, nm.x, nm.y, nm.z, tcr1.x, tcr1.y, ltan1.x, ltan1.y, ltan1.z,
        flb.x, flb.y, flb.z, nm.x, nm.y, nm.z, tcr3.x, tcr3.y, ltan1.x, ltan1.y, ltan1.z,

        blt.x, blt.y, blt.z, nm.x, nm.y, nm.z, tcr0.x, tcr0.y, ltan2.x, ltan2.y, ltan2.z,
        flb.x, flb.y, flb.z, nm.x, nm.y, nm.z, tcr3.x, tcr3.y, ltan2.x, ltan2.y, ltan2.z,
        flt.x, flt.y, flt.z, nm.x, nm.y, nm.z, tcr2.x, tcr2.y, ltan2.x, ltan2.y, ltan2.z
    };

    float tVerts[] = {
        blt.x, blt.y, blt.z, nm.x, nm.y, nm.z, tct0.x, tct0.y, ttan1.x, ttan1.y, ttan1.z,
        flt.x, flt.y, flt.z, nm.x, nm.y, nm.z, tct1.x, tct1.y, ttan1.x, ttan1.y, ttan1.z,
        frt.x, frt.y, frt.z, nm.x, nm.y, nm.z, tct3.x, tct3.y, ttan1.x, ttan1.y, ttan1.z,

        blt.x, blt.y, blt.z, nm.x, nm.y, nm.z, tct0.x, tct0.y, ttan2.x, ttan2.y, ttan2.z,
        frt.x, frt.y, frt.z, nm.x, nm.y, nm.z, tct3.x, tct3.y, ttan2.x, ttan2.y, ttan2.z,
        brt.x, brt.y, brt.z, nm.x, nm.y, nm.z, tct2.x, tct2.y, ttan2.x, ttan2.y, ttan2.z
    };

    float bbVerts[] = {
        flb.x, flb.y, flb.z, nm.x, nm.y, nm.z, tct0.x, tct0.y, bbtan1.x, bbtan1.y, bbtan1.z,
        blb.x, blb.y, blb.z, nm.x, nm.y, nm.z, tct1.x, tct1.y, bbtan1.x, bbtan1.y, bbtan1.z,
        brb.x, brb.y, brb.z, nm.x, nm.y, nm.z, tct3.x, tct3.y, bbtan1.x, bbtan1.y, bbtan1.z,

        flb.x, flb.y, flb.z, nm.x, nm.y, nm.z, tct0.x, tct0.y, bbtan2.x, bbtan2.y, bbtan2.z,
        brb.x, brb.y, brb.z, nm.x, nm.y, nm.z, tct3.x, tct3.y, bbtan2.x, bbtan2.y, bbtan2.z,
        frb.x, frb.y, frb.z, nm.x, nm.y, nm.z, tct2.x, tct2.y, bbtan2.x, bbtan2.y, bbtan2.z
    };

    glBindVertexArray( _faceVAO );
    glBindBuffer( GL_ARRAY_BUFFER, _faceVBO );

    float * verts;
    for(int i = 0; i < 6; i++){
        switch(i){
            case 0:
                glBufferData( GL_ARRAY_BUFFER, sizeof(fVerts), &fVerts, GL_STATIC_DRAW );
                break;
            case 1:
                glBufferData( GL_ARRAY_BUFFER, sizeof(rVerts), &rVerts, GL_STATIC_DRAW );
                break;
            case 2:
                glBufferData( GL_ARRAY_BUFFER, sizeof(bVerts), &bVerts, GL_STATIC_DRAW );
                break;
            case 3:
                glBufferData( GL_ARRAY_BUFFER, sizeof(lVerts), &lVerts, GL_STATIC_DRAW );
                break;
            case 4:
                glBufferData( GL_ARRAY_BUFFER, sizeof(tVerts), &tVerts, GL_STATIC_DRAW );
                break;
            case 5:
                glBufferData( GL_ARRAY_BUFFER, sizeof(bbVerts), &bbVerts, GL_STATIC_DRAW );
                break;
        }

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (8 * sizeof(float)));

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

}
//*************************************************************************************
//
// Callbacks

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (Engine*) glfwGetWindowUserPointer(window);

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (Engine*) glfwGetWindowUserPointer(window);

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (Engine*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}
