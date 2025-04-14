#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "ImGui/imgui.h"
#include "ImGui/backend/imgui_impl_glfw.h"
#include "ImGui/backend/imgui_impl_opengl3.h"

class Scene {
public:
    // Structs for unique shapes and materials
    struct Sphere {
        glm::vec3 position;
        float radius;
        glm::vec3 padding;
        int materialInd;
    };

    struct Plane {
        int firstTriangleInd;
        int secondTriangleInd;
    };

    struct Triangle {
        glm::vec4 x;
        glm::vec4 y;
        glm::vec4 z;
        glm::vec3 normal;
        int materialInd;
    };

    struct Material {
        glm::vec3 color;
        float roughness;
        glm::vec3 emissionColor;
        float emissionPower;
    };

public:
    // Scene variables
    int blurPasses;

	int numSpheres;
	int numTriangles;
	int numPlanes;
    int numMaterials;

    // Edited flags
    bool editedSpheres = false;
    bool editedPlanes = false;
    bool editedTriangles = false;
    bool editedMaterials = false;

    bool doBloom = true;

    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Triangle> newTriangles;
    std::vector<bool> isRegularTriangle;
    std::vector<Plane> planes;
    std::vector<Material> materials;
    std::vector<std::string> materialNames;

    // Shape buffers
    GLuint sphereSSBO;
    GLuint triangleSSBO;
    GLuint materialSSBO;

    // ImGui context
    ImGuiIO* io;

public:
    // Constructors/Destructors
	Scene();
    ~Scene();

    // Setup functions
    void createBuffers();
    void createImGuiEditor(GLFWwindow* window);

    // Getters/Setters
    void addSphere(glm::vec3 position, float radius, int materialInd);
    void addTriangle(glm::vec4 x, glm::vec4 y, glm::vec4 z, glm::vec4 normal, int materialInd);
    void addPlane(glm::vec3 normal, glm::vec3 topLeft, glm::vec3 bottomRight, float materialInd, bool sidePlane);
    void addMaterial(glm::vec3 color, float roughness, glm::vec3 emissionColor, float emissionPower);

    bool checkEdits();
    void handleEdits();

    // Modifiers
    void sendSpheres();
    void sendTriangles();
    void sendMaterials();
    void updatePlanes();

    // Functionality
    void displayEditor();
    void loadModel(const char* path);
    void triangulate(std::vector<std::string>& tokens, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals);

};

