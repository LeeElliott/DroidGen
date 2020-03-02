#include "Terrain.h"

float _rotation;
int limit = 0x40000;

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
	_rotation -= 1.f;
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
	//glRotatef(_rotation * 0.25f, 0, 1, 0); // Y
    glRotatef(0.5f, 0, 0, 1);  // Z

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glFrontFace(GL_CW);
    glVertexPointer(3, GL_FIXED, 0, vertices);
    glColorPointer(4, GL_FIXED, 0, colors);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
	
}

void Terrain::EditHeight(float disp)
{
	vertices[0][1] = -limit + (disp * (limit / 5));
	vertices[1][1] = -limit + (disp * (limit / 5));
	vertices[2][1] = -limit + (disp * (limit / 5));
	vertices[3][1] = -limit + (disp * (limit / 5));

	colors[0][2] = 0x10000;
	colors[1][2] = 0x10000;
	colors[2][2] = 0x10000;
	colors[3][2] = 0x10000;
}

void Terrain::EditHeights(float aDisp, float bDisp, float cDisp, float dDisp)
{
	vertices[0][1] = -limit +(aDisp * (limit / 5));
	vertices[1][1] = -limit +(bDisp * (limit / 5));
	vertices[2][1] = -limit +(cDisp * (limit / 5));
	vertices[3][1] = -limit +(dDisp * (limit / 5));

	colors[0][1] = 0x10000 * aDisp;
	colors[1][1] = 0x10000 * bDisp;
	colors[2][1] = 0x10000 * cDisp;
	colors[3][1] = 0x10000 * dDisp;
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

	indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 3; indices[4] = 0; indices[5] = 2;
}
