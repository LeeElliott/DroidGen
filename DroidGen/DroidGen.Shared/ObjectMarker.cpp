#include "ObjectMarker.h"

float rotation;
int limits = 0x40000;

ObjectMarker::ObjectMarker()
{

}

ObjectMarker::ObjectMarker(int type, int res, int sizeX, int sizeY, int sizeZ, int xPos, int yPos, int zPos)
{
	SetPosition(type, res, sizeX, sizeY, sizeZ, xPos, yPos, zPos);
}

ObjectMarker::~ObjectMarker()
{
}

void ObjectMarker::SetupGL(double width, double height)
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

void ObjectMarker::TearDownGL()
{
}

void ObjectMarker::Update()
{
	rotation -= 1.f;
}
void ObjectMarker::Prepare()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ObjectMarker::Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -5.2);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glFrontFace(GL_CW);
	glVertexPointer(3, GL_FIXED, 0, vertices);
	glColorPointer(4, GL_FIXED, 0, colors);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

}

void ObjectMarker::EditHeight(float disp)
{
	vertices[0][1] = -limits + (disp * (limits / 5));
	vertices[1][1] = -limits + (disp * (limits / 5));
	vertices[2][1] = -limits + (disp * (limits / 5));
	vertices[3][1] = -limits + (disp * (limits / 5));

	colors[0][2] = 0x10000;
	colors[1][2] = 0x10000;
	colors[2][2] = 0x10000;
	colors[3][2] = 0x10000;
}

void ObjectMarker::EditHeights(float aDisp, float bDisp, float cDisp, float dDisp)
{
	vertices[0][1] = -limits + (aDisp * (limits / 5));
	vertices[1][1] = -limits + (bDisp * (limits / 5));
	vertices[2][1] = -limits + (cDisp * (limits / 5));
	vertices[3][1] = -limits + (dDisp * (limits / 5));

	colors[0][1] = 0x10000 * aDisp;
	colors[1][1] = 0x10000 * bDisp;
	colors[2][1] = 0x10000 * cDisp;
	colors[3][1] = 0x10000 * dDisp;
}

void ObjectMarker::SetPosition(int type, int res, int sizeX, int sizeY, int sizeZ, int xPos, int yPos, int zPos)
{
	// Get the offset value
	float off = (2 * limits) / res;
	
	// TEMPORARY
	sizeY = 64;

	// Set x and z position
	vertices[0][0] = -limits + ((xPos - (sizeX / 2)) * off);
	vertices[0][1] = -limits + (yPos * off);
	vertices[0][2] = -limits + ((zPos - (sizeZ / 2)) * off);

	vertices[1][0] = -limits + ((xPos + (sizeX / 2)) * off);
	vertices[1][1] = -limits + (yPos * off);
	vertices[1][2] = -limits + ((zPos - (sizeZ / 2)) * off);

	vertices[2][0] = -limits + ((xPos + (sizeX / 2)) * off);
	vertices[2][1] = -limits + (yPos * off);
	vertices[2][2] = -limits + ((zPos + (sizeZ / 2)) * off);

	vertices[3][0] = -limits + ((xPos - (sizeX / 2)) * off);
	vertices[3][1] = -limits + (yPos * off);
	vertices[3][2] = -limits + ((zPos + (sizeZ / 2)) * off);

	vertices[4][0] = -limits + ((xPos - (sizeX / 2)) * off);
	vertices[4][1] = -limits + ((yPos + sizeY) * off);
	vertices[4][2] = -limits + ((zPos - (sizeZ / 2)) * off);

	vertices[5][0] = -limits + ((xPos + (sizeX / 2)) * off);
	vertices[5][1] = -limits + ((yPos + sizeY) * off);
	vertices[5][2] = -limits + ((zPos - (sizeZ / 2)) * off);

	vertices[6][0] = -limits + ((xPos + (sizeX / 2)) * off);
	vertices[6][1] = -limits + ((yPos + sizeY) * off);
	vertices[6][2] = -limits + ((zPos + (sizeZ / 2)) * off);

	vertices[7][0] = -limits + ((xPos - (sizeX / 2)) * off);
	vertices[7][1] = -limits + ((yPos + sizeY) * off);
	vertices[7][2] = -limits + ((zPos + (sizeZ / 2)) * off);

	indices[0] = 1;		indices[1] = 0;		indices[2] = 2;		indices[3] = 2;		indices[4] = 0;		indices[5] = 3;
	indices[6] = 7;		indices[7] = 6;		indices[8] = 2;		indices[9] = 2;		indices[10] = 3;	indices[11] = 7;
	indices[12] = 4;	indices[13] = 7;	indices[14] = 3;	indices[15] = 3;	indices[16] = 0;	indices[17] = 4;
	indices[18] = 5;	indices[19] = 4;	indices[20] = 0;	indices[21] = 0;	indices[22] = 1;	indices[23] = 5;
	indices[24] = 6;	indices[25] = 5;	indices[26] = 1;	indices[27] = 1;	indices[28] = 2;	indices[29] = 6;
	indices[30] = 4;	indices[31] = 5;	indices[32] = 6;	indices[33] = 7;	indices[34] = 4;	indices[35] = 6;

	// Switch based on object type
	switch (type)
	{
		// Large objects
	case 0:
		colors[0][0] = 0x10000;
		colors[0][2] = 0x10000;
		colors[1][0] = 0x10000;
		colors[1][2] = 0x10000;
		colors[2][0] = 0x10000;
		colors[2][2] = 0x10000;
		colors[3][0] = 0x10000;
		colors[3][2] = 0x10000;
		colors[4][0] = 0x10000;
		colors[4][2] = 0x10000;
		colors[5][0] = 0x10000;
		colors[5][2] = 0x10000;
		colors[6][0] = 0x10000;
		colors[6][2] = 0x10000;
		colors[7][0] = 0x10000;
		colors[7][2] = 0x10000;
		break;
		// Small objects
	case 1:
		colors[0][0] = 0x10000;
		colors[0][1] = 0x10000;
		colors[1][0] = 0x10000;
		colors[1][1] = 0x10000;
		colors[2][0] = 0x10000;
		colors[2][1] = 0x10000;
		colors[3][0] = 0x10000;
		colors[3][1] = 0x10000;
		colors[4][0] = 0x10000;
		colors[4][1] = 0x10000;
		colors[5][0] = 0x10000;
		colors[5][1] = 0x10000;
		colors[6][0] = 0x10000;
		colors[6][1] = 0x10000;
		colors[7][0] = 0x10000;
		colors[7][1] = 0x10000;
		break;
		// Enemy markers
	case 2:
		colors[0][0] = 0x10000;
		colors[1][0] = 0x10000;
		colors[2][0] = 0x10000;
		colors[3][0] = 0x10000;
		colors[4][0] = 0x10000;
		colors[5][0] = 0x10000;
		colors[6][0] = 0x10000;
		colors[7][0] = 0x10000;
		break;
		// Boundary markers
	case 3:

		break;
	}
}
