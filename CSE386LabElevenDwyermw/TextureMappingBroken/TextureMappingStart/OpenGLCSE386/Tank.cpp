#include "Tank.h"

#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Cube.h"


Tank::Tank( OpenGLApplicationBase * OpenGLApp)
	: VisualObject( OpenGLApp )
{

	VisualObject* body = new Cube(OpenGLApp, "", 1.0f, 2.0f, 3.0f);
	body->fixedTransformation == translate(mat4(1.0f), vec3(0.0f, -2.0f, 0.0f));
	this->addChild(body);

	VisualObject* leftTread = new Cube(OpenGLApp, "", 0.3f, 0.3f, 3.0f);
	body->fixedTransformation == translate(mat4(1.0f), vec3(-0.5f, -3.0f, 0.0f));
	this->addChild(leftTread);

	VisualObject* rightTread = new Cube(OpenGLApp, "", 1.0f, 2.0f, 3.0f);
	body->fixedTransformation == translate(mat4(1.0f), vec3(0.5f, -3.0f, 0.0f));
	this->addChild(rightTread);

	VisualObject* cannon = new Cylinder(OpenGLApp, 2.0f, 0.5f);
	body->fixedTransformation == translate(mat4(1.0f), vec3(0.0f, -1.5f, 0.0f));
	this->addChild(cannon);

} // end Tank constructor

void Tank::setShader( GLuint shaderProgram ) 
{ 
	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->setShader(shaderProgram);
	}
}



