#ifndef _BOMBER_H_
#define _BOMBER_H_

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
#include "OBJObject.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

class Bomber
{
public:
	Bomber(glm::vec3 pos);
	~Bomber();

	glm::vec3 transform;
	OBJObject* plane;
	static float speed;
	static bool turnBack;

	void draw(GLuint);
	void update();
	void moveLeft();
	void moveRight();
	void moveTo(glm::vec3 pos);
};

#endif

