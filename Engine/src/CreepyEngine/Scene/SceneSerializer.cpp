#include <CreepyEngine/Scene/SceneSerializer.hpp>
#include <CreepyEngine/Scene/Components.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <CreepyEngine/Debug/ImGuiLayer.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML {
    template <>
    struct convert<glm::vec3>{
        static Node encode(const glm::vec3& vec){
            Node node;
            node.push_back(vec.x);
            node.push_back(vec.y);
            node.push_back(vec.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& vec){
            // Check Container Vector Data
            if(!node.IsSequence() || node.size() != 3){
                return false;
            }

            vec.x = node[0].as<float>();
            vec.y = node[1].as<float>();
            vec.z = node[2].as<float>();

            return true;
        }
    };

    template <>
    struct convert<glm::vec4>{
        static Node encode(const glm::vec4& vec){
            Node node;
            node.push_back(vec.r);
            node.push_back(vec.g);
            node.push_back(vec.b);
            node.push_back(vec.a);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& vec){
            // Check Container Vector Data
            if(!node.IsSequence() || node.size() != 4){
                return false;
            }

            vec.r = node[0].as<float>();
            vec.g = node[1].as<float>();
            vec.b = node[2].as<float>();
            vec.a = node[3].as<float>();

            return true;
        }
    };
}

namespace Creepy
{

    YAML::Emitter& operator<<(YAML::Emitter& writer, const glm::vec3& vec){
        writer << YAML::Flow;
        writer << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        return writer;
    }

    YAML::Emitter& operator<<(YAML::Emitter& writer, const glm::vec4& vec){
        writer << YAML::Flow;
        writer << YAML::BeginSeq << vec.r << vec.g << vec.b << vec.a << YAML::EndSeq;
        return writer;
    }

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


    // TODO: Remove to another file
    void ImGuiLayer::SaveThemeToYaml(const std::string& filePath) noexcept {
        YAML::Emitter writer;

        writer << YAML::BeginMap;
        writer << YAML::Key << "Theme" << YAML::Value << "Unnamed";

        writer << YAML::Key << "Config" << YAML::BeginMap;

        {
            
            writer << YAML::Key << "WindowBg" << YAML::Value << m_editorConfig.WindowBg;

            writer << YAML::Key << "Header" << YAML::Value << m_editorConfig.Header;

            writer << YAML::Key << "HeaderHovered" << YAML::Value << m_editorConfig.HeaderHovered;
            writer << YAML::Key << "HeaderActive" << YAML::Value << m_editorConfig.HeaderActive;

            writer << YAML::Key << "Button" << YAML::Value << m_editorConfig.Button;
            writer << YAML::Key << "ButtonHovered" << YAML::Value << m_editorConfig.ButtonHovered;
            writer << YAML::Key << "ButtonActive" << YAML::Value << m_editorConfig.ButtonActive;

            writer << YAML::Key << "FrameBg" << YAML::Value << m_editorConfig.FrameBg;
            writer << YAML::Key << "FrameBgHovered" << YAML::Value << m_editorConfig.FrameBgHovered;
            writer << YAML::Key << "FrameBgActive" << YAML::Value << m_editorConfig.FrameBgActive;

            writer << YAML::Key << "Tab" << YAML::Value << m_editorConfig.Tab;
            writer << YAML::Key << "TabHovered" << YAML::Value << m_editorConfig.TabHovered;
            writer << YAML::Key << "TabActive" << YAML::Value << m_editorConfig.TabActive;
            writer << YAML::Key << "TabUnfocused" << YAML::Value << m_editorConfig.TabUnfocused;
            writer << YAML::Key << "TabUnfocusedActive" << YAML::Value << m_editorConfig.TabUnfocusedActive;

            writer << YAML::Key << "TitleBg" << YAML::Value << m_editorConfig.TitleBg;
            writer << YAML::Key << "TitleBgActive" << YAML::Value << m_editorConfig.TitleBgActive;
            writer << YAML::Key << "TitleBgCollapse" << YAML::Value << m_editorConfig.TitleBgCollapse;

            writer << YAML::EndMap;
        }

        writer << YAML::EndMap;

        std::ofstream outFile{filePath};
        outFile << writer.c_str();
        outFile.close();
    }

    bool ImGuiLayer::LoadThemeFromYaml(const std::string& filePath) noexcept {
        std::ifstream stream{filePath};
        std::stringstream strStream;
        strStream << stream.rdbuf();

        auto&& themeData = YAML::Load(strStream.str());

        if(!themeData["Theme"]){
            ENGINE_LOG_ERROR("Root Error");
            return false;
        }

        auto&& config = themeData["Config"];

        if(config){
            m_editorConfig.WindowBg = config["WindowBg"].as<glm::vec4>();

            m_editorConfig.Header = config["Header"].as<glm::vec4>();
            m_editorConfig.HeaderHovered = config["HeaderHovered"].as<glm::vec4>();
            m_editorConfig.HeaderActive = config["HeaderActive"].as<glm::vec4>();

            m_editorConfig.Button = config["Button"].as<glm::vec4>();
            m_editorConfig.ButtonHovered = config["ButtonHovered"].as<glm::vec4>();
            m_editorConfig.ButtonActive = config["ButtonActive"].as<glm::vec4>();

            m_editorConfig.FrameBg = config["FrameBg"].as<glm::vec4>();
            m_editorConfig.FrameBgHovered = config["FrameBgHovered"].as<glm::vec4>();
            m_editorConfig.FrameBgActive = config["FrameBgActive"].as<glm::vec4>();

            m_editorConfig.Tab = config["Tab"].as<glm::vec4>();
            m_editorConfig.TabHovered = config["TabHovered"].as<glm::vec4>();
            m_editorConfig.TabActive = config["TabActive"].as<glm::vec4>();
            m_editorConfig.TabUnfocused = config["TabUnfocused"].as<glm::vec4>();
            m_editorConfig.TabUnfocusedActive = config["TabUnfocusedActive"].as<glm::vec4>();

            m_editorConfig.TitleBg = config["TitleBg"].as<glm::vec4>();
            m_editorConfig.TitleBgActive = config["TitleBgActive"].as<glm::vec4>();
            m_editorConfig.TitleBgCollapse = config["TitleBgCollapse"].as<glm::vec4>();
        }
        else {
            ENGINE_LOG_ERROR("Config Error");
        }

        return true;
    }

}