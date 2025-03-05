#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <string>
#include <vector>

#include "Shader.h";
#include "Mesh.h"
#include "stb_image.h"

class Model {
public:
    Model(char* path);
    void Draw(Shader* shader);
    std::string directory;
    bool gammaCorrection;

private:
    // model data
    std::vector<Mesh> meshes;
    std::vector<Texture> texturesLoaded;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};

