#ifndef _BUILDINGCUBE_H_
#define _BUILDINGCUBE_H_

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
#include "stb_image.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

class BuildingCube
{
public:
	BuildingCube(glm::vec3 pos, char* texture, int type);
	~BuildingCube();

	glm::mat4 toWorld;
	int type;

	void draw(GLuint);
	void update();
	void spin(float);

	// These variables are needed for the shader program
	GLuint VBO, VAO, VAO2, VBO2;
	GLuint uProjection, uModelview;
	unsigned int texture1;
};

#endif

