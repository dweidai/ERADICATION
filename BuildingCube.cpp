#include "BuildingCube.h"
#include "Window.h"

BuildingCube::BuildingCube(glm::vec3 pos, char* texture, int type)
{
	float side = 1.0f;
	this->type = type;
	float vertices[] = {
		-side, -side, -side,  0.0f, 0.0f,
		side, -side, -side,  1.0f, 0.0f,
		side,  side, -side,  1.0f, 1.0f,
		side,  side, -side,  1.0f, 1.0f,
		-side,  side, -side,  0.0f, 1.0f,
		-side, -side, -side,  0.0f, 0.0f,

		-side, -side,  side,  0.0f, 0.0f,
		side, -side,  side,  1.0f, 0.0f,
		side,  side,  side,  1.0f, 1.0f,
		side,  side,  side,  1.0f, 1.0f,
		-side,  side,  side,  0.0f, 1.0f,
		-side, -side,  side,  0.0f, 0.0f,

		-side,  side,  side,  1.0f, 1.0f,
		-side,  side, -side,  0.0f, 1.0f,
		-side, -side, -side,  0.0f, 0.0f,
		-side, -side, -side,  0.0f, 0.0f,
		-side, -side,  side,  1.0f, 0.0f,
		-side,  side,  side,  1.0f, 1.0f,

		side,  side,  side,  0.0f, 1.0f,
		side,  side, -side,  1.0f, 1.0f,
		side, -side, -side,  1.0f, 0.0f,
		side, -side, -side,  1.0f, 0.0f,
		side, -side,  side,  0.0f, 0.0f,
		side,  side,  side,  0.0f, 1.0f,

		-side, -side, -side,  0.0f, 1.0f,
		side, -side, -side,  1.0f, 1.0f,
		side, -side,  side,  1.0f, 0.0f,
		side, -side,  side,  1.0f, 0.0f,
		-side, -side,  side,  0.0f, 0.0f,
		-side, -side, -side,  0.0f, 1.0f,

		
	};

	float roofVert[] = { 
		-side,  side, -side,  0.0f, 1.0f,
		side,  side, -side,  1.0f, 1.0f,
		side,  side,  side,  1.0f, 0.0f,
		side,  side,  side,  1.0f, 0.0f,
		-side,  side,  side,  0.0f, 0.0f,
		-side,  side, -side,  0.0f, 1.0f };

	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0, side, 0));
	toWorld = glm::translate(toWorld, pos);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		5 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roofVert), roofVert, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
	// texture 1
	// ---------
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

BuildingCube::~BuildingCube()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
}

void BuildingCube::draw(GLuint shaderProgram)
{ 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Window::depthMap);
	
	glUniform1f(glGetUniformLocation(shaderProgram, "shadowOn"), Window::shadowOn);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, &Window::lightSpaceMatrix[0][0]);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 1);
	glUniform1f(glGetUniformLocation(shaderProgram, "type"), 0.0f);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 30);

	glUniform1f(glGetUniformLocation(shaderProgram, "type"), 1.0f);
	glm::vec3 color = glm::vec3(0.7f, 0.7f, 0.7f);
	if(type == 1)
		color = glm::vec3(0.72f, 0.6f, 0.51f);
	if (type == 2)
		color = glm::vec3(0.6f, 0.6f, 0.6f);
	if (type == 3)
		color = glm::vec3(0.8f, 0.78f, 0.72f);
	if (type == 4)
		color = glm::vec3(0.75f, 0.75f, 0.75f);
	glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &color[0]);
	glBindVertexArray(VAO2);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

void BuildingCube::update()
{
	spin(1.0f);
}

void BuildingCube::spin(float deg)
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

