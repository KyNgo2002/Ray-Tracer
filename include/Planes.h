#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

class Planes {
private:
    const float planeData[48] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    };
    unsigned int VAO, VBO;
    Camera* camera;
    float scrWidth, scrHeight;
    glm::mat4 projection;
    std::vector<glm::mat4> planeModelMatrices;

public:
    Planes(Camera* camera, float scrWidth, float scrHeight);
    ~Planes();
    void draw(Shader* shader);
    void addPlane();
    void translatePlane(unsigned index, const glm::vec3& translationUnit);
    void scalePlane(unsigned index, const glm::vec3& scaleUnit);
    void rotatePlane(unsigned index, float angle, const glm::vec3& axis);
};

