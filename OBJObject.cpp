#include "OBJObject.h"
#include "Window.h"
#include "stb_image.h"
#include <iostream>
#include <vector>

#define PATH "/Users/apple/Desktop/CSE 167 Final/CSE 167 Final/planeColor.jpg"
OBJObject::OBJObject(const char* filepath, char* tex)
{
    toWorld = glm::mat4(1.0f);
   
    parse(filepath);
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //glGenBuffers(1, &NBO);
    glGenBuffers(1, &TBO);
    
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, (vertices.size()*sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); //haoqi
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
    unsigned char *data = stbi_load(tex, &width, &height, &nrChannels, 0);
    //std::cout << PATH << std::endl;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
}

OBJObject::~OBJObject(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //glDeleteBuffers(1, &NBO);
    glDeleteBuffers(1, &TBO);
}


void OBJObject::parse(const char *filepath)
{
    //parse the whole file
    FILE* fp;     // file pointer
    float x,y,z;  // vertex coordinates
    float u,v;
    int t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12; //triangle
    int c1,c2;    // characters read from file
    c2 = 1;
    //std::cout<< filepath <<"\n";
    fp = fopen(filepath, "rb");
    if (fp==NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }
    while ((c1 = fgetc(fp)) != EOF){
        //then there are several cases, one is vn, the other is v and the other is #
        if(c1=='v'){
            c2 = fgetc(fp);
            if( c2 == 'n'){
                //case that is vn
                fscanf(fp," %f %f %f\n", &x, &y, &z);
                //it takes glm::vec3
                input_normals.push_back(glm::vec3(x,y,z));
            }
            else if (c1 == 'v' && c2 == ' '){
                //case that is v
                fscanf(fp,"%f %f %f\n", &x, &y, &z);
                input_vertices.push_back(glm::vec3(x,y,z));
            }
            else if(c1 == 'v' && c2 == 't'){
                fscanf(fp," %f %f\n", &u, &v);
                input_uvs.push_back(glm::vec2(u,v));
            }
            else{
                while(c2 !='\n'){ // skip a line
                    c2 = fgetc(fp);
                    // ignore the rest
                }
            }
        }
        else if(c1=='f'){
                fscanf(fp," %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &t1, &t2, &t3, &t4, &t5, &t6, &t7, &t8, &t9, &t10, &t11, &t12);
                vertex_indices_.push_back(t1-1);
                normal_indices_.push_back(t3-1);
                uv_indices_.push_back(t2-1);
                vertex_indices_.push_back(t4-1);
                normal_indices_.push_back(t6-1);
                uv_indices_.push_back(t5-1);
                vertex_indices_.push_back(t7-1);
                normal_indices_.push_back(t9-1);
                uv_indices_.push_back(t8-1);
                
                vertex_indices_.push_back(t1-1);
                normal_indices_.push_back(t3-1);
                uv_indices_.push_back(t2-1);
                vertex_indices_.push_back(t7-1);
                normal_indices_.push_back(t9-1);
                uv_indices_.push_back(t8-1);
                vertex_indices_.push_back(t10-1);
                normal_indices_.push_back(t12-1);
                uv_indices_.push_back(t11-1);
        }
        else{
            //case that we ignore the whole line
            while(c2 !='\n'){ // skip a line
                c2 = fgetc(fp);
                // ignore the rest
            }
        }
    }
    fclose(fp);
    /*for(int i=0;i<20; i++){
        std::cout << normal_indices_[i] << std::endl;
    }*/
    for (unsigned i = 0; i < vertex_indices_.size(); i++) {
        vertices.push_back(input_vertices[vertex_indices_[i]]);
        normals.push_back(input_normals[normal_indices_[i]]);
        uvs.push_back(input_uvs[uv_indices_[i]]);
        indices.push_back(i);
    }

    
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
}

void OBJObject::draw(GLuint shaderProgram)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
    
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
    // the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

void OBJObject::update()
{

}


