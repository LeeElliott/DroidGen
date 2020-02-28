#ifndef OBJECT_MARKER_
#define OBJECT_MARKER_

#include <stdio.h>
#ifdef __ANDROID__
#include <GLES/gl.h>
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#endif

#include <vector>

class ObjectMarker
{
protected:

public:
	ObjectMarker();
	ObjectMarker(int type, int res, int sizeX, int sizeY, int sizeZ,  int xPos, int yPos, int zPos);
	~ObjectMarker();

	void SetupGL(double width, double height);
	void TearDownGL();
	void Update();
	void Prepare();
	void Draw();
	void EditHeight(float disp);
	void EditHeights(float aDisp, float bDisp, float cDisp, float dDisp);
	void SetPosition(int type, int res, int sizeX, int sizeY, int sizeZ, int xPos, int yPos, int zPos);
	void SetActive(bool a) { active = a; }
	bool GetActive() { return active; }

private:
	GLint vertices[8][3];
	GLint colors[8][4] =
	{
		{ 0x00000, 0x00000, 0x00000, 0x10000 },
		{ 0x00000, 0x00000, 0x00000, 0x10000 },
		{ 0x00000, 0x00000, 0x00000, 0x10000 },
		{ 0x00000, 0x00000, 0x00000, 0x10000 },
		{ 0x00000, 0x00000, 0x00000, 0x10000 },
		{ 0x00000, 0x00000, 0x00000, 0x10000 },
		{ 0x00000, 0x00000, 0x00000, 0x10000 },
		{ 0x00000, 0x00000, 0x00000, 0x10000 }
	};
	GLubyte indices[36];

	bool active;
};


#endif /* defined(OBJECT_MARKER_) */

