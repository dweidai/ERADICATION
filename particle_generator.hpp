#ifndef particle_generator_hpp
#define particle_generator_hpp

#include <stdio.h>
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>

struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;
    Particle()
    : Position(glm::vec3(0.0f, 0.0f,-10.0f)), Velocity(0.0f), Color(glm::vec4(0.2f,0.2f, 0.2f, 1.0f)), Life(0.0f) { }
};

class particle_generator{
public:
	bool reversed;
    glm::mat4 toWorld;
    GLuint nr_particles = 10000;
    std::vector<Particle> particles;
    GLuint VBO, VAO, texture;
    int width, height, nrChannels;
    GLuint uProjection;
    particle_generator();
    void draw(GLuint shaderProgram);
    GLuint FirstUnusedParticle();
    void RespawnParticle(Particle &particle, glm::vec3 position, glm::vec3 offset);
    void Update(GLfloat dt, glm::vec3 position, GLuint newParticles, glm::vec3 offset);
    void Draw(GLuint ShaderProgram);
};
#endif /* particle_generator_hpp */
