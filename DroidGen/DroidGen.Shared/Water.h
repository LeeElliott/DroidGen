#ifndef WATER_
#define WATER_

#include <stdio.h>
#ifdef __ANDROID__
#include <GLES/gl.h>
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#endif

#include <vector>

class Water
{
protected:

public:
	Water();
	Water(int res, int xPos, int zPos);
	~Water();

	void SetupGL(double width, double height);
	void TearDownGL();
	void Update();
	void Prepare();
	void Draw();
	void EditHeight(float disp);
	void SetPosition(int resolution, int xPos, int zPos);
	void SetActive(bool a) { active = a; }
	bool GetActive() { return active; }

private:
	GLint vertices[4][3];
	GLint colors[4][4] =
	{
		{ 0x00000, 0x00000, 0x00000, 0x10000 },
		{ 0x00000, 0x00000, 0x00000, 0x10000 },
		{ 0x00000, 0x00000, 0x00000, 0x10000 },
		{ 0x00000, 0x00000, 0x00000, 0x10000 }
	};
	GLubyte indices[6];

	bool active;
};


#endif /* defined(WATER_) */

