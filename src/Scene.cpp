#include "Scene.h"

Scene::Scene() {
    // Default Scene
    struct Sphere {
        glm::vec3 position;
        float radius;
        glm::vec3 padding;
        int materialInd;
    };
    spheres = {
        {glm::vec3{-5.0f, 10.0f, 0.0f}, 2.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 5},
        {glm::vec3{5.0f, 10.0f, 0.0f}, 2.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 5},
    };

    planes = {
        
    };

    triangles = {
        
    };

    triangleIsPlane = {

    };
    
    materials = {
        {glm::vec3{1.0f, 1.0f, 1.0f}, 0.0f, glm::vec3{1.0f, 1.0f, 1.0f}, 1.0f},
        {glm::vec3{1.0f, 0.0f, 0.0f}, 1.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 0.0f},
        {glm::vec3{0.0f, 0.0f, 1.0f}, 1.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 0.0f},
        {glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 0.0f},
        {glm::vec3{1.0f, 1.0f, 1.0f}, 1.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 0.0f},
        {glm::vec3{1.0f, 1.0f, 1.0f}, 0.0f, glm::vec3{1.0f, 1.0f, 1.0f}, 0.0f},

    };

    materialNames = {
        "Reflective Emissive",
        "Red left panel",
        "Blue right panel",
        "Green bottom Panel",
        "White top panel"
        "Reflective not emissive"
    };

    numSpheres = spheres.size();
    numTriangles = triangles.size();
    numPlanes = planes.size();
    numMaterials = materials.size();

    addPlane(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(10.0f, 0.0f, 10.0f), 3, false);
    addPlane(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-10.0f, 20.0f, -10.0f), glm::vec3(10.0f, 20.0f, 10.0f), 0, false);
    addPlane(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-10.0f, 20.0f, -10.0f), glm::vec3(10.0f, 0.0f, -10.0f), 4, false);
    addPlane(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-10.0f, 20.0f, 10.0f), glm::vec3(-10.0f, 0.0f, -10.0f), 1, true);
    addPlane(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(10.0f, 20.0f, -10.0f), glm::vec3(10.0f, 0.0f, 10.0f), 2, true);

    createBuffers();
}

Scene::~Scene() {
    glDeleteBuffers(1, &sphereSSBO);
    glDeleteBuffers(1, &triangleSSBO);
    glDeleteBuffers(1, &materialSSBO);
}

void Scene::createBuffers() {
    glGenBuffers(1, &sphereSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sphereSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere), spheres.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sphereSSBO);

    glGenBuffers(1, &triangleSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, triangleSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, triangles.size() * sizeof(Triangle), triangles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, triangleSSBO);

    glGenBuffers(1, &materialSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, materials.size() * sizeof(Material), materials.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, materialSSBO);
}

void Scene::createImGuiEditor(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); 

    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void Scene::addPlane(glm::vec3 normal, glm::vec3 topLeft, glm::vec3 bottomRight, float materialInd, bool sidePlane) {
    Triangle firstTriangle;
    Triangle secondTriangle;
    Plane newPlane;

    // top-left -> bottom-left -> bottom->right
    if (sidePlane) {
        firstTriangle.x = glm::vec4(topLeft.x, topLeft.y, topLeft.z, 0.0f);
        firstTriangle.y = glm::vec4(topLeft.x, bottomRight.y, topLeft.z, 0.0f);
        firstTriangle.z = glm::vec4(bottomRight.x, bottomRight.y, bottomRight.z, 0.0f);
    }
    else {
        firstTriangle.x = glm::vec4(topLeft.x, topLeft.y, topLeft.z, 0.0f);
        firstTriangle.y = glm::vec4(topLeft.x, bottomRight.y, bottomRight.z, 0.0f);
        firstTriangle.z = glm::vec4(bottomRight.x, bottomRight.y, bottomRight.z, 0.0f);
    }
    firstTriangle.normal = normal;
    firstTriangle.materialInd = materialInd;
    
    // top-left -> top-right -> bottom-right
    if (sidePlane) {
        secondTriangle.x = glm::vec4(topLeft.x, topLeft.y, topLeft.z, 0.0f);
        secondTriangle.y = glm::vec4(bottomRight.x, topLeft.y, bottomRight.z, 0.0f);
        secondTriangle.z = glm::vec4(bottomRight.x, bottomRight.y, bottomRight.z, 0.0f);
    }
    else {
        secondTriangle.x = glm::vec4(topLeft.x, topLeft.y, topLeft.z, 0.0f);
        secondTriangle.y = glm::vec4(bottomRight.x, topLeft.y, topLeft.z, 0.0f);
        secondTriangle.z = glm::vec4(bottomRight.x, bottomRight.y, bottomRight.z, 0.0f);
    }
    secondTriangle.normal = normal;
    secondTriangle.materialInd = materialInd;

    newPlane.firstTriangleInd = triangles.size();
    triangles.push_back(firstTriangle);
    triangleIsPlane.push_back(true);
    newPlane.secondTriangleInd = triangles.size();
    triangles.push_back(secondTriangle);
    triangleIsPlane.push_back(true);
    
    planes.push_back(newPlane);

    numTriangles += 2;
    numPlanes++;
    sendTriangles();
}

