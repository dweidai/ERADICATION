#ifndef OBJOBJECT_H
#define OBJOBJECT_H

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

class OBJObject
{
private:
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    //float angle;
    
public:
    OBJObject(const char* filepath, char* tex);
    ~OBJObject();
    
    glm::mat4 toWorld;
    
    void draw(GLuint);
    void update();
    //void spin(float);
    
    void parse(const char* filepath);
    
    void transit(OBJObject &obj1, OBJObject &obj2);
    std::vector<glm::vec3> getvertices(){
        return vertices;
    }
    float size;
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO, NBO, TBO;
    //GLuint VBO, VAO, EBO, TBO;
    GLuint uProjection, uModelview;
    GLuint uObjcolor;
    unsigned int texture;
    glm::mat4 modelview;
    
    std::vector<int> vertex_indices_;
    std::vector<int> normal_indices_;
    std::vector<int> uv_indices_;
    std::vector<glm::vec3> input_vertices;
    std::vector<glm::vec3> input_normals;
    std::vector<glm::vec2> input_uvs;
    int width, height, nrChannels;
    int count;
};

#endif

