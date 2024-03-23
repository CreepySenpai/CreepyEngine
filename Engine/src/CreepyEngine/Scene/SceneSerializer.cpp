#include <CreepyEngine/Scene/SceneSerializer.hpp>
#include <CreepyEngine/Scene/Components.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <yaml-cpp/yaml_ex.hpp>

namespace Creepy
{
    SceneSerializer::SceneSerializer(const Ref<Scene>& scene) noexcept : m_scene{scene} {

    }

    static std::string ConvertRigidBodyToString(RigidBody2DComponent::BodyType bodyType) noexcept{
        std::string name;
        switch(bodyType){
            case RigidBody2DComponent::BodyType::STATIC:
                name.assign("Static");
                break;
            case RigidBody2DComponent::BodyType::DYNAMIC:
                name.assign("Dynamic");
                break;
            case RigidBody2DComponent::BodyType::KINEMATIC:
                name.assign("Kinematic");
                break;
        }
        return name;
    }

    static RigidBody2DComponent::BodyType ConvertRigidBodyNameToType(const std::string& name) noexcept {
        if(name == "Static"){
            return RigidBody2DComponent::BodyType::STATIC;
        } else if(name == "Dynamic"){
            return RigidBody2DComponent::BodyType::DYNAMIC;
        }
        return RigidBody2DComponent::BodyType::KINEMATIC;
    }

    static void SerializeEntity(YAML::Emitter& writer, Entity& entity) noexcept {
        writer << YAML::BeginMap;

        writer << YAML::Key << "Entity";
        writer << entity.GetUUID().GetID();       // TODO: Add entity id


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
            
            auto& spriteComponent = entity.GetComponent<SpriteComponent>();
            writer << YAML::Key << "Color" << YAML::Value << spriteComponent.Color;
            writer << YAML::Key << "TilingFactor" << YAML::Value << spriteComponent.TilingFactor;
            
            if(spriteComponent.Texture){
                writer << YAML::Key << "Texture" << YAML::Value << spriteComponent.Texture->GetTexturePath().string();
            }

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

        if(entity.HasComponent<RigidBody2DComponent>()){
            writer << YAML::Key << "RigidBody2DComponent";

            auto& rigid2D = entity.GetComponent<RigidBody2DComponent>();

            {
                writer << YAML::BeginMap;
                writer << YAML::Key << "BodyType" << YAML::Value << ConvertRigidBodyToString(rigid2D.Type);
                writer << YAML::Key << "FixedRotation" << YAML::Value << rigid2D.FixedRotation;
                writer << YAML::EndMap;
            }

        }

        if(entity.HasComponent<BoxCollider2DComponent>()){
            writer << YAML::Key << "BoxCollider2DComponent";

            auto& bxCollider2D = entity.GetComponent<BoxCollider2DComponent>();

            {
                writer << YAML::BeginMap;

                writer << YAML::Key << "Offset" << YAML::Value << bxCollider2D.Offset;
                writer << YAML::Key << "Size" << YAML::Value << bxCollider2D.Size;
                writer << YAML::Key << "Density" << YAML::Value << bxCollider2D.Density;
                writer << YAML::Key << "Friction" << YAML::Value << bxCollider2D.Friction;
                writer << YAML::Key << "Restitution" << YAML::Value << bxCollider2D.Restitution;
                writer << YAML::Key << "RestitutionThreshold" << YAML::Value << bxCollider2D.RestitutionThreshold;
                
                writer << YAML::EndMap;
            }
        }

        writer << YAML::EndMap;
    }

