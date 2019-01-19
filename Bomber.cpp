#include "Bomber.h"
#include "Window.h"

float height = 25.0f;
bool Bomber::turnBack = false;
float Bomber::speed = 0.1f;

Bomber::Bomber(glm::vec3 pos)
{
	transform = pos;
	plane = new OBJObject("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/plane.obj", "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/planeColor.jpg");
	glm::mat4 tmp = plane->toWorld;
	tmp = glm::translate(tmp, pos);
	tmp = glm::scale(tmp, glm::vec3(0.4f, 0.4f, 0.4f));
	//tmp = glm::translate(tmp, glm::vec3(0, 0, 15));
	tmp = glm::rotate(tmp, glm::radians(90.0f), glm::vec3(0, 1, 0));
	tmp = glm::rotate(tmp, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	plane->toWorld = tmp;
}

Bomber::~Bomber()
{
	
}

void Bomber::draw(GLuint shaderProgram)
{ 
	plane->draw(shaderProgram);
}

void Bomber::update()
{
	if (!turnBack) {
		if (transform.y <= height) {
			transform.y += speed * 1.5;
			transform.z += speed;
		}
		else if (transform.z <= 70.0f) {
			transform.z += speed * 1.5;
		}
		else {
			turnBack = true;
			speed = -speed;
			plane->toWorld = glm::rotate(plane->toWorld, glm::radians(180.0f), glm::vec3(0, 0, 1));
			Window::hasBomb = true;
			Window::planeView = true;
		}
	}
	else if(transform.z >= -1000.0f){
		transform.z += speed * 1.5;
	}
	plane->toWorld[3] = glm::vec4(transform, 1.0f);
}

void Bomber::moveLeft() {

		transform.x += 1.6*speed;
	plane->toWorld[3] = glm::vec4(transform, 1.0f);

}

void Bomber::moveRight() {
	
		transform.x -= 1.6*speed;
	plane->toWorld[3] = glm::vec4(transform, 1.0f);
}

void Bomber::moveTo(glm::vec3 pos) {
	transform = pos;
	plane->toWorld[3] = glm::vec4(transform, 1.0f);
}

