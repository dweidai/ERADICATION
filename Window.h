#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Building.h"
#include "OBJObject.h"
#include "Bomb.h"
#include "Block.h"
#include "Sphere.h"
#include "Bomber.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "stb_image.h"
#include "particle_generator.hpp"
#include <vector>
#include <irrKlang/irrKlang.h>
using namespace irrklang;
using namespace std;


class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static glm::mat4 lightSpaceMatrix;
	static unsigned int depthMap;
	static float shadowOn;
	static ISoundEngine * soundEngine;
	static bool hasBomb;
	static bool planeView;

	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double x, double y);
	static glm::vec3 trackBallMapping(double x, double y);
	static void print(glm::vec3 &v);
	static unsigned int loadCubemap(vector<string> faces);
	static glm::vec3 getCamPos();
	static void drawSkybox();
	static void createDepthMap();
	static void renderDepthMap();
	static void updateCamera(glm::vec3 pos);
	static void explosion(glm::vec3 pos);
	static void restart();
	static void createCrater(glm::vec3 pos);
};

#endif
