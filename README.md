# DroidGen
## *Android procedural world generation framework.*

## Contents
1.0 Overview

2.0 Features

2.1 Terrain Displacement

2.2 Area Marking

2.3 Object Placement

2.4 Water Level

3.0 Issues

4.0 Legal

## 1.0 Overview
An Android procedural generation framework created using native development, with C++. This framework has been designed to generate unique 3D worldspace at runtime for use within mobile games.

## 2.0 Features


### 2.1 Terrain Displacement
Although not a true Perlin Noise, the algorithm was created to take in an array of floating-point values and modify them accordingly, using an array of permutation floats as a base 
value. This algorithm differs from true Perlin, as the permutation array used here is equal in 
size to the array of values used to map the terrain and is refilled every time the algorithm is 
run. The implemented algorithm also varies from a true Perlin Noise algorithm, as the 
permutation array is filled with pseudo-random values in the range 0 to 255, whereas in true 
Perlin, an array of 256 elements is filled with the values 0 to 255 in a pseudo random order.

### 2.2 Area Marking
This algorithm was created to take in an array of integer values to be modified, creates an 
array of a smaller scale of 64 by 64 to work upon and initializes every element to the value 1. 
The starting point of the “walk” is pre-set to the coordinates (31, 1) and the finishing 
conditions of the algorithm are that at least one third of the array has been marked with a 
zero and that the path must end adjacent to the “edge of map”. Until both of these 
conditions are met, the algorithm is designed to select one of the four adjacent grid-squares 
using a generated pseudo-random number and, if not already marked, mark it with a zero. 
Once the finishing conditions have been met, the algorithm applies these values to the large-scale array.

### 2.3 Object Placement
The object placement algorithm contains several distinct sub-algorithms for the generation 
of different types of object. This algorithm takes in the integer array filled by the random 
walk function to ensure that objects are placed in an appropriate location.

#### 2.3.1 Boundary Marking
The algorithm iterates through the array, first checking if the current value is equal to 1. If 
this check is passed, another check is performed, to see if any adjacent values are equal to 
zero. Finally, the current array element is checked to ensure that it is not an “edge” element, 
a check that prevents false positives around the edge of the map array.
It was observed that the boundary marking function produced a vast number of marked 
positions, the number of which could be limited to improve usability. An additional function 
was added to facilitate with this. This additional function takes in an integer that is used to 
set the space left between marked array elements. It then iterates through the array to 
check for marked elements. It then checks for concurrent marked horizontal elements, un-marking them unless they are either the last in the line or the space left has met the 
requirements. This process is repeated for concurrent vertical elements.
This algorithm was not enabled during testing, as it was deemed too difficult to represent 
with any clarity in the limited functionality available. This decision was also influenced by the 
consideration of this features effect on the time taken to render the scene.

#### 2.3.2 Small Object Generation
This portion of the algorithm takes in an 
integer counter variable to determine the number 
of small objects to be generated. It then loops until the number of generated objects 
matches the counter. The position coordinates are produced using pseudo-random integer 
generation, which are then checked to ensure that they fall within the playable area of the 
array before being marked.

#### 2.3.3 Large Object Generation
Designed for the purpose of generating large, decorative scenery objects within the world 
space. This was originally designed to take only the integer map array and number of objects 
required. First, the length and width of the objects are generated using pseudo random 
integer generation within a pre-set range. The algorithm then produces position coordinates 
using pseudo-random integer generation, which are then checked to ensure that the 
entirety of the object is out with the playable area.
This was later improved, to also account for the gradient of the generated terrain. This 
required the floating-point value array produced by the Perlin algorithm to be included in 
the parameters. In addition to the check to ensure that the entire object is out with the 
playable area, the new viability check ensures that the gradient of the terrain does not 
exceed 30 degrees at any point within the space that the object would occupy.

#### 2.3.4 Enemy Object Generation
This section of the algorithm takes in an integer counter variable to determine the number 
of small objects to be generated. It then loops until the number of generated objects 
matches the counter. The position coordinates are produced using pseudo-random integer 
generation, which are then checked to ensure that they fall within the playable area of the 
array and do not intersect a small object location, before being marked.

### 2.4 Water Level
The water level is calculated to be only a small percentage lower than the height of the playing area.

## 3.0 Issues
There persists an issue with the z-buffer within this project, which causes objects to erroneously be drawn in front of other objects and also prevents the water from ever being drawn.

## 4.0 Legal
The code contained within this repository has been written by and remains the property of Lee Elliott.

*Last updated on 16/02/20*
