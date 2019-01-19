#ifndef _BLOCK_H_
#define _BLOCK_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Building.h"
#include "Plane.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

class Block
{
public:
	Block(float gridSize, glm::vec3 pos);
	~Block();

	glm::vec3 transform;
	Plane* plane;
	float gridSize;
	int lm;
	vector<vector<glm::vec3>> grid;
	vector<vector<bool>> occupied;
	vector<Building*> buildings;
	vector<Plane*> planes;

	void draw(GLuint);
	void update();
	void rebuild();
	vector<int> detectExplosion(glm::vec3 pos, float radius);
};

#endif