    void SceneSerializer::SerializeToYaml(const std::filesystem::path& filePath) noexcept {
        YAML::Emitter writer;

        writer << YAML::BeginMap;
        writer << YAML::Key << "Scene" << YAML::Value << "Untitled";

        writer << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        m_scene->m_registry.view<entt::entity>().each([&scene = m_scene, &writer = writer](auto entityID){
            Entity entity{entityID, scene.get()};

            if(!entity.IsExits()){
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
    void SceneSerializer::SerializeToBinary(const std::filesystem::path& filePath) noexcept {
        ENGINE_LOG_ERROR("Binary Serializer Not Impl!!!");
    }

    bool SceneSerializer::DeserializeFromYaml(const std::filesystem::path& filePath) noexcept {
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

                auto uuid = entity["Entity"].as<uint64_t>();
                std::string entityName;

                auto&& tagNode = entity["TagComponent"];
                
                if(tagNode){
                    entityName = tagNode["Tag"].as<std::string>();
                }

                Entity deserializeEntity = m_scene->CreateEntity(uuid, entityName);

                auto&& transformNode = entity["TransformComponent"];
                if(transformNode){
                    auto& transform = deserializeEntity.GetComponent<TransformComponent>();
                    transform.Position = transformNode["Position"].as<glm::vec3>();
                    transform.Rotation = transformNode["Rotation"].as<glm::vec3>();
                    transform.Scale = transformNode["Scale"].as<glm::vec3>();
                }

                auto&& spriteNode = entity["SpriteComponent"];
                if(spriteNode){

                    auto& spriteComponent = deserializeEntity.AddComponent<SpriteComponent>();
                    spriteComponent.Color = spriteNode["Color"].as<glm::vec4>();
                    spriteComponent.TilingFactor = spriteNode["TilingFactor"].as<float>();
                    if(spriteNode["Texture"]){
                        spriteComponent.Texture = Texture2D::Create(spriteNode["Texture"].as<std::string>());
                    }

                }

                auto&& cameraNode = entity["CameraComponent"];
                if(cameraNode){
                    auto& sceneCamera = deserializeEntity.AddComponent<CameraComponent>();
                    auto& camera = sceneCamera.Camera;
                    
                    auto&& cameraProperties = cameraNode["Camera"];
                    
                    camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(cameraProperties["ProjectionType"].as<uint32_t>()));

                    camera.SetOrthographicSize(cameraProperties["OrthographicSize"].as<float>());
                    camera.SetOrthographicNearClip(cameraProperties["OrthographicNear"].as<float>());
                    camera.SetOrthographicFarClip(cameraProperties["OrthographicFar"].as<float>());

                    camera.SetPerspectiveVerticalFOV(cameraProperties["PerspectiveFOV"].as<float>());
                    camera.SetPerspectiveNearClip(cameraProperties["PerspectiveNear"].as<float>());
                    camera.SetPerspectiveFarClip(cameraProperties["PerspectiveFar"].as<float>());
                    
                    sceneCamera.IsPrimary = cameraNode["Primary"].as<bool>();
                    sceneCamera.FixedAspectRatio = cameraNode["FixedAspectRatio"].as<bool>();
                }

                auto&& rigidBody2DNode = entity["RigidBody2DComponent"];
                if(rigidBody2DNode){
                    auto& rigidBody2D = deserializeEntity.AddComponent<RigidBody2DComponent>();
                    rigidBody2D.Type = ConvertRigidBodyNameToType(rigidBody2DNode["BodyType"].as<std::string>());
                    rigidBody2D.FixedRotation = rigidBody2DNode["FixedRotation"].as<bool>();
                }

                auto&& boxCollider2DNode = entity["BoxCollider2DComponent"];
                if(boxCollider2DNode){
                    auto& boxCollider2D = deserializeEntity.AddComponent<BoxCollider2DComponent>();
                    boxCollider2D.Offset = boxCollider2DNode["Offset"].as<glm::vec2>();
                    boxCollider2D.Size = boxCollider2DNode["Size"].as<glm::vec2>();
                    boxCollider2D.Density = boxCollider2DNode["Density"].as<float>();
                    boxCollider2D.Friction = boxCollider2DNode["Friction"].as<float>();
                    boxCollider2D.Restitution = boxCollider2DNode["Restitution"].as<float>();
                    boxCollider2D.RestitutionThreshold = boxCollider2DNode["RestitutionThreshold"].as<float>();
                }

            }

        }

        return true;
    }

    bool SceneSerializer::DeserializeFromBinary(const std::filesystem::path& filePath) noexcept {
        ENGINE_LOG_ERROR("Binary Deserializer Not Impl!!!");
        return false;
    }

}