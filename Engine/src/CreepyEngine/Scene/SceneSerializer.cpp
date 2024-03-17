#include <CreepyEngine/Scene/SceneSerializer.hpp>
#include <CreepyEngine/Scene/Components.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <yaml-cpp/yaml_ex.hpp>

namespace Creepy
{
    SceneSerializer::SceneSerializer(const Ref<Scene>& scene) noexcept : m_scene{scene} {

    }

    static void SerializeEntity(YAML::Emitter& writer, Entity& entity) noexcept {
        writer << YAML::BeginMap;

        writer << YAML::Key << "Entity";
        writer << "12345";       // TODO: Add entity id


        if(entity.HasComponent<TagComponent>()){
            writer << YAML::Key << "TagComponent";
            writer << YAML::BeginMap;

            writer << YAML::Key << "Tag" << YAML::Value << entity.GetComponent<TagComponent>().Tag;

            writer << YAML::EndMap;
        }

        if(entity.HasComponent<TransformComponent>()){
            writer << YAML::Key << "TransformComponent";
            writer << YAML::BeginMap;

            auto& transformComponent = entity.GetComponent<TransformComponent>();

            writer << YAML::Key << "Position" << YAML::Value << transformComponent.Position;
            writer << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
            writer << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;

            writer << YAML::EndMap;
        }

        if(entity.HasComponent<SpriteComponent>()){
            writer << YAML::Key << "SpriteComponent";
            writer << YAML::BeginMap;

            writer << YAML::Key << "Color" << YAML::Value << entity.GetComponent<SpriteComponent>().Color;

            writer << YAML::EndMap;
        }

        if(entity.HasComponent<CameraComponent>()){
            writer << YAML::Key << "CameraComponent";
            writer << YAML::BeginMap;

            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& camera = cameraComponent.Camera;

            writer << YAML::Key << "Camera" << YAML::Value;
            
            {   // Camera Property
                writer << YAML::BeginMap;
                writer << YAML::Key << "ProjectionType" << YAML::Value << std::to_underlying(camera.GetProjectionType());
                writer << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
                writer << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
                writer << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();

                writer << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
                writer << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
                writer << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();

                writer << YAML::EndMap;
            }

            {   // Scene Camera Setting
                writer << YAML::Key << "Primary" << YAML::Value << cameraComponent.IsPrimary;
                writer << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
            }
            
            writer << YAML::EndMap;
        }

        writer << YAML::EndMap;
    }

    void SceneSerializer::SerializeToYaml(const std::string& filePath) noexcept {
        YAML::Emitter writer;

        writer << YAML::BeginMap;
        writer << YAML::Key << "Scene" << YAML::Value << "Untitled";

        writer << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        m_scene->m_registry.view<entt::entity>().each([&scene = m_scene, &writer = writer](auto entityID){
            Entity entity{entityID, scene.get()};

            if(entity.m_entityHandle == entt::null || entity.m_scene == nullptr){
                return;
            }

            SerializeEntity(writer, entity);
        });

        writer << YAML::EndSeq;
        writer << YAML::EndMap;

        std::ofstream fileOut{filePath};
        fileOut << writer.c_str();
        fileOut.close();
    }
    void SceneSerializer::SerializeToBinary(const std::string &filePath) noexcept {
        ENGINE_LOG_ERROR("Binary Serializer Not Impl!!!");
    }

    bool SceneSerializer::DeserializeFromYaml(const std::string &filePath) noexcept {
        std::ifstream stream{filePath};
        std::stringstream strStream;
        strStream << stream.rdbuf();
        
        YAML::Node sceneData = YAML::Load(strStream.str());
        
        // Check File Begin Name
        if(!sceneData["Scene"]){
            return false;
        }

        auto&& sceneName = sceneData["Scene"].as<std::string>();

        auto&& entities = sceneData["Entities"];

        // Check Entities Root Node Has Entity inside
        if(entities){

            for(auto&& entity : entities){

                uint64_t entityID = entity["Entity"].as<uint64_t>();    // 
                std::string entityName;

                auto&& tagComponent = entity["TagComponent"];
                if(tagComponent){
                    entityName = tagComponent["Tag"].as<std::string>();
                }

                Entity deserializeEntity = m_scene->CreateEntity(entityName);

                auto&& transformComponent = entity["TransformComponent"];
                if(transformComponent){
                    auto& transform = deserializeEntity.GetComponent<TransformComponent>();
                    transform.Position = transformComponent["Position"].as<glm::vec3>();
                    transform.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                    transform.Scale = transformComponent["Scale"].as<glm::vec3>();
                }

                auto&& spriteComponent = entity["SpriteComponent"];
                if(spriteComponent){
                    auto& color = deserializeEntity.AddComponent<SpriteComponent>().Color;
                    color = spriteComponent["Color"].as<glm::vec4>();
                }

                auto&& cameraComponent = entity["CameraComponent"];
                if(cameraComponent){
                    auto& sceneCamera = deserializeEntity.AddComponent<CameraComponent>();
                    auto& camera = sceneCamera.Camera;
                    
                    auto&& cameraProperties = cameraComponent["Camera"];
                    
                    camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(cameraProperties["ProjectionType"].as<uint32_t>()));

                    camera.SetOrthographicSize(cameraProperties["OrthographicSize"].as<float>());
                    camera.SetOrthographicNearClip(cameraProperties["OrthographicNear"].as<float>());
                    camera.SetOrthographicFarClip(cameraProperties["OrthographicFar"].as<float>());

                    camera.SetPerspectiveVerticalFOV(cameraProperties["PerspectiveFOV"].as<float>());
                    camera.SetPerspectiveNearClip(cameraProperties["PerspectiveNear"].as<float>());
                    camera.SetPerspectiveFarClip(cameraProperties["PerspectiveFar"].as<float>());
                    
                    sceneCamera.IsPrimary = cameraComponent["Primary"].as<bool>();
                    sceneCamera.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                }
            }

        }

        return true;
    }

    bool SceneSerializer::DeserializeFromBinary(const std::string &filePath) noexcept {
        ENGINE_LOG_ERROR("Binary Deserializer Not Impl!!!");
        return false;
    }

}