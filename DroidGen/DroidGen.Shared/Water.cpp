#include "Water.h"

Water::Water()
{

}

Water::Water(int res, int xPos, int zPos)
{
	SetPosition(res, xPos, zPos);
}

Water::~Water()
{
}

void Water::SetupGL(double width, double height)
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

void Water::TearDownGL()
{
}

void Water::Update()
{

}
void Water::Prepare()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Water::Draw(float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, z);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnable(GL_POINT_SIZE);
	glPointSize(10.0);

	glFrontFace(GL_CW);
	glVertexPointer(3, GL_FIXED, 0, vertices);
	glColorPointer(4, GL_FIXED, 0, colors);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

}

void Water::EditHeight(float disp)
{
	vertices[0][1] = -0x40000 + (disp * (0x40000 / 3));
	vertices[1][1] = -0x40000 + (disp * (0x40000 / 3));
	vertices[2][1] = -0x40000 + (disp * (0x40000 / 3));
	vertices[3][1] = -0x40000 + (disp * (0x40000 / 3));
}

void Water::SetPosition(int res, int xPos, int zPos)
{
	// Get the offset value
	float off = (2 * 0x40000) / res;

	// Set x and z position
	vertices[0][0] = -0x40000 + (xPos * off);
	vertices[0][1] = -0x40000;
	vertices[0][2] = -0x40000 + (zPos * off);

	vertices[1][0] = -0x40000 + ((xPos + 1) * off);
	vertices[1][1] = -0x40000;
	vertices[1][2] = -0x40000 + (zPos * off);

	vertices[2][0] = -0x40000 + ((xPos + 1) * off);
	vertices[2][1] = -0x40000;
	vertices[2][2] = -0x40000 + ((zPos + 1) * off);

	vertices[3][0] = -0x40000 + (xPos * off);
	vertices[3][1] = -0x40000;
	vertices[3][2] = -0x40000 + ((zPos + 1) * off);

	indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 3; indices[4] = 0; indices[5] = 2;

	colors[0][2] = 0x40000;	colors[1][2] = 0x40000;	colors[2][2] = 0x40000;	colors[3][2] = 0x40000;
}
