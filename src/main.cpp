#undef GLFW_DLL
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>

#include "Libs/stb_image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cmath>

#include "Libs/Shader.h"
#include "Libs/Window.h"
#include "Libs/Mesh.h"

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

//Vertex Shader
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateTriangle()
{
    GLfloat vertices[] =
    {
        //pos                   //TexCoord
        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        0.0, -1.0f, 1.0f,       0.5f, 0.0f,
        1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,       0.5f, 1.0f
    };

    unsigned int indices[] = 
    {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    for (int i = 0; i < 10; i++)
    {
        Mesh *obj1 = new Mesh();
        obj1->CreateMesh(vertices, indices, 20, 12);
        meshList.push_back(obj1);
    }
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(shader1);
}

int main()
{
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();

    CreateTriangle();
    CreateShaders();

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width1, height1, nrChannels1;
    unsigned char* data1 = stbi_load("Textures/container.jpg", &width1, &height1, &nrChannels1, 0);
    
    if (!data1)
    {
        std::cerr << "Failed to load texture: " << stbi_failure_reason() << "\n";
    }
    else
    {
        // Handle 1/3/4 channel images correctly
        GLenum format = GL_RGBA;
        if (nrChannels1 == 1) format = GL_RED;
        else if (nrChannels1 == 3) format = GL_RGB;
        else if (nrChannels1 == 4) format = GL_RGBA;
        
        // Avoid row-alignment issues for non-4-byte widths
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width1, height1, 0, format, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
       
        stbi_image_free(data1);
    }
    
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width2, height2, nrChannels2;
    unsigned char* data2 = stbi_load("Textures/paper.png", &width2, &height2, &nrChannels2, 0);
    
    if (!data2)
    {
        std::cerr << "Failed to load texture: " << stbi_failure_reason() << "\n";
    }
    else
    {
        // Handle 1/3/4 channel images correctly
        GLenum format = GL_RGBA;
        // if (nrChannels1 == 1) format = GL_RED;
        // else if (nrChannels1 == 3) format = GL_RGB;
        // else if (nrChannels1 == 4) format = GL_RGBA;
        
        // Avoid row-alignment issues for non-4-byte widths
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width2, height2, 0, format, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
       
        stbi_image_free(data2);
    }
        

    const glm::vec3 pyramidPositions[] = {
        glm::vec3(0.0f, 0.0f, -2.5f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    glm::vec3 cameraPos = glm::vec3(1.0f, 0.5f, 2.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, -0.3f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    //Loop until window closed
    while (!mainWindow.getShouldClose())
    {
        //Get + Handle user input events
        glfwPollEvents();
            
        //Clear window
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(mainWindow.getWindow(), GLFW_TRUE);
        }

        //draw here
        shaderList[0]->UseShader();

        glm::vec3 cameraFront(glm::normalize(cameraTarget - cameraPos));
        glm::vec3 cameraRight(glm::normalize(glm::cross(cameraFront, up)));
        glm::vec3 cameraUp(glm::cross(cameraRight, cameraFront));

        // Camera
        glm::mat4 view(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
        glm::mat4 projection(glm::perspective(45.0f, (GLfloat)WIDTH / HEIGHT, 0.1f, 1000.0f));

        GLuint model_location = shaderList[0]->GetUniformLocation("model");
        GLuint view_location = shaderList[0]->GetUniformLocation("view");
        GLuint projection_location = shaderList[0]->GetUniformLocation("projection");
        GLint uniform_texture1 = shaderList[0]->GetUniformLocation("texture1");
        GLint uniform_texture2 = shaderList[0]->GetUniformLocation("texture2"); 

        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        for (size_t i = 0; i < meshList.size(); i++) {
            glm::mat4 model(1.0f);

            model = glm::translate(model, pyramidPositions[i]);
            model = glm::rotate(model, glm::radians(2.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(0.8f, 0.8f, 1.0f));

            glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
       
            glUniform1i(uniform_texture1, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);

            glUniform1i(uniform_texture2, 1);
            glActiveTexture(GL_TEXTURE1); 
            glBindTexture(GL_TEXTURE_2D, texture2);
            
            //Object
            meshList[i]->RenderMesh();
        }

        glUseProgram(0);
        
        //end draw
        mainWindow.swapBuffers();
    }

    // Cleanup
    for (auto &mesh: meshList) {
        delete mesh;
    }
    
    for (auto &shader: shaderList) {
        delete shader;
    }

    return 0;
}
