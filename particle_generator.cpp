#include <stdio.h>
#include "particle_generator.hpp"
#include "Window.h"
GLuint lastUsedParticle = 0;
particle_generator:: particle_generator()
{
    toWorld = glm::mat4(1.0f);
    // Set up mesh and attribute properties
    GLfloat particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          4, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          4 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    glBindVertexArray(0);
    
    for (GLuint i = 0; i < nr_particles; ++i){
        particles.push_back(Particle());
    }
}
void particle_generator::Update(GLfloat dt, glm::vec3 position, GLuint newParticles, glm::vec3 offset){
    GLuint nr_new_particles = 10;
    // Add new particles
    for (GLuint i = 0; i < nr_new_particles; ++i)
    {
        int unusedParticle = FirstUnusedParticle();
        this->RespawnParticle(this->particles[unusedParticle], position, offset);
    }
    for (GLuint i = 0; i < nr_particles; ++i)
    {
        Particle &p = particles[i];
        p.Life -= (0.5)*dt; // reduce life
        if (p.Life > 0.0f)
        {    // particle is alive, thus update
            p.Position -= p.Velocity * dt;
            p.Color.x += dt * (0.1);
            p.Color.y += dt * (0.1);
            p.Color.z += dt * (0.1);
        }
    }
}
GLuint particle_generator::FirstUnusedParticle(){
    // Search from last used particle, this will usually return almost instantly
    for (GLuint i = lastUsedParticle; i < nr_particles; ++i){
        if (particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedParticle; ++i){
        if (particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Override first particle if all others are alive
    lastUsedParticle = 0;
    return 0;
}
void particle_generator::RespawnParticle(Particle &particle, glm::vec3 position, glm::vec3 offset)
{
    GLfloat rColor = ((rand() % 100) / 100.0f);
    particle.Position =position; //+ random + glm::vec3(offset);
    particle.Color = glm::vec4(rColor, rColor, rColor, 0.75f);
    particle.Life = 1.0f;
	if(!reversed)
		particle.Velocity = glm::vec3((50-(rand()%100) )/ 200.0f, (50-(rand() % 100)) / 200.0f, (rand() % 100) / 50.0f);
	else
		particle.Velocity = glm::vec3((50 - (rand() % 100)) / 200.0f, (50 - (rand() % 100)) / 200.0f, -(rand() % 100) / 50.0f);
}
void particle_generator::Draw(GLuint ShaderProgram){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for (int i=0; i< particles.size(); i++)
    {
        if (particles[i].Life > 0.01f)
        {
            glm::mat4 model = Window::V;
            glm::mat4 view = toWorld;
            // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
            // Consequently, we need to forward the projection, view, and model matrices to the shader programs
            // Get the location of the uniform variables "projection" and "modelview"
            GLuint uModel = glGetUniformLocation(ShaderProgram, "model");
            GLuint uView = glGetUniformLocation(ShaderProgram, "view");
            // Now send these values to the shader program;
            glUniformMatrix4fv(uModel, 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(uView, 1, GL_FALSE, &view[0][0]);
            
            glUniform3fv(glGetUniformLocation(ShaderProgram, "offset"), 1, &(particles[i].Position[0]));
            glUniform4fv(glGetUniformLocation(ShaderProgram, "color"), 1, &(particles[i].Color[0]));
            glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
        //std::cout << i << std::endl;
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
