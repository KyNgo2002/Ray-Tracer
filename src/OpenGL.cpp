#include "OpenGL.h"

OpenGL::OpenGL(float SCR_WIDTH, float SCR_HEIGHT) :
    SCR_WIDTH{ SCR_WIDTH },
    SCR_HEIGHT{ SCR_HEIGHT },
    camera(new Camera(SCR_WIDTH, SCR_HEIGHT)) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ray Tracer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    stbi_set_flip_vertically_on_load(true);

    lastX = SCR_WIDTH / 2.0f;
    lastY = SCR_HEIGHT / 2.0f;
    firstMouse = true;

    // Store "this" pointer in GLFW's user pointer
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0);

}

Camera* OpenGL::getCamera() {
    return camera;
}

GLFWwindow* OpenGL::getWindow() {
    return window;
}

float OpenGL::getScreenWidth() {
    return SCR_WIDTH;
}

float OpenGL::getScreenHeight() {
    return SCR_HEIGHT;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera* camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->setMoved();
        camera->processKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->setMoved();
        camera->processKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->setMoved();
        camera->processKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->setMoved();
        camera->processKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera->setMoved();
        camera->processKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera->setMoved();
        camera->processKeyboard(DOWN, deltaTime);
    }
    if (camera->moved) 
        camera->calculateLookAt();
}

void calculateFPS(unsigned& runningFrameCount, long long& totalFrames) {
    static int frames = 0;
    static float lastTime = 0.0f;
    auto currentTime = GetTickCount64() * 0.001f;

    frames++;
    if (currentTime - lastTime >= 1.0f) {
        totalFrames += frames;
        runningFrameCount++;
        std::cout << "FPS: " << frames << std::endl;
        std::cout << "Average FPS: " << totalFrames / runningFrameCount - 1 << std::endl;
        lastTime = currentTime;
        frames = 1;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    OpenGL* instance = static_cast<OpenGL*>(glfwGetWindowUserPointer(window));

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (instance->firstMouse)
    {
        instance->lastX = xpos;
        instance->lastY = ypos;
        instance->firstMouse = false;
    }

    float xoffset = xpos - instance->lastX;
    float yoffset = instance->lastY - ypos; // reversed since y-coordinates go from bottom to top

    instance->lastX = xpos;
    instance->lastY = ypos;
    
    instance->getCamera()->ProcessMouseMovement(xoffset, yoffset);
    instance->getCamera()->moved = true;
}