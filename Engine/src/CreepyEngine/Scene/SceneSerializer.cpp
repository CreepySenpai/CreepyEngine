#include <CreepyEngine/Scene/SceneSerializer.hpp>
#include <CreepyEngine/Scene/Components.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <CreepyEngine/Utils/ScriptEngineUtils.hpp>
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

        if(entity.HasComponent<ScriptComponent>()){
            writer << YAML::Key << "ScriptComponent";
            writer << YAML::BeginMap;

            writer << YAML::Key << "ScriptName" << YAML::Value << entity.GetComponent<ScriptComponent>().ScriptName;

            {
                auto& fieldData = ScriptEngine::GetScriptFieldData();

                if(fieldData.contains(entity.GetUUID())){
                    
                    writer << YAML::Key << "ScriptFields" << YAML::Value;

                    writer << YAML::BeginSeq;

                    // Format

                    //    - Name: 
                    //      Type:
                    //      Data:
                    for(auto& [fieldName, fieldValue] : fieldData.at(entity.GetUUID())){
                        writer << YAML::BeginMap;
                        
                        writer << YAML::Key << "Name" << YAML::Value << fieldName;

                        writer << YAML::Key << "Type" << YAML::Value << Utils::ConvertFieldTypeToString(fieldValue.DataType);
                        
                        writer << YAML::Key << "Data";

                        switch(fieldValue.DataType){
                            case ScriptFieldDataType::BOOL: {
                                writer << YAML::Value << fieldValue.GetValue<bool>();
                                break;
                            }
                            case ScriptFieldDataType::BYTE: {
                                writer << YAML::Value << fieldValue.GetValue<uint8_t>();
                                break;
                            }
                            case ScriptFieldDataType::SBYTE:
                            case ScriptFieldDataType::CHAR: {
                                writer << YAML::Value << fieldValue.GetValue<char>();
                                break;
                            }

                            case ScriptFieldDataType::SHORT: {
                                writer << YAML::Value << fieldValue.GetValue<short>();
                                break;
                            }

                            case ScriptFieldDataType::USHORT: {
                                writer << YAML::Value << fieldValue.GetValue<uint16_t>();
                                break;
                            }

                            case ScriptFieldDataType::INT: {
                                writer << YAML::Value << fieldValue.GetValue<int>();
                                break;
                            }

                            case ScriptFieldDataType::UINT: {
                                writer << YAML::Value << fieldValue.GetValue<uint32_t>();
                                break;
                            }

                            // TODO: Change or remove
                            case ScriptFieldDataType::NINT: {
                                writer << YAML::Value << fieldValue.GetValue<uint32_t>();
                                break;
                            }

                            case ScriptFieldDataType::LONG: {
                                writer << YAML::Value << fieldValue.GetValue<long>();
                                break;
                            }

                            case ScriptFieldDataType::ULONG: {
                                writer << YAML::Value << fieldValue.GetValue<uint64_t>();
                                break;
                            }
                            
                            case ScriptFieldDataType::FLOAT: {
                                writer << YAML::Value << fieldValue.GetValue<float>();
                                break;
                            }

                            case ScriptFieldDataType::DOUBLE: {
                                writer << YAML::Value << fieldValue.GetValue<double>();
                                break;
                            }
                            
                            // TODO: Change to float128_t
                            case ScriptFieldDataType::DECIMAL: {
                                writer << YAML::Value << fieldValue.GetValue<double>();
                                break;
                            }

                            // TODO: Change to native string
                            case ScriptFieldDataType::STRING: {
                                writer << YAML::Value << fieldValue.GetValue<char>();
                                break;
                            }
                            
                            // Store UUID to another entity
                            case ScriptFieldDataType::ENTITY: {
                                writer << YAML::Value << fieldValue.GetValue<uint64_t>();
                                break;
                            }

                            case ScriptFieldDataType::VECTOR2: {
                                writer << YAML::Value << fieldValue.GetValue<glm::vec2>();
                                break;
                            }

                            case ScriptFieldDataType::VECTOR3: {
                                writer << YAML::Value << fieldValue.GetValue<glm::vec3>();
                                break;
                            }

                            case ScriptFieldDataType::VECTOR4: {
                                writer << YAML::Value << fieldValue.GetValue<glm::vec4>();
                                break;
                            }

                            default:{
                                writer << YAML::Value << "Invalid";
                            }
                        }
                        

                        writer << YAML::EndMap;
                    }

                    writer << YAML::EndSeq;
                }
                
            }

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

        if(entity.HasComponent<CircleSpriteComponent>()){
            writer << YAML::Key << "CircleSpriteComponent";
            writer << YAML::BeginMap;

            auto& circleComponent = entity.GetComponent<CircleSpriteComponent>();

            writer << YAML::Key << "Color" << YAML::Value << circleComponent.Color;
            writer << YAML::Key << "Thickness" << YAML::Value << circleComponent.Thickness;
            writer << YAML::Key << "Fade" << YAML::Value << circleComponent.Fade;

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

        if(entity.HasComponent<CircleCollider2DComponent>()){
            writer << YAML::Key << "CircleCollider2DComponent";

            auto& circleCollider2D = entity.GetComponent<CircleCollider2DComponent>();

            {
                writer << YAML::BeginMap;

                writer << YAML::Key << "Offset" << YAML::Value << circleCollider2D.Offset;
                writer << YAML::Key << "Radius" << YAML::Value << circleCollider2D.Radius;
                writer << YAML::Key << "Density" << YAML::Value << circleCollider2D.Density;
                writer << YAML::Key << "Friction" << YAML::Value << circleCollider2D.Friction;
                writer << YAML::Key << "Restitution" << YAML::Value << circleCollider2D.Restitution;
                writer << YAML::Key << "RestitutionThreshold" << YAML::Value << circleCollider2D.RestitutionThreshold;
                
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

                auto&& scriptNode = entity["ScriptComponent"];
                if(scriptNode){
                    auto& script = deserializeEntity.AddComponent<ScriptComponent>();
                    script.ScriptName = scriptNode["ScriptName"].as<std::string>();

                    auto&& fields = scriptNode["ScriptFields"];

                    if(fields){

                        auto& fieldMap = ScriptEngine::GetScriptFieldData();
                        auto& uuid = deserializeEntity.GetUUID();

                        for(auto&& field : fields){
                            auto&& fieldName = field["Name"].as<std::string>();
                            auto&& fieldTypeName = field["Type"].as<std::string>();
                            
                            auto&& fieldType = Utils::ConvertStringToFieldType(fieldTypeName);

                            fieldMap[uuid][fieldName].DataType = fieldType;

                            auto&& fieldAt = fieldMap.at(uuid).at(fieldName);
                            switch(fieldType){
                                case ScriptFieldDataType::BOOL:{
                                    fieldAt.SetValue<bool>(field["Data"].as<bool>());
                                    break;
                                }
                                case ScriptFieldDataType::BYTE:{
                                    fieldAt.SetValue<uint8_t>(field["Data"].as<uint8_t>());
                                    break;
                                }
                                // case ScriptFieldDataType::SBYTE:{
                                //    break; 
                                // }
                                case ScriptFieldDataType::CHAR:{
                                    fieldAt.SetValue<char>(field["Data"].as<char>());
                                    break; 
                                }
                                case ScriptFieldDataType::SHORT:{
                                    fieldAt.SetValue<short>(field["Data"].as<short>());
                                    break;
                                }
                                case ScriptFieldDataType::USHORT:{
                                    fieldAt.SetValue<uint16_t>(field["Data"].as<uint16_t>());
                                    break;
                                }
                                case ScriptFieldDataType::INT:{
                                    fieldAt.SetValue<int>(field["Data"].as<int>());
                                    break;
                                }
                                case ScriptFieldDataType::UINT:{
                                    fieldAt.SetValue<uint32_t>(field["Data"].as<uint32_t>());
                                    break;
                                }
                                // case ScriptFieldDataType::NINT:{
                                //    break; 
                                // }
                                case ScriptFieldDataType::LONG:{
                                    fieldAt.SetValue<long>(field["Data"].as<long>());
                                    break;
                                }
                                case ScriptFieldDataType::ULONG:{
                                    fieldAt.SetValue<uint64_t>(field["Data"].as<uint64_t>());
                                    break;
                                }
                                case ScriptFieldDataType::FLOAT:{
                                    fieldAt.SetValue<float>(field["Data"].as<float>());
                                    break;
                                }
                                case ScriptFieldDataType::DOUBLE:{
                                    fieldAt.SetValue<double>(field["Data"].as<double>());
                                    break;
                                }
                                // TODO: float128_t
                                case ScriptFieldDataType::DECIMAL:{
                                    fieldAt.SetValue<double>(field["Data"].as<double>());
                                    break;
                                }
                                // case ScriptFieldDataType::STRING:{
                                //    break; 
                                // }
                                case ScriptFieldDataType::ENTITY:{
                                    fieldAt.SetValue<uint64_t>(field["Data"].as<uint64_t>());
                                    break;
                                }
                                case ScriptFieldDataType::VECTOR2:{
                                    fieldAt.SetValue<glm::vec2>(field["Data"].as<glm::vec2>());
                                    break;
                                }
                                case ScriptFieldDataType::VECTOR3:{
                                    fieldAt.SetValue<glm::vec3>(field["Data"].as<glm::vec3>());
                                    break;
                                }
                                case ScriptFieldDataType::VECTOR4:{
                                    fieldAt.SetValue<glm::vec4>(field["Data"].as<glm::vec4>());
                                    break;
                                }
                            }
                        }
                    }
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

                auto&& circleSpriteNode = entity["CircleSpriteComponent"];
                if(circleSpriteNode){
                    auto& circleSpiteComponent = deserializeEntity.AddComponent<CircleSpriteComponent>();
                    circleSpiteComponent.Color = circleSpriteNode["Color"].as<glm::vec4>();
                    circleSpiteComponent.Thickness = circleSpriteNode["Thickness"].as<float>();
                    circleSpiteComponent.Fade = circleSpriteNode["Fade"].as<float>();
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

                auto&& circleCollider2DNode = entity["CircleCollider2DComponent"];
                if(circleCollider2DNode){
                    auto& circleCollider2D = deserializeEntity.AddComponent<CircleCollider2DComponent>();
                    circleCollider2D.Offset = circleCollider2DNode["Offset"].as<glm::vec2>();
                    circleCollider2D.Radius = circleCollider2DNode["Radius"].as<float>();
                    circleCollider2D.Density = circleCollider2DNode["Density"].as<float>();
                    circleCollider2D.Friction = circleCollider2DNode["Friction"].as<float>();
                    circleCollider2D.Restitution = circleCollider2DNode["Restitution"].as<float>();
                    circleCollider2D.RestitutionThreshold = circleCollider2DNode["RestitutionThreshold"].as<float>();
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