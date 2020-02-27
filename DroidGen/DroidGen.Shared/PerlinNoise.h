#pragma once
#include <stdlib.h>

class PerlinNoise
{
public:
	PerlinNoise();
	~PerlinNoise();

	void GeneratePerlin(int w, int h, float(&map)[262144]);

private:
	void ShuffleArray();
	void GeneratePermutations();
	void GenerateNoise(int it, float scalar, float(&map)[262144]);
	void GenerateAdditional(int it, float scalar, float(&map)[262144]);


	int width;
	int height;
	float permutations[512][512];
};

