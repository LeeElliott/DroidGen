#ifndef __HelloTerrainNative__main__
#define __HelloTerrainNative__main__

#include <stdio.h>
#ifdef __ANDROID__
#include <GLES/gl.h>
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#endif

#include <vector>

class Terrain
{
protected:

public:
	Terrain();
	Terrain(int res, int xPos, int yPos);
	~Terrain();

	void SetupGL(double width, double height);
	void TearDownGL();
	void Update();
	void Prepare();
	void Draw();
	void EditHeights(float(&heights)[256], float path);
	void SetPosition(int resolution, int xPos, int zPos);
	GLint GetPosition(int a, int b) { return vertices[a][b]; }

private:
	GLint vertices[256][3];
	GLint colors[256][4] = { { 0x00000, 0x10000, 0x00000, 0x10000,} };
	GLint colors2[256][4] = { { 0x00000, 0x00000, 0x10000, 0x10000,} };
	GLint colors3[256][4] = { { 0x10000, 0x00000, 0x00000, 0x10000,} };
	GLubyte indices[1350];
	
	int myX; int myZ;
};


#endif /* defined(__HelloTerrainNative__main__) */
