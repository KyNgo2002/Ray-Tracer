#include "Scene.h"

Scene::Scene() {
    // Default Scene
	numSpheres = 6;
	numTriangles = 1;
	numPlanes = 1;
    numMaterials = 5;

    spheres = {
        {glm::vec3{-6.0f, 0.0f, 0.0f}, 1.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 2},
        {glm::vec3{-3.0f, 0.0f, 0.0f}, 1.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 3},
        {glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 4},
        {glm::vec3{0.0f, -101.f, 0.0f}, 100.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 1},
        {glm::vec3{2.0f, 0.0f, 0.0f}, 1.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 0},
        {glm::vec3{-4.0f, 0.0f, -93.0f}, 56.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 0}
    };

    planes = {
        {glm::vec3{0.0f, 1.0f, 0.0f}, 1, glm::vec2{-10.0f, 10.0f},
            glm::vec2{-10.0f, 10.0f}, glm::vec2{-10.0f, 10.0f}, 1.0f}
    };

    triangles = {
        {glm::vec4{-1.5f, 2.0f, 2.0f, -1.0f}, glm::vec4{1.5f, 2.0f, 2.0f, -1.0f},
        glm::vec4{0.0f, 5.0f, 2.0f, -1.0f}, glm::vec4{0.0f, 0.0f, 1.0f, -1.0f}, 2}
    };
    
    materials = {
        {glm::vec3{1.0f, 1.0f, 1.0f}, 0.0f, glm::vec3{1.0f, 1.0f, 1.0f}, 2.0f},
        {glm::vec3{1.0f, 1.0f, 1.0f}, 0.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f},
        {glm::vec3{1.0f, 0.0f, 1.0f}, 0.1f, glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f},
        {glm::vec3{0.0f, 1.0f, 0.0f}, 0.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f},
        {glm::vec3{1.0f, 0.0f, 0.0f}, 0.0f, glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f}
    };

    std::vector<std::string> materialNames = {
        "Emissive",
        "Plane",
        "Purple Sphere",
        "Green Sphere",
        "Red Sphere"
    };

    createBuffers();
}

Scene::~Scene() {
    glDeleteBuffers(1, &sphereSSBO);
    glDeleteBuffers(1, &planeSSBO);
    glDeleteBuffers(1, &triangleSSBO);
    glDeleteBuffers(1, &materialSSBO);
}

void Scene::createBuffers() {
    glGenBuffers(1, &sphereSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sphereSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere), spheres.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sphereSSBO);

    glGenBuffers(1, &planeSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, planeSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, planes.size() * sizeof(Plane), planes.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, planeSSBO);

    glGenBuffers(1, &triangleSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, triangleSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, triangles.size() * sizeof(Triangle), triangles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, triangleSSBO);

    glGenBuffers(1, &materialSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, materials.size() * sizeof(Material), materials.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, materialSSBO);
}

void Scene::createImGuiEditor(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
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

void Scene::dislayEditor() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Scene properties");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Sphere Properties")) {
        for (int i = 0; i < spheres.size(); ++i) {
            ImGui::Text("Sphere %d", (i + 1));
            ImGui::PushID(i);
            editedSpheres |= ImGui::DragFloat3("Position", glm::value_ptr(spheres[i].position), 0.1f);
            editedSpheres |= ImGui::DragFloat("Radius", &spheres[i].radius, 0.1f);
            ImGui::Separator();
            ImGui::PopID();
        }
    }
    if (ImGui::CollapsingHeader("Plane Properties")) {
        for (int i = 0; i < planes.size(); ++i) {
            ImGui::Text("Plane %d", (i + 1));
            ImGui::PushID(i);
            editedPlanes |= ImGui::DragFloat3("Normal##xx", glm::value_ptr(planes[i].normal), 0.1f);
            editedPlanes |= ImGui::DragFloat("Distance##xx", &planes[i].dist, 0.1f);
            ImGui::Separator();
            ImGui::PopID();
        }
    }

    if (ImGui::CollapsingHeader("Triangle Properties")) {
        for (int i = 0; i < triangles.size(); ++i) {
            ImGui::Text("Triangle %d", (i + 1));
            ImGui::PushID(i);
            editedTriangles |= ImGui::DragFloat3("X##xx", glm::value_ptr(triangles[i].x), 0.1f);
            editedTriangles |= ImGui::DragFloat3("Y##xx", glm::value_ptr(triangles[i].y), 0.1f);
            editedTriangles |= ImGui::DragFloat3("Z##xx", glm::value_ptr(triangles[i].z), 0.1f);
            ImGui::Separator();
            ImGui::PopID();
        }
    }
    if (ImGui::CollapsingHeader("Material Properties")) {
        for (int i = 0; i < materials.size(); ++i) {
            ImGui::PushID(i);
            ImGui::Text("Material %d", (i + 1));
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

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
