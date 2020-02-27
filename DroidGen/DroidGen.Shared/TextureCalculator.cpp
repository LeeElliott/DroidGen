#include "TextureCalculator.h"



TextureCalculator::TextureCalculator()
{
}


TextureCalculator::~TextureCalculator()
{
}

void TextureCalculator::ApplyTextures(float(&map)[262144], int w, int h)
{

}

void TextureCalculator::CalculateNormals(float(&map)[262144], int w, int h)
{
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			// Get perpendicular vectors of a plane
			std::vector<float> vectorA; 
			vectorA.push_back(2.0f);
			vectorA.push_back(0.0f);
			vectorA.push_back(map[(i + 1) + (j * w)] - map[(i - 1) + (j * w)]);

			std::vector<float> vectorB; 
			vectorB.push_back(0.0f);
			vectorB.push_back(2.0f);
			vectorB.push_back(map[i + ((j + 1) * w)] - map[i + ((-1) * w)]);

			// Get cross product
			std::vector<float> normal;
			normal[0] = (vectorA[1] * vectorB[2]) - (vectorA[2] * vectorB[1]);
			normal[1] = (vectorA[0] * vectorB[2]) - (vectorA[2] * vectorB[0]);
			normal[2] = (vectorA[0] * vectorB[1]) - (vectorA[1] * vectorB[0]);

			// Normalize normal vector
			// Calculate current length of vector
			float length = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));

			// Divide each element by the calculated length
			normal[0] /= length;
			normal[1] /= length;
			normal[2] /= length;

			// Apply normals to the terrain object here

		}
	}
}
