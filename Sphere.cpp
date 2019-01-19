#include "Sphere.h"
#include "Window.h"


Sphere::Sphere(const char *filepath, char* texture) 
{
	toWorld = glm::mat4(1.0f);
	size = 10.0f;
	isVisible = false;
	parse(filepath);
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &TBO);
	glGenBuffers(1, &NBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), out_vertices.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(glm::vec3), out_normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(glm::vec2), out_uvs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);

	// texture 
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(texture, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
}

Sphere::~Sphere() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &TBO);
	glDeleteBuffers(1, &NBO);
}

void Sphere::parse(const char *filepath) 
{
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	float r, g, b;  // vertex color
	int v1, v2, v3, vn1, vn2, vn3, vt1, vt2, vt3;
	
	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) 
	{ 
		cout << "error loading file!" << endl; 
		exit(-1); 
	}  // just in case the file can't be found or is corrupt
	
	while (1) {
		char lineHeader[128];
		int res = fscanf(fp, "%s", lineHeader);
		if (res == EOF) {
			break;
		}

		if (strcmp(lineHeader, "v") == 0)
		{
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			temp_vertices.push_back(glm::vec3(x, y, z)); 
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			temp_normals.push_back(glm::vec3(x, y, z));
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			fscanf(fp, "%f %f", &x, &y);
			temp_uvs.push_back(glm::vec2(x, y));
		}
		else if (strcmp(lineHeader, "f") == 0) {
			fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
			vertexIndices.push_back(v1);
			vertexIndices.push_back(v2);
			vertexIndices.push_back(v3);
			uvIndices.push_back(vt1);
			uvIndices.push_back(vt2);
			uvIndices.push_back(vt3);
			normalIndices.push_back(vn1);		
			normalIndices.push_back(vn2);			
			normalIndices.push_back(vn3);
		}	
	}

	float max_x = temp_vertices[0].x, min_x = temp_vertices[0].x,
		max_y = temp_vertices[0].y, min_y = temp_vertices[0].y,
		max_z = temp_vertices[0].z, min_z = temp_vertices[0].z;
	for (unsigned int i = 0; i < temp_vertices.size(); ++i)
	{
		if (temp_vertices[i].x > max_x) {
			max_x = temp_vertices[i].x;
		}
		else if (temp_vertices[i].x < min_x) {
			min_x = temp_vertices[i].x;
		}
		if (temp_vertices[i].y > max_y) {
			max_y = temp_vertices[i].y;
		}
		else if (temp_vertices[i].y < min_y) {
			min_y = temp_vertices[i].y;
		}
		if (temp_vertices[i].z > max_z) {
			max_z = temp_vertices[i].z;
		}
		else if (temp_vertices[i].z < min_z) {
			min_z = temp_vertices[i].z;
		}
	}
	float dx = (max_x + min_x) / 2.0f;
	float dy = (max_y + min_y) / 2.0f;
	float dz = (max_z + min_z) / 2.0f;

	for (int i = 0; i < temp_vertices.size(); i++) {
		temp_vertices[i] = glm::vec3((temp_vertices[i].x - dx) * size, 
			(temp_vertices[i].y - dy)*size, (temp_vertices[i].z - dz)*size);
	}

	
	// For each triangle
	for (unsigned int v = 0; v < vertexIndices.size(); v += 3)
	{
		// For each vertex of the triangle
		for (unsigned int i = 0; i < 3; i += 1)
		{
			unsigned int vertexIndex = vertexIndices[v + i];
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];

			unsigned int uvIndex = uvIndices[v + i];
			glm::vec2 uv = temp_uvs[uvIndex - 1];

			unsigned int normalIndex = normalIndices[v + i];
			glm::vec3 normal = temp_normals[normalIndex - 1];

			out_vertices.push_back(vertex);
			out_uvs.push_back(uv);
			out_normals.push_back(normal);
		}
	}
	
	fclose(fp);
}

void Sphere::draw(GLuint shaderProgram)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, out_vertices.size());
	glBindVertexArray(0);
}

void Sphere::update()
{
	if (isVisible) {
		toWorld = glm::scale(toWorld, glm::vec3(1.04f, 1.04f, 1.04f));
		timer += 1;
		if (timer >= 90) {
			isVisible = false;
			Window::explosion(glm::vec3(toWorld[3]));
			toWorld = glm::mat4(1.0f);
		}
	}
	
}

void Sphere::explode(glm::vec3 pos)
{
	toWorld[3] = glm::vec4(pos, 1.0f);
	isVisible = true;
	timer = 0;
	Window::soundEngine->play2D("/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/explode.mp3", false, false, true);
}
