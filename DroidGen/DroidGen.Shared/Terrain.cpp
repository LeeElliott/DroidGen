#include "Terrain.h"

float _rotation;
int limit = 0x40000;

Terrain::Terrain()
{
	
}

Terrain::Terrain(int res, int xPos, int zPos)
{
	// Get the offset value
	SetPosition(res, xPos, zPos);	
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

void Terrain::Draw(float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glTranslatef(x, y, z);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
	glPointSize(10);

    glFrontFace(GL_CW);
    glVertexPointer(3, GL_FIXED, 0, vertices);
    glColorPointer(4, GL_FIXED, 0, colors);
	glDrawElements(GL_TRIANGLES, 1350, GL_UNSIGNED_BYTE, indices);
}

void Terrain::EditHeights(float(&heights)[256], float path)
{
	for (int i = 0; i < 256; i++)
	{
		vertices[i][1] = -limit + (heights[i] * (limit / 3));

		if (heights[i] == path)
		{
			colors[i][0] = 0x5000;
			colors[i][1] = 0x5000;
			colors[i][2] = 0x5000;
			colors[i][3] = 0x10000;
		}
		else if (heights[i] < path)
		{
			colors[i][0] = 0xBA00 + (0x4600 * (path - heights[i]));
			colors[i][1] = 0x5D00 + (0xA300 * (path - heights[i]));
			colors[i][2] = 0x00000;
			colors[i][3] = 0x10000;
		}
		else
		{
			colors[i][0] = 0x00000;
			colors[i][1] = 0x10000 * (heights[i]);
			colors[i][2] = 0x00000;
			colors[i][3] = 0x10000;
		}
	}	
}

void Terrain::SetPosition(int res, int xPos, int zPos)
{
	// Get the offset value
	float off = (2 * limit) / res;
	myX = xPos * 15;
	myZ = zPos * 15;
	
	int n = 0;
	for (int j = 0; j < 16; j++)
	{
		for (int i = 0; i < 16; i++)
		{
			vertices[j * 16 + i][0] = -limit + ((myX + i) * off);
			vertices[j * 16 + i][1] = -limit + 10;
			vertices[j * 16 + i][2] = -limit + ((myZ + j) * off);	

			colors[j * 16 + i][0] = 0x00000;
			colors[j * 16 + i][1] = 0x10000;
			colors[j * 16 + i][2] = 0x00000;
			colors[j * 16 + i][3] = 0x10000;
		}
	}
	
	for (int j = 0; j < 15; j++)
	{
		for (int i = 0; i < 15; i++)
		{
			indices[n] = j * 16 + i;
			indices[n + 1] = j * 16 + (i + 1);
			indices[n + 2] = (j + 1) * 16 + i;
					
			indices[n + 3] = (j + 1) * 16 + (i + 1);
			indices[n + 4] = (j + 1) * 16 + i;
			indices[n + 5] = j * 16 + (i + 1);
			n += 6;
		}
	}
}
