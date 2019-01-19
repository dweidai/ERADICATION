#include "Window.h"

const char* window_title = "ERADICATION";
GLint shaderProgram;
GLint skyboxShader;
GLint depthShader;
GLint modelShader;
GLint shadowMapShader;
GLint particleShader;
int mouse = 0;
glm::vec3 lastPos, curPos;

Cube * cube;
Bomb * bomb;
Bomber* bomber;
Building * building;
Block* block;
Sphere* sphere;
vector<Block*> blocks;
glm::vec3 blockPos[3][3];
Plane* mapPlane;
//Plane* crater;
Plane* ground;
bool showShadowMap = false;
bool canMove = false;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/shader.vert"
#define FRAGMENT_SHADER_PATH "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/shader.frag"

// Default camera parameters
glm::vec3 cam_pos(40.0f, 40.0f, 0.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is
//glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;
const glm::mat4 I = glm::mat4(1.0f);

vector<std::string> faces
{
	"/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/right2.png",
	"/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/left2.png",
	"/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/up2.png",
	"/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/down2.png",
	"/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/front2.png",
	"/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/back2.png"
};
unsigned int skyboxTexture;

unsigned int SHADOW_WIDTH = 1600, SHADOW_HEIGHT = 900;
unsigned int depthMapFBO;
unsigned int Window::depthMap;
glm::mat4 Window::lightSpaceMatrix;
glm::mat4 lightProjection = glm::ortho(-35.0f, 35.0f, -30.0f, 30.0f, -20.0f, 40.0f);
glm::vec3 lightPos = glm::vec3(10, 15, -5);
glm::vec3 bomberStartPos = glm::vec3(0, 0, -70);
bool drawCity = true;
bool Window::hasBomb = true;
float Window::shadowOn = 1.0f;
float explosionRadius = 20.0f;
bool Window::planeView = false;
vector<Sphere*> craters;

ISoundEngine * Window::soundEngine = createIrrKlangDevice();
ISound * bgm;
ISound * engine;
ISound * bombSound;

particle_generator * smokeL;
particle_generator * smokeR;

int total_bds = 0;
int total_kills = 0;

void Window::initialize_objects()
{
	srand(3);
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShader = LoadShaders("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/skybox.vert", "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/skybox.frag");
	depthShader = LoadShaders("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/depthShader.vert", "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/depthShader.frag");
	modelShader = LoadShaders("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/model.vert", "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/model.frag");
	shadowMapShader = LoadShaders("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/shadowMap.vert", "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/shadowMap.frag");
	particleShader = LoadShaders("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/particle.vert", "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/particle.frag");
	smokeL = new particle_generator();
	smokeR = new particle_generator();
	cube = new Cube();
	skyboxTexture = loadCubemap(faces);
	bomber = new Bomber(bomberStartPos);
	bomb = new Bomb(glm::vec3(0, 25, 0));
	//building = new Building(4, 4, 1.0f, glm::vec3(0, 0, 0));
	mapPlane = new Plane(glm::vec3(60, 0, 0), "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/building.jpg");
	mapPlane->isShadowMap = true;
	mapPlane->toWorld = glm::scale(mapPlane->toWorld, glm::vec3(10, 0, 10));
	
	ground = new Plane(glm::vec3(0,-0.1f,0), "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/ground3.jpg");
	ground->scale(60, 60);

	if (drawCity) {
		for (int z = -1; z < 2; z++) {
			for (int x = -1; x < 2; x++) {
				glm::vec3 pos = glm::vec3(x*18.0f, 0, z*18.0f);
				Block* blk = new Block(2.0f, pos);
				blocks.push_back(blk);
			}
		}
	}
	else {
		block = new Block(2.0f, glm::vec3(0, 0, 0));
	}

	createDepthMap();
	bgm = soundEngine->play2D("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/BGM.flac", true, false, true);
	engine = soundEngine->play2D("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/engine.wav", true, false, true);
	bgm->setVolume(0.5f);
	engine->setVolume(0.4f);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
    delete(cube);
    delete(bomb);
    delete(bomber);
    delete(building);
    delete(block);
    delete(sphere);
    for(int i=0; i< blocks.size(); i++){
        delete(blocks.at(i));
    }
    delete(mapPlane);
    delete(ground);
    delete(smokeL);
    delete(smokeR);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyboxShader);
	glDeleteProgram(modelShader);
	glDeleteProgram(depthShader);
	glDeleteProgram(shadowMapShader);
	soundEngine->drop();
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glEnable(GL_CULL_FACE);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	SHADOW_HEIGHT = height;
	SHADOW_WIDTH = width;

	if (height > 0)
	{
		P = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.01f, 2000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	if (canMove) {
		bomber->update();
		bomb->update();
		if (planeView) {
			updateCamera(bomber->transform);
		}
		if (!Bomber::turnBack) {
			smokeL->reversed = false;
			smokeR->reversed = false;
			smokeL->Update(0.1f, bomber->transform + glm::vec3(2.5, 0.5f, -5.5), 2, glm::vec3(1.0f, 1.0f, 0.0f));
			smokeR->Update(0.1f, bomber->transform + glm::vec3(-1.3, 0.5f, -5.5), 2, glm::vec3(1.0f, 1.0f, 0.0f));
		}
		else {
			smokeL->reversed = true;
			smokeR->reversed = true;
			smokeL->Update(0.1f, bomber->transform + glm::vec3(1.3, 0.5f, 5.5), 2, glm::vec3(1.0f, 1.0f, 0.0f));
			smokeR->Update(0.1f, bomber->transform + glm::vec3(-2.5, 0.5f, 5.5), 2, glm::vec3(1.0f, 1.0f, 0.0f));
		}
	}
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	renderDepthMap();

	glUseProgram(modelShader);
	bomber->draw(modelShader);
	bomb->draw(modelShader);
	for (Sphere* c : craters) {
		c->draw(modelShader);
	}

	glUseProgram(particleShader);
	smokeL->Draw(particleShader);
	smokeR->Draw(particleShader);
	
	if (showShadowMap) {
		glUseProgram(shadowMapShader);
		mapPlane->draw(shadowMapShader);
	}

	glUseProgram(shaderProgram);
	//crater->draw(shaderProgram);
	ground->draw(shaderProgram);
	
	if (drawCity) {
		for (auto blk : blocks) {
			blk->draw(shaderProgram);
		}
	}
	else {
		block->draw(shaderProgram);
	}

	drawSkybox();

	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float speed = 0.1f;
	if (action == GLFW_REPEAT) {

		if (key == GLFW_KEY_W) {
			glm::vec3 dir = cam_pos - cam_look_at;
			cam_pos -= dir * speed;
			cam_look_at -= dir * speed;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
		if (key == GLFW_KEY_S) {
			glm::vec3 dir = cam_pos - cam_look_at;
			cam_pos += dir * speed;
			cam_look_at += dir * speed;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		
		if (key == GLFW_KEY_UP) {
			cam_look_at += glm::vec3(0, speed, 0);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
		if (key == GLFW_KEY_DOWN) {
			cam_look_at += glm::vec3(0, -speed, 0);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
		if (key == GLFW_KEY_LEFT) {
			bomber->moveLeft();
		}
		if (key == GLFW_KEY_RIGHT) {
			bomber->moveRight();
		}
		
	}
	
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		if (key == GLFW_KEY_R) {
			restart();
		}
		if (key == GLFW_KEY_Q) {
			if (shadowOn == 1.0f)
				shadowOn = 0.0f;
			else
				shadowOn = 1.0f;
		}
		if (key == GLFW_KEY_M) {
			showShadowMap = !showShadowMap;
		}
		if (key == GLFW_KEY_SPACE) {
			if (hasBomb) {
				glm::vec3 bias = glm::vec3(0.6f, 0, -3);
				if(Bomber::turnBack)
					bias = glm::vec3(0.6f, 0, 3);
				bomb->dropBomb(bomber->transform + bias);
				hasBomb = false;
				planeView = false;
				//ground->toWorld = glm::rotate(ground->toWorld, glm::radians(90.0f), glm::vec3(0, 1, 0));
				cam_pos = glm::vec3(-60, 25, 0);
				cam_look_at = glm::vec3(0, 7, 0);
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
				engine->setVolume(0.25f);
				bombSound = soundEngine->play2D("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/falling.wav", false, false, true);
				bombSound->setVolume(0.2f);
			}
		}
		if (key == GLFW_KEY_P) {
			planeView = !planeView;
		}
		if (key == GLFW_KEY_O) {
			canMove = !canMove;
		}
	}
}


void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
		mouse = 1;
		
	}
	else {
		mouse = 0;
	}
}

void Window::cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	if (mouse == 1) {
		lastPos = trackBallMapping(x, y);
		mouse = 2;
	}
	else if (mouse == 2) {
		curPos = trackBallMapping(x, y);
		glm::vec3 rotAxis = glm::cross(lastPos, curPos);
		float angle = asin(glm::length(rotAxis)) / 50.0f;
		glm::mat4 R = glm::rotate(glm::mat4(1.0f), angle, rotAxis);

		//glm::vec3 cam_dir = cam_pos - cam_look_at;
		//cam_dir = glm::vec3(glm::vec4(cam_dir, 0.0f) * R);
		glm::vec3 d = cam_pos - cam_look_at;
		glm::vec4 temp = glm::vec4(d, 1.0f)*R;

		cam_look_at = cam_pos - glm::vec3(temp);
		
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

glm::vec3 Window::trackBallMapping(double x, double y) {
	glm::vec3 v(0.0f);

	v.x = (2 * x - width) / width;
	v.y = (height - 2*y) / height;

	float d = glm::length(v);
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d*d);
	v = glm::normalize(v);

	return v;
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0) {
		glm::vec3 dir = cam_pos - cam_look_at;
		cam_pos -= dir * 0.05f;
		cam_look_at -= dir * 0.05f;
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
	else if (yoffset < 0) {
		glm::vec3 dir = cam_pos - cam_look_at;
		cam_pos += dir * 0.05f;
		cam_look_at += dir * 0.05f;
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}


void Window::print(glm::vec3 &v) {
	cout << v.x << " " << v.y << " " << v.z << endl;
}

glm::vec3 Window::getCamPos() {
	return cam_pos;
}

void Window::drawSkybox() {
	glUseProgram(skyboxShader);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "model"), 1, GL_FALSE, &cube->toWorld[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glBindVertexArray(cube->VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int Window::loadCubemap(vector<string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			cout << "Cubemap texture failed to load at path: " << endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Window::createDepthMap() {
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::renderDepthMap() {
	// render depth of scene to texture (from light's perspective)
	glm::mat4 lightView;
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	glUseProgram(depthShader);
	glUniformMatrix4fv(glGetUniformLocation(depthShader, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//building->draw(depthShader);
	//plane->draw(depthShader);
	
	if (drawCity) {
		for (auto blk : blocks) {
			blk->draw(depthShader);
		}
	}
	else {
		block->draw(depthShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::updateCamera(glm::vec3 pos) {
	cam_look_at = pos;
	if(!Bomber::turnBack)
		cam_pos = cam_look_at + glm::vec3(0, 12, -18);
	else
		cam_pos = cam_look_at + glm::vec3(0, 12, 18);

	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

void Window::explosion(glm::vec3 pos) {
	
	createCrater(pos);
	
	for (auto blk : blocks) {
		vector<int> v = blk->detectExplosion(pos, explosionRadius);
		total_bds += v[0];
		total_kills += v[1];
	}
	if (Bomber::turnBack) {
		cout << "--------------------------------------" << endl;
		cout << "Mission Report:" << endl;
		cout << "You destroyed " << total_bds << " buildings." << endl;
		cout << "You eliminated " << total_kills << " infecteds." << endl;
		cout << "Your rating for this mission: ";
		if (total_kills > 24000)
			cout << "A" << endl;
		else if (total_kills > 22000)
			cout << "B" << endl;
		else if (total_kills > 18000)
			cout << "C" << endl;
		else
			cout << "D" << endl;
		cout << "--------------------------------------" << endl;
	}
}

void Window::restart() {
	cout << "Rebuilding city..." << endl;
	bgm->stop();
	bgm->drop();
	engine->stop();
	engine->drop();

	craters.clear();
	ground->toWorld = glm::rotate(ground->toWorld, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	bomber->moveTo(bomberStartPos);
	hasBomb = true;
	planeView = true;
	if (drawCity) {
		for (auto blk : blocks) {
			blk->rebuild();
		}
	}
	else {
		block->rebuild();
	}
	renderDepthMap();

	cout << "Rebuild completed!" << endl;
	bgm = soundEngine->play2D("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/BGM.flac", true, false, true);
	engine = soundEngine->play2D("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/engine.wav", true, false, true);
	bgm->setVolume(0.5f);
	engine->setVolume(0.4f);
}

void Window::createCrater(glm::vec3 pos) {
	Sphere* sphere = new Sphere("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/sphere.obj", "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/crater.jpg");
	sphere->toWorld = glm::translate(sphere->toWorld, glm::vec3(0, 0.1f, 0));
	sphere->toWorld = glm::translate(sphere->toWorld, pos);
	sphere->toWorld = glm::scale(sphere->toWorld, glm::vec3(25, 0.1f, 25));
	craters.push_back(sphere);
}
