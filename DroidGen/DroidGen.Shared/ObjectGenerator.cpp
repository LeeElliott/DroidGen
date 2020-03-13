#include "ObjectGenerator.h"

ObjectGenerator::ObjectGenerator()
{
}

ObjectGenerator::~ObjectGenerator()
{
}

/*void ObjectGenerator::GenerateObjects(int(&map)[262144], int w, int h, int lCount, int sCount, int eCount, pLimit)
{
	PerimeterMarking(map, w, h);
	// RemoveUnnecessary(map, w, h, 8);
	LargeObjectGeneration(map, w, h, lCount);
	SmallObjectGeneration(map, w, h, sCount);
	EnemyLocationGeneration(map, w, h, eCount);
}*/

void ObjectGenerator::GenerateObjects(int(&map)[262144], float(&heights)[262144], int w, int h, int lCount, int sCount, 
	int eCount, int pLimit, int(&lData)[60], int(&sData)[70], int(&eData)[30], float water)
{
	// Reset size array
	for (int i = 0; i < 45; i++)
	{
		lData[i] = 0;
	}

	PerimeterMarking(map, w, h);
	RemoveUnnecessary(map, w, h, 8);
	LargeObjectGeneration(map, heights, w, h, lCount, lData, water);
	SmallObjectGeneration(map, w, h, sCount, sData);
	EnemyLocationGeneration(map, w, h, eCount, eData);
}

 void ObjectGenerator::PerimeterMarking(int(&map)[262144], int w, int h)
{
	 // Mark playing area boundary
	 for (int j = 0; j < h; j++)
	 {
		 for (int i = 0; i < w; i++)
		 {
			 // Checks if current grid square is not playing area
			 if (map[j * w + i] == 1)
			 {
				 // Maps out edges of playable area by checking if adjacent
				 // to a grid square of playable area
				 if (map[j * w + (i - 1)] == 0 || map[j * w + (i + 1)] == 0
					 || map[(j - 1) * w + i] == 0 || map[(j + 1) * w + i] == 0)
				 {
					 if (i != 0 && i != w - 1 && j != 0 && j != h - 1)
					 {
						 map[j * w + i] = -1;
					 }
				 }
			 }
		 }
	 }
}

 void ObjectGenerator::RemoveUnnecessary(int(&map)[262144], int w, int h, int l)
 {
	 for (int j = 0; j < h; j++)
	 {
		 for (int i = 0; i < w; i++)
		 {
			 if (map[j * w + i] == -1)
			 {
				 bool hLine = true, vLine = true;
				 int next = 1;
				 int counter = 0;

				 while (hLine)
				 {
					 // Check for horizontal continuous lines
					 if (i + next < w && map[j * w + (i + next)] == -1)
					 {
						 // Check if the line continues past next vertex
						 if (map[j * w + (i + next + 1)] == -1)
						 {
							 if (counter < l)
							 {
								 map[j * w + (i + next)] = 1;
								 next++;
								 counter++;
							 }
							 else
							 {
								 next++;
								 counter = 0;
							 }
						 }
						 else
						 {
							 hLine = false;
						 }
					 }
					 else
					 {
						 hLine = false;
					 }
				 }
				 while (vLine)
				 {
					 // Check for vertical continuous lines
					 if (j + next < h && map[(j + next) * w + i] == -1)
					 {
						 // Check if the line continues past next vertex
						 if (map[(j + next + 1) * w + i] == -1)
						 {
							 if (counter < l)
							 {
								 map[(j + next) * w + i] = 1;
								 next++;
								 counter++;
							 }
							 else
							 {
								 next++;
								 counter = 0;
							 }
						 }
						 else
						 {
							 vLine = false;
						 }
					 }
					 else
					 {
						 vLine = false;
					 }
				 }
			 }
		 }
	 }
 }

 void ObjectGenerator::LargeObjectGeneration(int(&map)[262144], int w, int h, int lCount, int(&lData)[60])
 {
	 // Pseudo-random number of large objects
	 std::vector<int> objectlData;

	 for (int i = 0; i < lCount; i++)
	 {
		 objectlData.push_back((rand() % 128 + 48) / 2);
		 objectlData.push_back((rand() % 128 + 48) / 2);
	 }
	
	 // Required to prevent extended hang
	 int attemptCounter = 0;
	 
	 // Loop until size vector is empty
	 while (!objectlData.empty())
	 {
		 // Pseudo-random coordinates
		 int x = rand() % 512;
		 int y = rand() % 512;

		 // If not walkable area
		 if (map[y * w + x] == 1)
		 {
			 // Check if object can fit
			 if (ProximityCheck(map, w, h, x, y, objectlData[objectlData.size() - 2],
				 objectlData[objectlData.size() - 1]))
			 {
				 // Function marks area
				 MarkArea(map, w, h, x, y, objectlData[objectlData.size() - 2], objectlData[objectlData.size() - 1], -2);
				 // Pop the two lData
				 objectlData.pop_back();
				 objectlData.pop_back();
				 // Reset attempt counter
				 attemptCounter = 0;
			 }
			 else
			 {
				 // Increment attempt counter
				 attemptCounter++;

				 // Give up if tried too many times
				 if (attemptCounter > 500)
				 {
					 // Discard remaining lData from vector
					 objectlData.clear();
				 }
			 }
		 }
	 }
 }

 void ObjectGenerator::LargeObjectGeneration(int(&map)[262144], float(&heights)[262144], int w, int h, 
	 int lCount, int(&lData)[60], float water)
 {
	 // Pseudo-random number of large objects
	 std::vector<int> objectlData;

	 for (int i = 0; i < 45; i++)
	 {
		 lData[i] = -1;
	 }

	 int elem = 0;
	 for (int i = 0; i < lCount; i++)
	 {
		 int objectType = rand() % 12;
		 switch (objectType)
		 {
		 default :
			 // Set width
			 objectlData.push_back(16);
			 // Set depth
			 objectlData.push_back(16);
			 // Set object to draw
			 lData[elem] = 16;
			 lData[elem + 1] = 16;
			 elem += 4;
			 break;
		 case 1:
			 // Set width
			 objectlData.push_back(32);
			 // Set depth
			 objectlData.push_back(16);
			 // Set object to draw
			 lData[elem] = 32;
			 lData[elem + 1] = 16;
			 elem += 4;
			 break;
		 case 2:
			 // Set width
			 objectlData.push_back(16);
			 // Set depth
			 objectlData.push_back(32);
			 // Set object to draw
			 lData[elem] = 16;
			 lData[elem + 1] = 32;
			 elem += 4;
			 break;
		 case 3:
			 // Set width
			 objectlData.push_back(16);
			 // Set depth
			 objectlData.push_back(64);
			 // Set object to draw
			 lData[elem] = 32;
			 lData[elem + 1] = 32;
			 elem += 4;
			 break;
		 case 4:
			 // Set width
			 objectlData.push_back(64);
			 // Set depth
			 objectlData.push_back(16);
			 // Set object to draw
			 lData[elem] = 64;
			 lData[elem + 1] = 16;
			 elem += 4;
			 break;
		 case 5:
			 // Set width
			 objectlData.push_back(32);
			 // Set depth
			 objectlData.push_back(32);
			 // Set object to draw
			 lData[elem] = 32;
			 lData[elem + 1] = 32;
			 elem += 4;
			 break;
		 case 6:
			 // Set width
			 objectlData.push_back(64);
			 // Set depth
			 objectlData.push_back(32);
			 // Set object to draw
			 lData[elem] = 64;
			 lData[elem + 1] = 32;
			 elem += 4;
			 break;
		 case 7:
			 // Set width
			 objectlData.push_back(32);
			 // Set depth
			 objectlData.push_back(64);
			 // Set object to draw
			 lData[elem] = 32;
			 lData[elem + 1] = 64;
			 elem += 4;
			 break;
		 case 8:
			 // Set width
			 objectlData.push_back(64);
			 // Set depth
			 objectlData.push_back(64);
			 // Set object to draw
			 lData[elem] = 64;
			 lData[elem + 1] = 64;
			 elem += 4;
			 break;
		 case 9:
			 // Set width
			 objectlData.push_back(96);
			 // Set depth
			 objectlData.push_back(64);
			 // Set object to draw
			 lData[elem] = 96;
			 lData[elem + 1] = 64;
			 elem += 4;
			 break;
		 case 10:
			 // Set width
			 objectlData.push_back(96);
			 // Set depth
			 objectlData.push_back(32);
			 // Set object to draw
			 lData[elem] = 96;
			 lData[elem + 1] = 32;
			 elem += 4;
			 break;
		 }		 
		 
	 }
	
	 // Required to prevent extended hang
	 int attemptCounter = 0;
	 int nextElem = 2;
	 // Loop until size vector is empty
	 while (!objectlData.empty())
	 {
		 // Pseudo-random coordinates
		 int x = rand() % 512;
		 int y = rand() % 512;

		 // If not walkable area
		 if (map[y * w + x] == 1)
		 {
			 // Check if object can fit also using slope
			 if (GradientCheck(map, heights, w, h, x, y, objectlData[objectlData.size() - 2],
				 objectlData[objectlData.size() - 1], water))
			 {
				 // Function marks area
				 MarkArea(map, w, h, x, y, objectlData[objectlData.size() - 2], objectlData[objectlData.size() - 1], -2);

				 // Pop the two lData
				 objectlData.pop_back();
				 objectlData.pop_back();
				 // Reset attempt counter
				 attemptCounter = 0;
				 lData[nextElem] = x;
				 lData[nextElem + 1] = y;
				 nextElem += 4;
			 }
			 else
			 {
				 // Increment attempt counter
				 attemptCounter++;

				 // Give up if tried too many times
				 if (attemptCounter > 5000)
				 {
					 // Discard remaining lData from vector
					 objectlData.clear();
				 }
			 }
		 }
	 }
 }

 void ObjectGenerator::SmallObjectGeneration(int(&map)[262144], int w, int h, int sCount, int(&sData)[70])
 {
	 int counter = 0;
	 // Loop until all objects placed
	 for (int k = 0; k < sCount; k++)
	 {
		 bool siteChosen = false;
		 // Loop until object is successfully positioned		 
		 while (!siteChosen)
		 {
			 // Pseudo-random coordinates
			 int x = rand() % w;
			 int y = rand() % h;

			 // Check that location is within walkable area
			 if (map[y * w + x] == 0)
			 {
				 // Accept as successful may require additional checks
				 siteChosen = true;

				 // Place holder goes here
				 MarkArea(map, w, h, x, y, 8, 8, -4);

				 sData[counter] = x;
				 sData[counter + 1] = y;
				 counter += 2;
			 }
		 }
	 }
 }

 void ObjectGenerator::EnemyLocationGeneration(int(&map)[262144], int w, int h, int eCount, int(&eData)[30])
 {
	 int counter = 0;
	 // Loop until all enemy locations placed
	 for (int k = 0; k < eCount; k++)
	 {
		 bool siteChosen = false;
		 // Loop until enemy is successfully positioned
		 while (!siteChosen)
		 {
			 // Pseudo-random coordinates
			 int x = rand() % w;
			 int y = rand() % h;

			 // Check that location is within walkable area
			 if (map[y * w + x] == 0)
			 {
				 // Accept as successful may require additional checks
				 siteChosen = true;

				 // Place holder goes here
				 MarkArea(map, w, h, x, y, 8, 8, -3);

				 eData[counter] = x;
				 eData[counter + 1] = y;
				 counter += 2;
			 }
		 }
	 }
 }


 bool ObjectGenerator::ProximityCheck(int(&map)[262144], int tw, int th, int x, int y, int w, int h)
 {
	 // Add buffer to object dimensions
	 int width = w + 16; int height = h + 16;
	 
	 // Double loop to check every pixel the object would occupy
	 for (int j = 0; j < height; j++)
	 {
		 for (int i = 0; i < width; i++)
		 {
			 // Calculate pixel position in map
			 int coordinate = (x - (width / 2) + i) + ((y - (height / 2) + j) * tw);

			 // If value is anything other than that of un-walkable
			 // or position is out of bounds
			 if (map[coordinate] != 1 || x - (width / 2) < 1 || x + (width / 2) > tw ||
				 y - (height / 2) < 1 || y + (height / 2) > th)
			 {
				 // Check failed
				 return false;
			 }
		 }
	 }

	 // Check passed
	 return true;
 }

 bool ObjectGenerator::GradientCheck(int(&map)[262144], float(&heights)[262144], int tw, int th, int x, int y, 
	 int w, int h, float water)
 {
	 // Add buffer to object dimensions
	 int width = w + 4; int height = h + 4;
	 
	 // Double loop to check every pixel the object would occupy
	 for (int j = 0; j < h; j++)
	 {
		 for (int i = 0; i < w; i++)
		 {
			 // Horizontal neighbours
			 float x1 = (x - (w / 2)) + (i - 1);
			 float x2 = (x - (w / 2)) + (i + 1);
			 float y1 = heights[(y - (h / 2) + j) * tw + (x - (h / 2) + (i - 1))];
			 float y2 = heights[(y - (h / 2) + j) * tw + (x - (h / 2) + (i + 1))];
			 // Vertical neighbours
			 float x3 = (y - (h / 2)) + (j - 1);
			 float x4 = (y - (h / 2)) + (j + 1);
			 float y3 = heights[(y - (h / 2) + (j - 1)) * tw + (x - (h / 2) + i)];
			 float y4 = heights[(y - (h / 2) + (j + 1)) * tw + (x - (h / 2) + i)];

			 // Get terrain angle in degrees
			 float angleX = atan2(y2 - y1, x2 - x1) * 180 / 3.14159265;
			 float angleY = atan2(y4 - y3, x4 - x3) * 180 / 3.14159265;

			 // If angle is over tolerable value
			 if (angleX > 15 || angleY > 15)
			 {
				 // Check failed
				 return false;
			 }

			 if (heights[y * tw + x] <= water)
			 {
				 return false;
			 }

			 // Calculate pixel position in map
			 int coordinate = (x - (width / 2) + i) + ((y - (height / 2) + j) * tw);

			 // If value is anything other than that of un-walkable
			 // or position is out of bounds
			 if (map[coordinate] != 1 || heights[coordinate] <= water ||x - (width / 2) < 1 || x + (width / 2) > tw ||
				 y - (height / 2) < 1 || y + (height / 2) > th)
			 {
				 // Check failed
				 return false;
			 }
		 }
	 }

	 // Check passed
	 return true;
 }

 void ObjectGenerator::MarkArea(int(&map)[262144], int tw, int th, int x, int y, int w, int h, int v)
 {
	 for (int j = 0; j < h; j++)
	 {
		 for (int i = 0; i < w; i++)
		 {
			 map[(y - (h / 2) + j) * tw + (x - (w / 2) + i)] = v;
		 }
	 }
 }
