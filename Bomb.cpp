#include "Bomb.h"
#include "Window.h"

Bomb::Bomb(glm::vec3 pos)
{
	transform = pos;
	bomb = new OBJObject("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/bomb.obj", "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/bomb.jpg");
	sphere = new Sphere("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/sphere.obj", "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/explosion.jpg");
	glm::mat4 tmp = bomb->toWorld;
	tmp = glm::translate(tmp, pos);
	tmp = glm::rotate(tmp, glm::radians(90.0f), glm::vec3(0, 1, 0));
	bomb->toWorld = tmp;
	isVisible = false;
	hSpeed = 0.15f;
	vSpeed = 0.0f;
}

Bomb::~Bomb()
{
	
}

void Bomb::draw(GLuint shaderProgram)
{ 
	if (isVisible) {
		bomb->draw(shaderProgram);
	}
	if (sphere->isVisible) {
		sphere->draw(shaderProgram);
	}
	
}

void Bomb::update()
{
	if (isVisible) {
		transform.y -= vSpeed;
		transform.z += Bomber::speed * 1.5f;
		vSpeed += 0.002;
		bomb->toWorld[3] = glm::vec4(transform, 1.0f);
		if (transform.y <= 0) {
			isVisible = false;
			sphere->explode(glm::vec3(transform.x, 0, transform.z));
		}
	}
	sphere->update();
}

void Bomb::dropBomb(glm::vec3 pos) {
	transform = pos;
	bomb->toWorld[3] = glm::vec4(transform, 1.0f);
	if (Bomber::turnBack) {
		bomb->toWorld = glm::rotate(bomb->toWorld, glm::radians(180.0f), glm::vec3(0, 1, 0));
	}
	isVisible = true;
	vSpeed = 0.0f;
}


