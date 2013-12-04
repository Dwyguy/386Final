#ifndef ___TANK_H___
#define ___TANK_H___

#include "visualobject.h"
#include "LoadShaders.h"

class Tank : public VisualObject
{
	public:
		Tank( OpenGLApplicationBase * OpenGLApp);

		virtual void setShader( GLuint shaderProgram );

	protected:

		GLuint projectionLocation; 
		GLuint modelViewLocation;
};

#endif // ___SPACESHIP_H___