bool Scene::checkEdits() {
    return editedSpheres || editedPlanes || editedTriangles || editedMaterials;
}

void Scene::handleEdits() {
    if (editedSpheres) 
        sendSpheres();
    if (editedPlanes)
        updatePlanes();
    if (editedTriangles)
        sendTriangles();
    if (editedMaterials)
        sendMaterials();
}

void Scene::sendSpheres() {
    editedSpheres = false;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sphereSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere), spheres.data(), GL_DYNAMIC_DRAW);
}

void Scene::sendTriangles() {
    editedTriangles = false;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, triangleSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, triangles.size() * sizeof(Triangle), triangles.data(), GL_DYNAMIC_DRAW);
}

void Scene::sendMaterials() {
    editedMaterials = false;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, materials.size() * sizeof(Material), materials.data(), GL_DYNAMIC_DRAW);
}

void Scene::updatePlanes() {
    editedPlanes = false;
    for (int i = 0; i < numPlanes; ++i) {
        Plane& plane = planes[i];
        Triangle& first = triangles[plane.firstTriangleInd];
        Triangle& second = triangles[plane.secondTriangleInd];
        second.materialInd = first.materialInd;
    }
    sendTriangles();
}

void Scene::displayEditor() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Scene properties");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Sphere Properties")) {
        for (int i = 0; i < numSpheres; ++i) {
            ImGui::Text("Sphere %d", (i + 1));
            ImGui::PushID(i);
            editedSpheres |= ImGui::DragFloat3("Position", glm::value_ptr(spheres[i].position), 0.1f);
            editedSpheres |= ImGui::DragFloat("Radius", &spheres[i].radius, 0.1f);
            editedSpheres |= ImGui::SliderInt("Material #", &spheres[i].materialInd, 0, numMaterials - 1);
            ImGui::Separator();
            ImGui::PopID();
        }
    }

    if (ImGui::CollapsingHeader("Triangle Properties")) {
        int ind = 1;
        for (int i = 0; i < numTriangles; ++i) {
            if (triangleIsPlane[i] == false) {
                ImGui::Text("Triangle %d", (ind++));
                ImGui::PushID(i);
                editedTriangles |= ImGui::DragFloat3("X##xx", glm::value_ptr(triangles[i].x), 0.1f);
                editedTriangles |= ImGui::DragFloat3("Y##xx", glm::value_ptr(triangles[i].y), 0.1f);
                editedTriangles |= ImGui::DragFloat3("Z##xx", glm::value_ptr(triangles[i].z), 0.1f);
                editedTriangles |= ImGui::SliderInt("Material #", &triangles[i].materialInd, 0, numMaterials - 1);
                ImGui::Separator();
                ImGui::PopID();
            }
        }
    }

    if (ImGui::CollapsingHeader("Plane Properties")) {
        for (int i = 0; i < numPlanes; ++i) {
            Plane& plane = planes[i];
            ImGui::Text("Plane %d", (i + 1));
            ImGui::PushID(i);
            /*editedPlanes |= ImGui::DragFloat3("Top left##xx", glm::value_ptr(triangles[plane.firstTriangleInd].x), 0.1f);
            editedPlanes |= ImGui::DragFloat3("Bottom Right##xx", glm::value_ptr(triangles[plane.secondTriangleInd].z), 0.1f);*/
            editedPlanes |= ImGui::SliderInt("Material #", &triangles[plane.firstTriangleInd].materialInd, 0, numMaterials - 1);
            ImGui::Separator();
            ImGui::PopID();
        }
    }

    if (ImGui::CollapsingHeader("Material Properties")) {
        for (int i = 0; i < numMaterials; ++i) {
            ImGui::PushID(i);
            ImGui::Text("%s", materialNames[i].c_str());
            editedMaterials |= ImGui::ColorEdit3("Color##xx", glm::value_ptr(materials[i].color), 0.1f);
            editedMaterials |= ImGui::DragFloat("Roughness##xx", &materials[i].roughness, 0.02f, 0.0f, 1.0f);
            editedMaterials |= ImGui::ColorEdit3("Emission Color##xx", glm::value_ptr(materials[i].emissionColor), 0.1f);
            editedMaterials |= ImGui::DragFloat("Emission Power##xx", &materials[i].emissionPower, 0.01f, 0.0f, 10.0f);
            ImGui::Separator();
            ImGui::PopID();
        }
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
