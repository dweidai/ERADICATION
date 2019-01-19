#ifndef _PLANE_H_
#define _PLANE_H_

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

class Plane
{
public:
	Plane(glm::vec3 pos, char* texture);
	~Plane();

	glm::mat4 toWorld;
	int type;
	bool isShadowMap;
	bool isVisible;

	void draw(GLuint);
	void scale(int x, int z);
	void changePos(glm::vec3 pos);

	// These variables are needed for the shader program
	GLuint VBO, VAO, VAO2, VBO2;
	GLuint uProjection, uModelview;
	unsigned int texture1;
};

#endif

