#include "OpenGL.h"

static void glfw_error_callback(int error, const char* description);

// Constructor
OpenGL::OpenGL(float SCR_WIDTH, float SCR_HEIGHT)
    : SCR_WIDTH{ SCR_WIDTH },
      SCR_HEIGHT{ SCR_HEIGHT },
      camera(new Camera(SCR_WIDTH, SCR_HEIGHT)) {

    // GLFW Initialization
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ray Tracer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Glad Initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
        std::cout << "Failed to initialize GLAD" << std::endl;

    stbi_set_flip_vertically_on_load(true);

    // Mouse state
    lastX = SCR_WIDTH / 2.0f;
    lastY = SCR_HEIGHT / 2.0f;
    firstMouse = true;
    editingMode = false;

    // Store "this" pointer in GLFW user pointer
    glfwSetWindowUserPointer(window, this);

    // Callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!editingMode)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // OpenGL config
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0);
}

// Getters functions
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

// Toggle editing mode
void OpenGL::changeEditingMode() {
    editingMode = !editingMode;
    camera->changeEditingMode();
    if (editingMode)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// Framebuffer size callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Handle user input
void OpenGL::processInput(float deltaTime) {
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
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (ePressed == false) {
            ePressed = true;
            changeEditingMode();
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
        ePressed = false;
    }
    if (camera->moved) 
        camera->calculateLookAt();
}

// Mouse movement callback
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    OpenGL* instance = static_cast<OpenGL*>(glfwGetWindowUserPointer(window));

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (instance->firstMouse) {
        instance->lastX = xpos;
        instance->lastY = ypos; 
        instance->firstMouse = false;
    }

    float xoffset = xpos - instance->lastX;
    float yoffset = instance->lastY - ypos; 

    instance->lastX = xpos;
    instance->lastY = ypos;
    
    if (!instance->editingMode) {
        instance->getCamera()->ProcessMouseMovement(xoffset, yoffset);
        instance->getCamera()->moved = true;
    }
}

// GLFW error callback function
static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}