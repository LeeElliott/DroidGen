#include "RandomWalk.h"

RandomWalk::RandomWalk()
{
	srand(time(0));
}

RandomWalk::~RandomWalk()
{
}

void RandomWalk::Walk(int(&map)[262144])
{
	// One eighth scale of original map
	int resolution = 64;
	float walkMap[64][64];

	for (int j = 0; j < resolution; j++)
	{
		for (int i = 0; i < resolution; i++)
		{
			walkMap[i][j] = 1;
		}
	}

	int xVal = 31;	int yVal = 1;
	int count = 0;
	bool finished = false;
	
	// Loop until finished
	do
	{
		// Change value of current gridsquare to zero
		if (walkMap[xVal][yVal] == 1)
		{
			walkMap[xVal][yVal] = 0;
			count++;
		}
		
		// Choose one of four directions
		int direction = rand() % 4;

		// Check not too close to edge
		switch (direction)
		{
		case 0:								// East
			if (xVal < resolution - 1)
			{
				xVal++;
			}
			break;
		case 1:								// South
			if (yVal < resolution - 1)
			{
				yVal++;
			}
			break;
		case 2:								// West
			if (xVal > 1)
			{
				xVal--;
			}
			break;
		case 3:								// North
			if (yVal > 1)
			{
				yVal--;
			}
			break;
		}

		// If within two squares of edge
		if (xVal < 2 || xVal > resolution - 2 || yVal < 2 || yVal > resolution - 2)
		{
			// If at least one third is walkable
			if (count > (resolution * resolution) / 3)
			{
				// This ends the loop
				finished = true;
			}
		}
	} while (!finished);
	
	// Loop prevents edges being part of the playing area
	for (int j = 0; j < resolution; j++)
	{
		for (int i = 0; i < resolution; i++)
		{
			if (i == 0 || i == resolution - 1 || j == 0 || j == resolution - 1)
			{
				walkMap[i][j] = 1;
			}
		}
	}

	// This loop sets the playable area in main body
	for (int j = 0; j < 512; j++)
	{
		for (int i = 0; i < 512; i++)
		{
			map[j * 512 + i] = walkMap[i / 8][j / 8];
		}
	}
}
