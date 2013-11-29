//#include <math.h>
//#define GLM_MESSAGES
//#define GLM_FORCE_RADIANS

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>

using namespace glm;
 
#include "OpenGLApplicationBase.h"

#include "ColoredPyramid.h"
#include "Pyramid.h"
#include "Controller.h"
#include "Spinner.h"
#include "WaypointController.h"
#include "OrbitController.h"
#include "WaypointController2.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Torus.h"
#include "Cone.h"
#include "Floor2.h"
#include "Wall.h"
#include "Cube.h"

#include "SpaceShip.h"


#include "GeneralLightingTechnique.h"

void print(vec3 v) {

	cout << "[ " << v.x << " " << v.y << " " << v.z << " ]" << endl;
}

void print(vec4 v) {

	cout << "[ " << v.x << " " << v.y << " " << v.z << " " << v.w << " ]" << endl;
}


class textureLab : public OpenGLApplicationBase
{
public:

	textureLab()
	{
		// Read the files and create the OpenGL shader program.
		shaderProgram = setUpLighting();

		cube = new Cube(this, "self.bmp", 2.0f, 2.0f);
		cube->fixedTransformation = rotate(mat4(1.0f), 180.0f, vec3(0.0f, 0.0f, 1.0f));
		cube->setShader(shaderProgram);
		root.addChild(cube);

		wall = new Wall(this, "stonewall.bmp");
		wall->fixedTransformation = translate(mat4(1.0f), vec3(0.0f, -3.0f, -4.0f));
		wall->setShader(shaderProgram);
		root.addChild(wall);

		pyramid1 = new Sphere( this );
		pyramid1->fixedTransformation = rotate(mat4(1.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f));
		pyramid1->addController(new SpinnerController(vec3(3.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 35.0f));
		pyramid1->setShader(shaderProgram);
		root.addChild( pyramid1 );

		pyramid2 = new Cylinder( this );
		pyramid2->addController(new SpinnerController(vec3(-3.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 15.0f));
		pyramid2->fixedTransformation = rotate(mat4(1.0f), 90.0f, vec3(1,0,0));
		pyramid2->setShader(shaderProgram);
		root.addChild( pyramid2 );

		vector<vec3> waypoints;
		waypoints.push_back(vec3( 3.5f, -3.0f, 3.5f ));
		waypoints.push_back(vec3( -3.5f, -3.0f, -3.5f ));
		waypoints.push_back(vec3( 3.5f, -3.0f, -3.5f ));
		waypoints.push_back(vec3( -3.5f, -3.0f, 3.5f ));
		waypoints.push_back(vec3( 3.5f, 2.5f, 3.5f ));
		waypoints.push_back(vec3( -3.5f, 2.5f, -3.5f ));
		waypoints.push_back(vec3( 3.5f, 2.5f, -3.5f ));
		waypoints.push_back(vec3( -3.5f, 2.5f, 3.5f ));
		
		pyramid4 = new Cone( this );
		pyramid4->fixedTransformation = rotate( mat4(1.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f) );
		pyramid4->addController( new WaypointController2(waypoints) );
		pyramid4->setShader(shaderProgram);
		root.addChild( pyramid4 );

		floor = new Floor2(this);
		floor->setShader(shaderProgram);
		root.addChild( floor );

		rotationX = 0.0f;
		rotationY = 0.0f;
		viewTranslate = -12.0f;

	} // end constructor


	~textureLab()
	{

	} // end destructor

	GLuint setUpLighting() {

		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "pVS-phong-per-frag.vert" },
			{ GL_FRAGMENT_SHADER, "pfixed-function-phong-per-frag.frag"},
			{ GL_NONE, NULL } // signals that there are no more shaders
		};

		shaderProgram = LoadShaders(shaders);

		projectionLocation = getUniformLocation(shaderProgram, "projectionMatrix");
		assert(projectionLocation != 0xFFFFFFFF);

		viewLocation = getUniformLocation(shaderProgram, "viewMatrix");
		assert(viewLocation != 0xFFFFFFFF);

		this->eyePositionLocation = getUniformLocation(shaderProgram, "worldEyePosition");

		// Get ambient light locations
		 basicLight = AmbientLight( 
		 getUniformLocation( shaderProgram, "ambLight.colorIntensity"),
		 getUniformLocation( shaderProgram, "ambLight.enabled"));
 
		 // Set ambient light values
		 basicLight.setColorIntensity( vec4( 0.3f, 0.3f, 0.3f, 1.0f ));
		 basicLight.setEnabled ( true );

		  // Get directional light locations
		 directLight = DirectionalLight( getUniformLocation( shaderProgram, "directLight.colorIntensity"),
										 getUniformLocation( shaderProgram, "directLight.direction"), 
										 getUniformLocation( shaderProgram, "directLight.specularIntensity"), 
										 getUniformLocation( shaderProgram, "directLight.enabled")  );
 
		 // Set directional light values
		 directLight.setColorIntensity( vec4(0.75f, 0.75f, 0.75f, 1.0f) );
		 directLight.setDirection( vec3(1.0f, 1.0f, 1.0f) );
		 directLight.setSpecularIntensity( 
		 vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		 directLight.setEnabled( true );

		  posLight = PositionalLight( getUniformLocation( shaderProgram, "posLight.colorIntensity"),
									  getUniformLocation( shaderProgram, "posLight.position"),
									  getUniformLocation( shaderProgram,  "posLight.specularIntensity"),
									  getUniformLocation( shaderProgram,  "posLight.enabled")  );
 
		
		 posLight.setColorIntensity( vec4(0.5f, 0.5f, 0.5f, 1.0f) );
		 posLight.setPosition( vec3(1.0f, 3.0f, 1.0f) );
		 posLight.setSpecularIntensity( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		 posLight.setEnabled( true );

		 // Spot light
		  spotLight = SpotLight( getUniformLocation( shaderProgram, "spLight.colorIntensity"),
								 getUniformLocation( shaderProgram,  "spLight.position"),
								 getUniformLocation( shaderProgram, "spLight.specularIntensity"),
								 getUniformLocation( shaderProgram,  "spLight.spotDirection"),
								 getUniformLocation( shaderProgram, "spLight.spotCutoffCos"),
								 getUniformLocation( shaderProgram, "spLight.enabled") );
 
		 spotLight.setColorIntensity( vec4(1.0f, 1.0f, 1.0f, 1.0f));
		 spotLight.setPosition(  vec3(0.0f, 8.0f, 0.0f) );
		 spotLight.setSpecularIntensity( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		 spotLight.setSpotDirection( vec3(0.0f, -1.0f, 0.0f) );
		 spotLight.setSpotCutoffCos( cos(glm::radians(15.0f)) );
		 spotLight.setEnabled( true );


		// Read the files and create the OpenGL shader program.
		return shaderProgram;
	}


    virtual bool initialize()
    {
		//glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable( GL_DEPTH_TEST );

		setUpMenus();

		// Set the viewing transformation that will be used to render all objects 
		// in the scene.
		this->ViewMatrix = translate(mat4(1.0f), vec3( 0.0f, 0.0f,-12.0f ));

		root.initialize();

		floor->modelMatrix = translate(mat4(1.0f), vec3( 0.0f, -3.0f, 0.0f ));

		return true;

    } // end initialize


	// Render scene objects
	virtual void drawScene()
	{ 
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(getProjectionMatrix()));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(getViewMatrix()));

		root.draw();

	} // end drawScene


	// Update scene objects inbetween frames
	virtual void updateScene( float elapsedTimeSec ) 
	{ 
		setViewPoint();

		root.update( elapsedTimeSec );

	} // end updateScene


	// Set the viewing transformations
	void setViewPoint(  ) 
	{
		mat4 transView, rotateViewX, rotateViewY;
		transView = translate(mat4(1.0f), vec3( 0.0f, 0.0f, viewTranslate ));
		rotateViewX = rotate(mat4(1.0f), rotationX, vec3(1.0f, 0.0f, 0.0f)); 
		rotateViewY = rotate(mat4(1.0f), rotationY, vec3(0.0f, 1.0f, 0.0f)); 
				
		ViewMatrix = transView * rotateViewX * rotateViewY;

		vec3 viewPoint = (inverse(ViewMatrix)[3]).xyz;

		glUniform3fv( eyePositionLocation, 1, value_ptr(viewPoint));

	} // end setViewPoint


	void SpecialKeyboardCB(int Key, int x, int y)
	{
		switch (Key) {

			case GLUT_KEY_RIGHT:
				rotationY++;
				break;
			case GLUT_KEY_LEFT:
				rotationY--;
				break;
			case GLUT_KEY_UP:
				rotationX++;
				break;
			case GLUT_KEY_DOWN:
				rotationX--;
				break;
			default:
				OpenGLApplicationBase::SpecialKeyboardCB(Key, x, y);
				break;
		}

	} // end SpecialKeyboardCB


	void KeyboardCB(unsigned char Key, int x, int y)
	{
		static SpinnerController * pyramid1Cont;
		switch (Key) {
			case 'w': case 'W':
				viewTranslate++;
				break;
			case 's': case 'S':
				viewTranslate--;
				break; 
			case '1':
				if (pyramid0->getParent() == NULL ) {
					root.addChild(pyramid0);
				}
				else {
					pyramid0->detachFromParent();
				}
				break;
			case '2':
				cout << "case 2" << endl;
				if (pyramid1->hasController() ) {
					pyramid1Cont = (SpinnerController*)pyramid1->removeController();
					cout << "detach" << endl;
				}
				else {
					pyramid1->addController (pyramid1Cont);
					cout << "attach" << endl;
				}
				break;

			case 'a':

				if (basicLight.getEnabled() == true ) {
					basicLight.setEnabled( false );
				}
				else {
					basicLight.setEnabled( true );
				}

				break;
			case 'd':

				if (directLight.getEnabled() == true ) {
					directLight.setEnabled( false );
				}
				else {
					directLight.setEnabled( true );
				}
				break;
			case 'p':

				if (posLight.getEnabled() == true ) {
					posLight.setEnabled( false );
				}
				else {
					posLight.setEnabled( true );
				}
				break;
			case 'x':

				if (spotLight.getEnabled() == true ) {
					spotLight.setEnabled( false );
				}
				else {
					spotLight.setEnabled( true );
				}
				break;

			default:
				OpenGLApplicationBase::KeyboardCB(Key, x, y);
					break;
		}

	} // end KeyboardCB


	friend void mainMenu(int value);

	void setUpMenus()
	{	
		GLuint menu2id = createPolygonMenu();
		GLuint menu3id = createFrontFaceMenu();
		GLuint menu4id = createPointSizeMenu();
		GLuint menu5id = createAntialiasingMenu();

		// Create main menu
		topMenu = glutCreateMenu(mainMenu);
		glutAddSubMenu("Polygon Mode", menu2id); 
		glutAddSubMenu("Rendered Polygon Face ", menu3id);
		glutAddSubMenu("Point Size ", menu4id); 
		glutAddSubMenu("Antialiasing ", menu5id);
		glutAddMenuEntry("Quit", 1); // Specify indentifier for "Quit"			  
		glutAttachMenu(GLUT_RIGHT_BUTTON); // Attach menu to right mouse button

	} //end setUpMenus

	// Data Members

	VisualObject * pyramid0, * pyramid1, *pyramid2, *pyramid3, *pyramid4, * floor, * wall, *cube;

	VisualObject * planet0, * planet1, * planet2;
	VisualObject root;

	GLfloat rotationX, rotationY, viewTranslate;

	AmbientLight basicLight;
	DirectionalLight directLight;
	PositionalLight posLight;
	SpotLight spotLight;

	GLuint shaderProgram;
	GLuint eyePositionLocation; 
	GLuint projectionLocation; 
	GLuint viewLocation;

}; // end textureLab


void mainMenu(int value)
{
	switch (value) {

		case(1):
			glutLeaveMainLoop(); // End the program
			break;
		default:
			cout << "Unknown Main Menu Selection!" << endl;
	}

} // end mainMenu




int main(int argc, char** argv)
{
    GLUTBaseInit(argc, argv);

	GLUTBaseCreateWindow( "CSE 386 Texture Lab" );

    textureLab pApp;
	
	pApp.initialize();

	GLUTBaseRunApplication(&pApp);

    return 0;

} // end main