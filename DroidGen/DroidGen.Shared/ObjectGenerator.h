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
		int eCount, int pLimit, int(&lData)[60], int(&sData)[70], int(&eData)[30]);

private:
	void PerimeterMarking(int(&map)[262144], int w, int h);
	void RemoveUnnecessary(int(&map)[262144], int w, int h, int l);
	void LargeObjectGeneration(int(&map)[262144], int w, int h, int lCount, int(&lData)[60]);
	void LargeObjectGeneration(int(&map)[262144], float(&heights)[262144], int w, int h, int lCount, int(&lData)[60]);
	void SmallObjectGeneration(int(&map)[262144], int w, int h, int sCount, int(&sData)[70]);
	void EnemyLocationGeneration(int(&map)[262144], int w, int h, int eCount, int(&eData)[30]);
	

	bool ProximityCheck(int(&map)[262144], int tw, int th, int x, int y, int w, int h);
	bool GradientCheck(int(&map)[262144], float(&heights)[262144], int tw, int th, int x, int y, int w, int h);
	void MarkArea(int(&map)[262144], int tw, int th, int x, int y, int w, int h, int v);
};

