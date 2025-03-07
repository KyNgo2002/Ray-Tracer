#include "Spheres.h"

Spheres::Spheres(Camera* camera, float scrWidth, float scrHeight) :
    camera{ camera },
    scrWidth{ scrWidth },
    scrHeight{ scrHeight },
    projection{ glm::perspective(glm::radians(45.0f), scrWidth / scrHeight, 0.1f, 100.0f) } {

}
