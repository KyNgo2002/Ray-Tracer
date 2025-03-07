#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

class Spheres {
private:
    unsigned int VAO, VBO;
    Camera* camera;
    float scrWidth, scrHeight;
    glm::mat4 projection;
    std::vector<glm::mat4> cubeModelMatrices;

public:
    Spheres(Camera* camera, float scrWidth, float scrHeight);
    ~Spheres();
    void draw(Shader* shader);
    void addCube();
    void translateCube(unsigned index, const glm::vec3& translationUnit);
    void scaleCube(unsigned index, const glm::vec3& scaleUnit);
};

