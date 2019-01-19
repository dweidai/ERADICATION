#ifndef SPHERE_H
#define SPHERE_H

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
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "stb_image.h"
using namespace std;

class Sphere
{
private:
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec3> out_normals;
	std::vector<glm::vec2> out_uvs;
	float size = 1.0f;

public:
	glm::mat4 toWorld;
	unsigned int texture1;
	bool isVisible;
	int timer;

	Sphere(const char* filepath, char* texture);
	~Sphere();

	void parse(const char* filepath);
	void draw(GLuint);
	void update();
	void explode(glm::vec3 pos);

	GLuint VBO, VAO, EBO, NBO, TBO;
	GLuint uProjection, uModel, uView;
};

#endif