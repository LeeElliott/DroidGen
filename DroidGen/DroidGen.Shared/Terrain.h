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
	void EditHeights(float(&map)[262144]);
	void SetPosition(int resolution, int xPos, int yPos);
	GLint GetPosition(int a, int b) { return vertices[a][b]; }

private:
	GLint vertices[4][3];
	GLint colors[4][4] =
	{
		{ 0x10000, 0x10000, 0x10000, 0x10000 },
		{ 0x10000, 0x10000, 0x10000, 0x10000 },
		{ 0x10000, 0x10000, 0x10000, 0x10000 },
		{ 0x10000, 0x10000, 0x10000, 0x10000 }
	};
	GLubyte indices[6];
};


#endif /* defined(__HelloTerrainNative__main__) */
