#include "Terrain.h"

float _rotation;
int one;
int limit = 0x40000;

GLint colors[4][4] =
{
    { 0x10000, 0x10000, 0x10000, 0x10000 },
    { 0x10000, 0x10000, 0x10000, 0x10000 },
    { 0x10000, 0x10000, 0x10000, 0x10000 },
    { 0x10000, 0x10000, 0x10000, 0x10000 }
};

GLubyte indices[] =
{
    0, 1, 2,    3, 0, 2
};

Terrain::Terrain()
{
	
}

Terrain::Terrain(int res, int xPos, int yPos)
{
	// Get the offset value
	float off = (2 * limit) / res;

	// Set x and z position
	vertices[0][0] = -limit + (xPos * off);
	vertices[0][1] = -limit;
	vertices[0][2] = -limit + (yPos * off);

	vertices[1][0] = -limit + ((xPos + 1) * off);
	vertices[1][1] = -limit;
	vertices[1][2] = -limit + (yPos * off);

	vertices[2][0] = -limit + ((xPos + 1) * off);
	vertices[2][1] = -limit;
	vertices[2][2] = -limit + ((yPos + 1) * off);

	vertices[3][0] = -limit + (xPos * off);
	vertices[3][1] = -limit;
	vertices[3][2] = -limit + ((yPos + 1) * off);

	indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 3; indices[4] = 0; indices[5] = 2;
}

Terrain::~Terrain()
{
}

void Terrain::SetupGL(double width, double height)
{
    // Initialize GL state.
    glDisable(GL_DITHER);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, width, height);
    GLfloat ratio = (GLfloat)width / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustumf(-ratio, ratio, -1, 1, 1, 10);
}

void Terrain::TearDownGL()
{
}

void Terrain::Update()
{
	_rotation = 1.f;
	//_rotation -= 1.f;
}
void Terrain::Prepare()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Terrain::Draw()
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -8.0f);
    glRotatef(_rotation * 0.25f, 1, 0, 0);  // X

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glFrontFace(GL_CW);
    glVertexPointer(3, GL_FIXED, 0, vertices);
    glColorPointer(4, GL_FIXED, 0, colors);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
	
}

void Terrain::EditHeights(float(&map)[262144])
{

}

void Terrain::SetPosition(int res, int xPos, int yPos)
{
	// Get the offset value
	float off = (2 * limit) / res;

	// Set x and z position
	vertices[0][0] = -limit + (xPos * off);
	vertices[0][2] = -limit + (yPos * off);

	vertices[1][0] = -limit + ((xPos + 1) * off);
	vertices[1][2] = -limit + (yPos * off);

	vertices[2][0] = -limit + ((xPos + 1) * off);
	vertices[2][2] = -limit + ((yPos + 1) * off);

	vertices[3][0] = -limit + (xPos * off);
	vertices[3][2] = -limit + ((yPos + 1) * off);
}
