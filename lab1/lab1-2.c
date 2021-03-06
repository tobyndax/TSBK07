// Questions:
// 1) Why don't we need a buffer to upload translMatrix? Do we need buffers for everything that is not uniform (for example different colors)?


// Lab 1-2.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include "MicroGlut_Mac/MicroGlut.h"
// Linking hint for Lightweight IDE
// uses framework Cocoa
#endif
#include "GL_utilities.h"
#include <math.h>

#define PI 3.14159265

// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
	-0.5f,-0.5f,0.0f,
	-0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f
};

GLfloat color[] =
{
	1.0f,1.0f,1.0f,1.0f,
};

GLfloat translMatrix[] =
{
    1.0f, 0.0f, 0.0f, 0.5f,
    0.0f, 1.0f, 0.0f, 0.5f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

GLfloat rotMatrix[] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

GLfloat phi = PI/2;
GLfloat cosPhi;
GLfloat sinPhi;
GLfloat minSin;

// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;
	// Reference to shader program
	GLuint program;

	dumpInfo(); // Dump driver info to stdout

	// GL inits
	glClearColor(0.2,0.2,0.5,0); // Color in buffer upon clear buffer
	glDisable(GL_DEPTH_TEST); // Since 2D I guess?
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-2.vert", "lab1-2.frag"); // These are the programs that run on GPU
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID); // Generate one vertex array object
	glBindVertexArray(vertexArrayObjID); 		// Bind vertex array object to use

	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID); // Generate one vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID); // Bind generated array buffer to GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Upload geometry (vertices) to vertex array buffer

	// Upload program to and enable program (=shaders) on GPU
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		// Choose location of our shader,  vector size 3 (3 scalars per vertice),
		// type of data (GL_FLOAT).
		// The two zeros at the end are offsets between each value and initial offset.

	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
		// Enable program (shader) with position

	// End of upload of geometry

	// ---------------------------------------
	// Upload color
	glUniform4fv(glGetUniformLocation(program, "color"), 1, color);
    
	// ---------------------------------------
    
    // Send translMatrix to Vertex
    glUniformMatrix4fv(glGetUniformLocation(program, "translMatrix"), 1, GL_TRUE, translMatrix);
    
    // Send rotMatrix to Vertex
    cosPhi = cos(phi);
    sinPhi = sin(phi);
    minSin = -sinPhi;
    
    rotMatrix[0] = cosPhi;
    rotMatrix[5] = cosPhi;
    rotMatrix[1] = minSin;
    rotMatrix[4] = sinPhi;
    
    glUniformMatrix4fv(glGetUniformLocation(program, "rotMatrix"), 1, GL_TRUE, rotMatrix);

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen (using chosen color earlier)
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select Vertex Array Oobject
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw 3 vertices from chosen VAO starting with nr 0, counting up

	printError("display");

	glutSwapBuffers(); // Swap buffer so that GPU can use the buffer we uploaded to it and we can write to another
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init ();
	glutMainLoop();
}
