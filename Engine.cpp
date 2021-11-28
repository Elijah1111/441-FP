#include "Engine.hpp"
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

}

void Engine::_setupBuffers() {
    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos,
				   	_lightingShaderAttributeLocations.vNorm );

    _player = new Player(_lightingShaderProgram->getShaderProgramHandle(),
                       _lightingShaderUniformLocations.mvpMatrix,
                       _lightingShaderUniformLocations.normMat,
                       _lightingShaderUniformLocations.materialColor,
                       _lightingShaderUniformLocations.model
                       );
    
	_createGroundBuffers();
    _generateEnvironment();
}

void Engine::_createObstacle(){
  		double p = 25;
		// translate to spot
		glm::mat4 transToSpotMtx = glm::translate( glm::mat4(1.0), glm::vec3(5.0f, 0.0f, -p) );

		// compute random height
		GLdouble height = powf(getRand(), 2.5)*5 + 1;
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
            // don't just draw a building ANYWHERE.
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
                glm::mat4 modelMatrix = transToHeight * scaleToHeightMtx * transToSpotMtx;

                // compute random color
                glm::vec3 color( getRand(), getRand(), getRand() );
                // store building properties
                BuildingData currentBuilding = {modelMatrix, color};
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
                TreeData tmp = {topModMatrix, trunkModMatrix};
                _trees.emplace_back(tmp);
            }
        }
    }
    glm::mat4 transToSpotMtxW = glm::translate( glm::mat4(1.0), glm::vec3(-WORLD_SIZE/2, 0.0f, -WORLD_SIZE/2));
    glm::mat4 modelMatrixW = transToSpotMtxW;
    glm::vec3 colorW( 1, 1, 1);
    BuildingData currentBuildingW = {modelMatrixW, colorW};
    _buildings.emplace_back( currentBuildingW );

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
	//BEGIN DRAW SKYBOX
	_textureShaderProgram->useProgram();
	glm::mat4 tmpModel = glm::translate(glm::mat4(1.0f),glm::vec3(0,WORLD_SIZE,0));
	glm::mat4 mvpMtx = projMtx * viewMtx * tmpModel;
	_textureShaderProgram->setProgramUniform(
					_textureShaderUniformLocations.mvpMatrix, mvpMtx);
   	_computeAndSendMatrixUniforms(tmpModel, viewMtx, projMtx);
	
	CSCI441::setVertexAttributeLocations(_textureShaderAttributeLocations.vPos);
	
	CSCI441::drawSolidCubeTextured(WORLD_SIZE*3);
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
    for( const BuildingData& currentBuilding : _buildings ) {
        _computeAndSendMatrixUniforms(currentBuilding.modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &currentBuilding.color[0]);

        CSCI441::drawSolidCube(1.0);
    }
    //// END DRAWING THE BUILDINGS ////
    //// BEGIN DRAWING THE TREES   ////
    for( const TreeData& cTree : _trees ) {
        _computeAndSendMatrixUniforms(cTree.trunkModMatrix, viewMtx, projMtx);
        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &cTree.brown[0]);
        CSCI441::drawSolidCylinder(0.5,0.5,1.0,10,10);
        
	_computeAndSendMatrixUniforms(cTree.topModMatrix, viewMtx, projMtx);
        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &cTree.green[0]);
        CSCI441::drawSolidCone(1.0,1.0,10,10);
    }
    //// END DRAWING THE TREES ////
	//// BEGIN DRAWING OBSTACLES ////
	std::vector<int> remove;
	for(int i=0; i<_obs.size(); i++) {
        Obstacle currentObs = _obs[i];
		//print(currentObs.pos);//very spammy
		_computeAndSendMatrixUniforms(currentObs.modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &currentObs.color[0]);

        CSCI441::drawSolidCube(1.0);
		if(currentObs.step())
				remove.emplace_back(i);
    	_obs[i] = currentObs;
	}
	for(int i : remove){//remove obstacls that have gone out of bounds
			_obs.erase(_obs.begin() + i);
	}
	//// END DRAWING OBSTACLES ////
	


    //// BEGIN SETING LOOK AT LOCAL///
    glm::vec3 viewPos;
	viewPos = _freeCam->getPosition();

    glUniform3fv(_lightingShaderUniformLocations.viewPos, 1, &viewPos[0]);
    //// END SETING LOOK AT LOCAL///

    //// BEGIN DRAWING THE MODELS ////
    glm::mat4 modelMtx(1.0f);
    
    glm::mat4 modelMtx0 = glm::translate( modelMtx, _player->pos );
    
    // draw our craft now
    _player->drawMe( modelMtx0, viewMtx, projMtx );
    
    //// END DRAWING MODELS ////
   	
}

void Engine::_updateScene() {
		// player Move 
		if (_keys[GLFW_KEY_A]) {//move left
			_player->moveLeft(_camStat.speed.x * 5);
		}
		if (_keys[GLFW_KEY_D]) {//Move Right
			_player->moveRight(_camStat.speed.x * 5);
		}
		if (_keys[GLFW_KEY_W])
				_player->jump();
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



        glfwSwapBuffers(_window);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
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
			char fname[] = "./texture/skybox%d.jpg";
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
