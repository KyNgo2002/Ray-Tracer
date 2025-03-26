#pragma once

#include <vector>
#include <string>

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
    struct Sphere {
        glm::vec3 position;
        float radius;
        glm::vec3 padding;
        int materialInd;
    };

    struct Plane {
        glm::vec3 normal;
        int materialInd;
        glm::vec2 xMax;
        glm::vec2 yMax;
        glm::vec2 zMax;
        float dist;
    };

    struct Triangle {
        glm::vec4 x;
        glm::vec4 y;
        glm::vec4 z;
        glm::vec4 normal;
        int materialInd;
    };

    struct Material {
        glm::vec3 color;
        float roughness;
        glm::vec3 emissionColor;
        float emissionPower;
    };

public:
	int numSpheres;
	int numTriangles;
	int numPlanes;
    int numMaterials;

    bool editedSpheres = false;
    bool editedPlanes = false;
    bool editedTriangles = false;
    bool editedMaterials = false;

    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Plane> planes;
    std::vector<Material> materials;
    std::vector<std::string> materialNames;

    GLuint sphereSSBO;
    GLuint planeSSBO;
    GLuint triangleSSBO;
    GLuint materialSSBO;

    ImGuiIO io;

public:
    // Constructors/Destructors/Setup
	Scene();
    ~Scene();
    void createBuffers();
    void createImGuiEditor(GLFWwindow* window);

    // Getters/Setters
    void addSphere();
    void addTriangle();
    void addPlane();
    void addMaterial();

    bool checkEdits();

    // Modifiers
    void sendSpheres();
    void sendTriangles();
    void sendMaterials();

    // Functionality
    void dislayEditor();

};

