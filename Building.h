#ifndef _BUILDING_H_
#define _BUILDING_H_

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
#include "BuildingCube.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

class Building
{
public:
	Building(int type, int height, float size, glm::vec3 pos);
	~Building();

	glm::vec3 transform;
	std::vector<BuildingCube*> cubes;
	int type;
	int height;
	float size;
	bool isVisible;

	void draw(GLuint);
	void update();

};

#endif

