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
    // Constructor
    Model(char* path);
    void Draw(Shader* shader);

    std::string directory;
    bool gammaCorrection;

public:
    size_t getNumMeshes();
    size_t getNumTextures();

    void printMeshVertices();
private:
    // model data
    std::vector<Mesh> meshes;
    std::vector<Texture> texturesLoaded;

private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
    
};

