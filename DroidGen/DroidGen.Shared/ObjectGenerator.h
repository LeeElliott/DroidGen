#pragma once
#include <cstdlib>
#include <math.h>
#include <vector>

class ObjectGenerator
{
public:
	ObjectGenerator();
	~ObjectGenerator();

	//void GenerateObjects(int(&map)[262144], int w, int h, int lCount, int sCount, int eCount, int pLimit);
	void GenerateObjects(int(&map)[262144], float(&heights)[262144], int w, int h, int lCount, int sCount, 
		int eCount, int pLimit, int(&sizes)[45]);

private:
	void PerimeterMarking(int(&map)[262144], int w, int h);
	void RemoveUnnecessary(int(&map)[262144], int w, int h, int l);
	void LargeObjectGeneration(int(&map)[262144], int w, int h, int lCount, int(&sizes)[30]);
	void LargeObjectGeneration(int(&map)[262144], float(&heights)[262144], int w, int h, int lCount, int(&sizes)[45]);
	void SmallObjectGeneration(int(&map)[262144], int w, int h, int sCount);
	void EnemyLocationGeneration(int(&map)[262144], int w, int h, int eCount);
	

	bool ProximityCheck(int(&map)[262144], int tw, int th, int x, int y, int w, int h);
	bool GradientCheck(int(&map)[262144], float(&heights)[262144], int tw, int th, int x, int y, int w, int h);
	void MarkArea(int(&map)[262144], int tw, int th, int x, int y, int w, int h, int v);
};

