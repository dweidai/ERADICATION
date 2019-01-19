#include "Block.h"
#include "Window.h"

int zombiesEachFloor = 37;

Block::Block(float gridSize, glm::vec3 pos)
{
	this->gridSize = gridSize;
	transform = pos;
	occupied = vector<vector<bool>>(10, vector<bool>(10, false));

	float d = gridSize / 2.0f;
	for (int z = -5; z < 5; z++) {
		vector<glm::vec3> v;
		for (int x = -5; x < 5; x++) {
			glm::vec3 p = glm::vec3(x*gridSize + d, 0, z*gridSize + d) + pos;
			v.push_back(p);
		}
		grid.push_back(v);
	}

	lm = rand() % 4;
	
	if (lm == 1) {
		glm::vec3 t = glm::vec3(2, 0, -1);
		plane = new Plane(pos + t, "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/landmark.jpg");
		plane->toWorld = glm::translate(plane->toWorld, glm::vec3(0, -0.05f, 0));
		plane->scale(6, 3);

		for (int i = 3; i < 9; i++) {
			occupied[3][i] = true;
			occupied[4][i] = true;
			occupied[5][i] = true;
		}
	}
	if (lm == 0) {
		plane = new Plane(pos, "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/parking.jpg");
		plane->toWorld = glm::translate(plane->toWorld, glm::vec3(0, -0.05f, 0));
		//plane->toWorld = glm::rotate(plane->toWorld, glm::radians(90.0f), glm::vec3(0, 1, 0));
		plane->scale(3, 2);
		
		for (int i = 4; i < 6; i++) {
			for (int j = 4; j < 6; j++) {
				occupied[i][j] = true;
			}
		}
	}

	int i1 = rand() % 2 + 2;
	int i2 = rand() % 2 + 4;
	int i3 = rand() % 2 + 6;
	int j1 = rand() % 2 + 2;
	int j2 = rand() % 2 + 4;
	int j3 = rand() % 2 + 6;

	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			if (!occupied[i][j]) {
				if (i == i1 || i == i2 || i==i3 || i == 0 || i == 9) {
					Plane* plane = new Plane(grid[i][j], "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/road_h.jpg");
					planes.push_back(plane);
				}
				else if (j == j1 || j == j2 || j == j3 || j == 0 || j == 9) {
					Plane* plane = new Plane(grid[i][j], "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/road_v.jpg");
					planes.push_back(plane);
				}
				else {
					Building* b = new Building(rand() % 5, rand() % 5 + 2, gridSize / 2.0f, grid[i][j]);
					buildings.push_back(b);
				}
			}
		}
	}
}

Block::~Block()
{
	for (Building* b : buildings) {
		delete(b);
	}
	for (Plane* p : planes) {
		delete(p);
	}
}

void Block::draw(GLuint shaderProgram)
{ 
	if (lm == 1) {
		plane->draw(shaderProgram);
	}
	if (lm == 0) {
		plane->draw(shaderProgram);
	}
	for (Plane* p : planes) {
		p->draw(shaderProgram);
	}
	for (Building* b : buildings) {
		b->draw(shaderProgram);
	}
}

void Block::update()
{
	
}

vector<int> Block::detectExplosion(glm::vec3 pos, float radius) {
	vector<int> res;
	int bds = 0;
	int floors = 0;
	for (Building* b : buildings) {
		float d = glm::length(b->transform-pos);
		if (d <= radius) {
			b->isVisible = false;
			bds++;
			floors += b->height;
		}
	}
	for (Plane* p : planes) {
		glm::vec3 transform = glm::vec3(p->toWorld[3]);
		float d = glm::length(transform - pos);
		if (d <= radius) {
			p->isVisible = false;
		}
	}
	res.push_back(bds);
	res.push_back(floors * zombiesEachFloor);
	return res;
}

void Block::rebuild()
{
	for (Building* b : buildings) {
		delete(b);
	}
	for (Plane* p : planes) {
		delete(p);
	}
	buildings.clear();
	planes.clear();
	occupied = vector<vector<bool>>(10, vector<bool>(10, false));

	lm = rand() % 4;

	if (lm == 1) {
		glm::vec3 t = glm::vec3(2, 0, -1);
		plane = new Plane(transform + t, "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/landmark.jpg");
		plane->toWorld = glm::translate(plane->toWorld, glm::vec3(0, -0.05f, 0));
		plane->scale(6, 3);

		for (int i = 3; i < 9; i++) {
			occupied[3][i] = true;
			occupied[4][i] = true;
			occupied[5][i] = true;
		}
	}
	if (lm == 0) {
		plane = new Plane(transform, "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/parking.jpg");
		plane->toWorld = glm::translate(plane->toWorld, glm::vec3(0, -0.05f, 0));
		plane->scale(3, 2);

		for (int i = 4; i < 6; i++) {
			for (int j = 4; j < 6; j++) {
				occupied[i][j] = true;
			}
		}
	}

	int i1 = rand() % 2 + 2;
	int i2 = rand() % 2 + 4;
	int i3 = rand() % 2 + 6;
	int j1 = rand() % 2 + 2;
	int j2 = rand() % 2 + 4;
	int j3 = rand() % 2 + 6;

	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			if (!occupied[i][j]) {
				if (i == i1 || i == i2 || i == i3 || i == 0 || i == 9) {
					Plane* plane = new Plane(grid[i][j], "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/road_h.jpg");
					planes.push_back(plane);
				}
				else if (j == j1 || j == j2 || j == j3 || j == 0 || j == 9) {
					Plane* plane = new Plane(grid[i][j], "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/road_v.jpg");
					planes.push_back(plane);
				}
				else {
					Building* b = new Building(rand() % 5, rand() % 5 + 2, gridSize / 2.0f, grid[i][j]);
					buildings.push_back(b);
				}
			}
		}
	}
}


