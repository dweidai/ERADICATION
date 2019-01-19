#ifndef _BOMB_H_
#define _BOMB_H_

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
#include "Sphere.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

class Bomb
{
public:
	Bomb(glm::vec3 pos);
	~Bomb();

	glm::vec3 transform;
	OBJObject* bomb;
	Sphere* sphere;
	bool isVisible;
	float hSpeed, vSpeed;

	void draw(GLuint);
	void update();
	void dropBomb(glm::vec3 pos);

};

#endif

