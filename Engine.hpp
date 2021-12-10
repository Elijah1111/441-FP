#ifndef LAB05_LAB05_ENGINE_HPP
#define LAB05_LAB05_ENGINE_HPP

#include <CSCI441/OpenGLEngine.hpp>
#include <CSCI441/ShaderProgram.hpp>
#include <CSCI441/FreeCam.hpp>
#include <stb_image.h>

#include <vector>
#include <ctime>

#include "Player.hpp"
#include "Obstacle.hpp"

class Engine : public CSCI441::OpenGLEngine {
public:
    Engine(int OPENGL_MAJOR_VERSION, int OPENGL_MINOR_VERSION,
                int WINDOW_WIDTH, int WINDOW_HEIGHT,
                const char* WINDOW_TITLE);
    ~Engine();

    void run() final;
	void loadFile(std::string);
	void setRes(int res);	

    /// \desc handle any key events inside the engine
    /// \param key key as represented by GLFW_KEY_ macros
    /// \param action key event action as represented by GLFW_ macros
    void handleKeyEvent(GLint key, GLint action);

    /// \desc handle any mouse button events inside the engine
    /// \param button mouse button as represented by GLFW_MOUSE_BUTTON_ macros
    /// \param action mouse event as represented by GLFW_ macros
    void handleMouseButtonEvent(GLint button, GLint action);

    /// \desc handle any cursor movement events inside the engine
    /// \param currMousePosition the current cursor position
    void handleCursorPositionEvent(glm::vec2 currMousePosition);

    /// \desc value off-screen to represent mouse has not begun interacting with window yet
    static constexpr GLfloat MOUSE_UNINITIALIZED = -9999.0f;
    
    /// \desc what hero we have selected
    int selectedHero = 0;
private:
    /// \desc set up functions
    void _setupGLFW() final;
    void _setupOpenGL() final;
    void _setupShaders() final;
    void _setupBuffers() final;
    void _setupScene() final;
    void _setupTextures() final;
	/// \desc cleanup functions
    void _cleanupBuffers() final;
    void _cleanupShaders() final;

	void _createSkybox();
	GLuint _loadAndRegisterTexture(const char*);
    GLuint _loadAndRegisterFlatTexture(const char*);
	
	struct skybox{
		GLuint handle;
		GLuint VAO;
		GLuint VBO;
	}_skybox;	
	
	/// \desc draws everything to the scene from a particular point of view
    /// \param viewMtx the current view matrix for our camera
    /// \param projMtx the current projection matrix for our camera
    void _renderScene(glm::mat4 viewMtx, glm::mat4 projMtx);
    /// \desc handles moving as determined by keyboard input
    void _updateScene();

    /// \desc tracks the number of different keys that can be present as determined by GLFW
    static constexpr GLuint NUM_KEYS = GLFW_KEY_LAST;
    /// \desc boolean array tracking each key state.  if true, then the key is in a pressed or held
    /// down state.  if false, then the key is in a released state and not being interacted with
    GLboolean _keys[NUM_KEYS];

    /// \desc last location of the mouse in window coordinates
    glm::vec2 _mousePosition;
    /// \desc current state of the left mouse button
    GLint _leftMouseButtonState;

    /// \desc Free Camera
    CSCI441::FreeCam* _freeCam;
	struct cameraStats{
		glm::vec2 speed = glm::vec2(0.05,0.05);
		glm::vec3 startPos = glm::vec3(20.0f, 5.0f, 0.0f) ;
		glm::vec2 startRot = glm::vec2(-M_PI/2.0f,M_PI/2.0f);
		bool free = false;//is the camera in free roam
	}_camStat;//start locations etc

    /// \desc HoverPlayer model
    Player* _player;

    /// \desc the size of the world (controls the ground size and locations of buildings)
    static constexpr GLfloat WORLD_SIZE = 55.0f;
    /// \desc VAO for our ground
    GLuint _groundVAO;
    
    /// \desc VAO for ViewPort
    
    /// \desc the number of points that make up our ground object
    GLsizei _numGroundPoints;

    /// \desc creates the ground VAO
    void _createGroundBuffers();
    
    /// \desc smart container to store information specific to each building we wish to draw
    struct BuildingData {
        /// \desc transformations to position and size the building
        glm::mat4 modelMatrix;
        /// \desc color to draw the building
        glm::vec3 color;
    };
    /// \desc information list of all the buildings to draw
    std::vector<BuildingData> _buildings;
    
    struct BumpData {
        float w,d,h;

        glm::mat4 modelMatrix;
    };

    std::vector<BumpData> _bumps;

    struct TreeData {
        /// \desc transformations to position and size the building
        glm::mat4 topModMatrix;
        glm::mat4 trunkModMatrix;
	glm::vec3 green = glm::vec3(0.0,0.8,0.0);
	glm::vec3 brown = glm::vec3(0.325,0.192,0.094);
    };
    std::vector<TreeData> _trees;
	

	std::vector<Obstacle> _obs;//obstacles


    /// \desc generates building information to make up our scene
    void _generateEnvironment();
	
	
	void _createObstacle();//creates an obstacle


    /// \desc shader program that performs lighting
    CSCI441::ShaderProgram* _lightingShaderProgram = nullptr;   // the wrapper for our shader program
    CSCI441::ShaderProgram* _textureShaderProgram = nullptr;   // the wrapper for our shader program
    CSCI441::ShaderProgram* _bumpShaderProgram = nullptr;

	/// \desc stores the locations of all of our shader uniforms
    struct LightingShaderUniformLocations {
        /// \desc precomputed MVP matrix location
        GLint mvpMatrix;
        GLint model;
        /// \desc material diffuse color location
        GLint materialColor;
        /// \desc normal matrix
    	GLint normMat;
        /// \desc direction for the direction light
        GLint lDir;
        /// \desc color of the direction light
        GLint lCol;
	/// \desc poisition we are view from
        GLint viewPos;

        GLint dirBool;
    } _lightingShaderUniformLocations;
  	
	struct TextureShaderUniformLocations {
        /// \desc precomputed MVP matrix location
        GLint mvpMatrix;
        GLint texMap;

    } _textureShaderUniformLocations;

    struct BumpShaderUniformLocation {
        GLint mvpMatrix;
        GLint model;

        GLint lPos;

        GLint vPos;

        GLint texMap;
        GLint norMap;
    } _bumpShaderUniformLocations;
    
    GLuint _tex;
    GLuint _nor;

    struct pointLight{
	/// \desc position of the point light
	GLint col;
	GLint pos;
    }_pointLight;
    
	struct spotLight{
        /// \desc position of the point light
        GLint col;
        GLint pos;
        GLint width;
        GLint dir;
    }_spotlight;
    
	struct TextureShaderAttributeLocations{//locations for texture atrib
        GLint vPos;
        GLint texPos;
    } _textureShaderAttributeLocations;
	
	/// \desc stores the locations of all of our shader attributes
    struct LightingShaderAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
        /// \desc vertex normal
        GLint vNorm;
    } _lightingShaderAttributeLocations;

    struct BumpShaderAttributeLocations {
        GLint aPos;
        GLint aNormal;
        GLint aTexCoord;
        GLint aTangent;
    } _bumpShaderAttributeLocations;

    bool _rightPressed = false;

    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;

    void _drawRecBumped(float h, float w, float d);
};
/// \desc functions for user interactions
void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods );
void cursor_callback(GLFWwindow *window, double x, double y );
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods );

#endif // LAB05_LAB05_ENGINE_HPP
