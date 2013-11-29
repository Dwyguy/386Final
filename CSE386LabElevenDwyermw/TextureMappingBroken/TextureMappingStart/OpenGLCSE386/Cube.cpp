#include "Cube.h"
#include "vertexStructs.h"


Cube::Cube(OpenGLApplicationBase * OpenGLApp, string textureFileName,
	float height, float width, int rows, int columns)
	: VisualObject(OpenGLApp), textureFileName( textureFileName ),
	  height(height), width(width), rows(rows), columns(columns)
{

}


Cube::~Cube(void)
{
}

void Cube::buildShaderProgram()
{
	if ( shaderProgram==0 )  {

		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "pVSpSolidColor.vert" },
			{ GL_FRAGMENT_SHADER, "pFSc.frag"},
			{ GL_NONE, NULL } // signals that there are no more shaders
		};

		// Read the files and create the OpenGL shader program.
		shaderProgram = LoadShaders(shaders);
	}

	// This program will stay in effect for all draw calls until it is 
	// replaced with another or explicitly disabled (and the 
	// fixed function pipeline is enabld) by calling glUseProgram with NULL
    glUseProgram(shaderProgram);

	modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);

	material = Material( getUniformLocation( shaderProgram, "object.ambientMat"),
						 getUniformLocation( shaderProgram, "object.diffuseMat"),
						 getUniformLocation( shaderProgram, "object.specularMat"),
						 getUniformLocation( shaderProgram, "object.specularExp"),
						 getUniformLocation( shaderProgram, "object.emissiveMat" ),
						 getUniformLocation( shaderProgram, "object.textureMapped") );// ****
	
	material.setAmbientAndDiffuseMat( vec4( 0.5f, 0.5f, 0.5f, 1.0f ));
	material.setSpecularMat( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	material.setSpecularExponentMat( 64.0f );
	material.setTextureMapped( true ); ///****

}


// Construct visual object display list.
void Cube::initialize()
{
	textureObject = setupTexture( textureFileName );

	buildShaderProgram( );

	// vector containers to hold  data
	vector<pntVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	vec3 n0 = vec3( 1.0f, 0.0f, 0.0f);
	vec3 n1 = vec3( -1.0f, 0.0f, 0.0f);
	vec3 n2 = vec3( 0.0f, 1.0f, 0.0f);
	vec3 n3 = vec3( 0.0f, -1.0f, 0.0f);
	vec3 n4 = vec3( 0.0f, 0.0f, 1.0f);
	vec3 n5 = vec3( 0.0f, 0.0f, -1.0f);

	vec3 v0 = vec3( -width/2, width/2, width/2); 
	vec3 v1 = vec3( -width/2, -width/2, width/2); 
	vec3 v2 = vec3( width/2, -width/2, width/2); 
	vec3 v3 = vec3( width/2, width/2, width/2);
	vec3 v4 = vec3( width/2, width/2, -width/2); 
	vec3 v5 = vec3( -width/2, width/2, -width/2); 
	vec3 v6 = vec3( -width/2, -width/2, -width/2); 
	vec3 v7 = vec3( width/2, -width/2, -width/2);

	 
	 vec2 t0 = vec2(0.0f, 0.0f);
	 vec2 t1 = vec2(0.0f, 1.0f);
	 vec2 t2 = vec2(1.0f, 1.0f);
	 vec2 t3 = vec2(1.0f, 0.0f);

	// +Z
	v.push_back( pntVertexData( v0, n4, t0) ); // 0
	v.push_back( pntVertexData( v1, n4, t1) ); // 1
	v.push_back( pntVertexData( v2, n4, t2) ); // 2
	v.push_back( pntVertexData( v3, n4, t3) ); // 3
	// +X
	v.push_back( pntVertexData( v3, n0, t0) ); // 4
	v.push_back( pntVertexData( v2, n0, t1) ); // 5
	v.push_back( pntVertexData( v7, n0, t2) ); // 6
	v.push_back( pntVertexData( v4, n0, t3) ); // 7
	// -Z
	v.push_back( pntVertexData( v4, n5, t0) ); // 8
	v.push_back( pntVertexData( v7, n5, t1) ); // 9
	v.push_back( pntVertexData( v6, n5, t2) ); // 10
	v.push_back( pntVertexData( v5, n5, t3) ); // 11
	// -X
	v.push_back( pntVertexData( v5, n1, t0) ); // 12
	v.push_back( pntVertexData( v6, n1, t1) ); // 13
	v.push_back( pntVertexData( v1, n1, t2) ); // 14
	v.push_back( pntVertexData( v0, n1, t3) ); // 15
	// +Y
	v.push_back( pntVertexData( v5, n2, t0) ); // 16
	v.push_back( pntVertexData( v0, n2, t1) ); // 17
	v.push_back( pntVertexData( v3, n2, t2) ); // 18
	v.push_back( pntVertexData( v4, n2, t3) ); // 19
	// -Y
	v.push_back( pntVertexData( v1, n3, t0) ); // 20
	v.push_back( pntVertexData( v6, n3, t1) ); // 21
	v.push_back( pntVertexData( v7, n3, t2) ); // 22
	v.push_back( pntVertexData( v2, n3, t3) ); // 23

	indices.push_back( 0 );
	indices.push_back( 1 );
	indices.push_back( 2 );
	indices.push_back( 0 );
	indices.push_back( 2 );
	indices.push_back( 3 );

	indices.push_back( 4 );
	indices.push_back( 5 );
	indices.push_back( 6 );
	indices.push_back( 4 );
	indices.push_back( 6 );
	indices.push_back( 7 );

	indices.push_back( 8 );
	indices.push_back( 9 );
	indices.push_back( 10 );
	indices.push_back( 8 );
	indices.push_back( 10 );
	indices.push_back( 11 );

	indices.push_back( 12 );
	indices.push_back( 13 );
	indices.push_back( 14 );
	indices.push_back( 12 );
	indices.push_back( 14 );
	indices.push_back( 15 );

	indices.push_back( 16 );
	indices.push_back( 17 );
	indices.push_back( 18 );
	indices.push_back( 16 );
	indices.push_back( 18 );
	indices.push_back( 19 );

	indices.push_back( 20 );
	indices.push_back( 21 );
	indices.push_back( 22 );
	indices.push_back( 20 );
	indices.push_back( 22 );
	indices.push_back( 23 );

	glGenVertexArrays (1, &vertexArrayObject);
	glBindVertexArray( vertexArrayObject );

	// Create the buffer to hold interleaved data and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);
 
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,  sizeof(pntVertexData),  (const GLvoid*)(2 * sizeof(vec3)) );
	glEnableVertexAttribArray(3);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	indiceCount = indices.size();
 
	v.clear();
	indices.clear();

	VisualObject::initialize();

} // end initialize

// Preform drawing operations
void Cube::draw()
{
	glUseProgram(shaderProgram);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));

	material.setShaderMaterialProperties();

	 glBindTexture(GL_TEXTURE_2D, textureObject);

	// Draw wall
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw
