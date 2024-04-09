#include <CreepyEngine/Utils/ModelImporterUtils.hpp>
#include <CreepyEngine/Renderer/Model.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Creepy::Utils {

    struct ModelImporterStorage{
        Assimp::Importer Importer;
    };

    static ModelImporterStorage s_modelImporterStorage;

    Creepy::Mesh ProcessMesh(aiMesh* mesh, const aiScene* modelScene) noexcept {

        std::vector<Creepy::Vertex> vertices;
        std::vector<uint32_t> indices;
        vertices.reserve(mesh->mNumVertices);

        for(uint32_t i{}; i < mesh->mNumVertices; i++){

            Creepy::Vertex vertex;
            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;
            vertex.Position.w = 1.0f;               // for transform

            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;

            if(mesh->mTextureCoords[0]){

                vertex.TextureCoord.x = mesh->mTextureCoords[0][i].x;
                vertex.TextureCoord.y = mesh->mTextureCoords[0][i].y;
            }
            else{
                vertex.TextureCoord.x = 0.0f;
                vertex.TextureCoord.y = 0.0f;
            }

            vertices.emplace_back(vertex);

        }

        for(uint32_t i{}; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];
            for(uint32_t j{}; j < face.mNumIndices; j++){

                indices.emplace_back(face.mIndices[j]);

            }
        }

        return {vertices, indices};
    }

    void ProcessNode(aiNode* node, const aiScene* modelScene, std::vector<Ref<Mesh>>& totalMesh) noexcept {
        
        for(uint32_t i{}; i < node->mNumMeshes; i++){
            
            auto subMesh = modelScene->mMeshes[node->mMeshes[i]];

            totalMesh.emplace_back(std::make_shared<Mesh>(ProcessMesh(subMesh, modelScene)));
        }


        // Recursive
        for(uint32_t i{}; i < node->mNumChildren; i++){
            ProcessNode(node->mChildren[i], modelScene, totalMesh);
        }

    }

    std::vector<Ref<Creepy::Mesh>> ModelImporter::LoadMesh(const std::filesystem::path& filePath) noexcept {

        if(!std::filesystem::exists(filePath)){
            ENGINE_LOG_ERROR("Model Path Dont Exits: {}", filePath.string());
            return {};
        }

        auto modelScene = s_modelImporterStorage.Importer.ReadFile(filePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!modelScene || modelScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !modelScene->mRootNode){
            ENGINE_LOG_ERROR("Model Error: {}", s_modelImporterStorage.Importer.GetErrorString());
            return {};
        }

        ENGINE_LOG_WARNING("Has Model: {}", filePath.string());

        std::vector<Ref<Mesh>> totalMesh;

        ProcessNode(modelScene->mRootNode, modelScene, totalMesh);

        s_modelImporterStorage.Importer.FreeScene();

        return totalMesh;
    }
    
    Creepy::Model ModelImporter::LoadModel(const std::filesystem::path& filePath) noexcept {
        return {LoadMesh(filePath)};
    }

}