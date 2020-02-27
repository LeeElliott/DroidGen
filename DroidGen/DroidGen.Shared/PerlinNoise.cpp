#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
{
}

PerlinNoise::~PerlinNoise()
{
}

void PerlinNoise::GeneratePerlin(int w, int h, float(&map)[262144])
{
	width = w;
	height = h;

	GeneratePermutations();
	GenerateNoise(4, 0.5f, map);
	GenerateAdditional(1, 0.15f, map);
}

void PerlinNoise::ShuffleArray()
{
	
}

void PerlinNoise::GeneratePermutations()
{
	for (int j = 0; j < 512; j++)
	{
		for (int i = 0; i < 512; i++)
		{
			float randomValue = rand() % 255;
			permutations[i][j] = randomValue / 256.0f;
		}
	}
}

void PerlinNoise::GenerateNoise(int it, float scalar, float(&map)[262144])
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float noise = 0.0f;
			float scale = 1.0f;
			float scaleAcc = 0.0f;

			for (int i = 0; i < it; i++)
			{
				// Distance between significant pixels
				int pitch = width >> it;
				
				// Calculate significant pixel locations
				int x1 = (x / pitch) * pitch;
				int y1 = (y / pitch) * pitch;

				int x2 = (x1 + pitch) % width;
				int y2 = (y1 + pitch) % width;

				// Calculate weightings based on distance from significant pixels
				float blendX = (float)(x - x1) / (float)pitch;
				float blendY = (float)(y - y1) / (float)pitch;

				// Calculate top and bottom samples using horizontal weighting
				float sampleT = (1.0f - blendX) * permutations[x1][y1] + blendX * permutations[x2][y1];
				float sampleB = (1.0f - blendX) * permutations[x1][y2] + blendX * permutations[x2][y2];

				// Increase accumulative scalar
				scaleAcc += scale;
				
				// Calculate noise value using samples and vertical weighting
				noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
				scale = scale / scalar;
			}

			// Scale to acceptable range
			map[y * width + x] = noise / scaleAcc;
		}
	}
}

void PerlinNoise::GenerateAdditional(int it, float scalar, float(&map)[262144])
{
	for (int y = 0; y < width; y++)
	{
		for (int x = 0; x < height; x++)
		{
			float noise = 0.0f;
			float scale = 1.0f;
			float scaleAcc = 0.0f;

			for (int i = 0; i < it; i++)
			{
				int pitch = width >> it;
				int x1 = (x / pitch) * pitch;
				int y1 = (y / pitch) * pitch;

				int x2 = (x1 + pitch) % width;
				int y2 = (y1 + pitch) % width;

				float blendX = (float)(x - x1) / (float)pitch;
				float blendY = (float)(y - y1) / (float)pitch;

				float sampleT = (1.0f - blendX) * permutations[x1][y1] + blendX * permutations[x2][y1];
				float sampleB = (1.0f - blendX) * permutations[x1][y2] + blendX * permutations[x2][y2];

				scaleAcc += scale;
				noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
				scale = scale / scalar;
			}

			// Scale to seed range
			map[y * width + x] *= noise / scaleAcc;
		}
	}
}

