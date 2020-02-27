#pragma once

#include <stdlib.h>
#include <math.h>
#include <vector>

class TextureCalculator
{
public:
	TextureCalculator();
	~TextureCalculator();

	void ApplyTextures(float(&map)[262144], int w, int h);

private:
	void CalculateNormals(float(&map)[262144], int w, int h);
	
};

