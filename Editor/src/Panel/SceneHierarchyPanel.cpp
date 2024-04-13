#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <CreepyEngine/Utils/ScriptEngineUtils.hpp>
#include <Coral/ManagedObject.hpp>
#include <Coral/Type.hpp>


#include <Panel/SceneHierarchyPanel.hpp>
#include <Editor/EditorLayer.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Creepy {

    extern const std::filesystem::path AssetDirectory;

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) noexcept : m_scene{scene} {

    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene) noexcept {
        m_scene.reset();
        m_scene = scene;
        
        // TODO: If we have multi scene and we want keep it last selected entity we need to change it
        m_selectedEntity = {};
    }
    
    void SceneHierarchyPanel::OnImGuiRender() noexcept {
        ImGui::Begin("Scene Hierarchy");

        m_scene->m_registry.view<entt::entity>().each([this](auto entityID){

            Entity entity{entityID, m_scene.get()};

            drawEntityNode(entity);

        });

        // Deselect entity
        if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()){
            m_selectedEntity = {};
        }
        

        // Active when click on blank space
        if(ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)){
            
            if(ImGui::MenuItem("Create Entity")){
                m_scene->CreateEntity("Empty Entity");
            }

            ImGui::EndPopup();

        }

        ImGui::End();

        ImGui::Begin("Properties");

        if(m_selectedEntity.IsExits()){
            drawEntityProperty(m_selectedEntity);
        }

        ImGui::End();
    }

    static void drawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) noexcept {

        auto& io = ImGui::GetIO();

        auto&& boldFont = io.Fonts->Fonts[0]; // We have load bold font on index 0

        // Unique ID for every obj
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);

        ImGui::SetColumnWidth(0, columnWidth);

        ImGui::Text(label.c_str());

        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

        float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        ImVec2 buttonSize{lineHeight + 3.0f, lineHeight};

        /*================================================*/

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.4f, 0.0f, 0.0f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.7f, 0.0f, 0.0f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{1.0f, 0.0f, 0.0f, 1.0f});
        ImGui::PushFont(boldFont);

        if(ImGui::Button("X", buttonSize)){
            values.x = resetValue;
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        ImGui::DragFloat("##X", &values.x, 0.1f);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        /*================================================*/
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.4f, 0.0f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.0f, 0.7f, 0.0f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.0f, 1.0f, 0.0f, 1.0f});
        ImGui::PushFont(boldFont);

        if(ImGui::Button("Y", buttonSize)){
            values.y = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        ImGui::DragFloat("##Y", &values.y, 0.1f);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        /*================================================*/

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.0f, 0.4f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.0f, 0.0f, 0.7f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.0f, 0.0f, 1.0f, 1.0f});
        ImGui::PushFont(boldFont);

        if(ImGui::Button("Z", buttonSize)){
            values.z = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        ImGui::DragFloat("##Z", &values.z, 0.1f);

        ImGui::PopItemWidth();

        ImGui::PopStyleVar();


        ImGui::Columns(1);  // reset

        ImGui::PopID();
    }

    template <typename T>
    struct MyDrawComponent {
        template<typename UIFunction>
        static void DrawComponent(const std::string& label, Entity& entity, UIFunction&& func) noexcept
        {
            if (entity.HasComponent<T>())
            {
                constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap
                    | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

                // Get content available before set padding
                auto&& contentAvailable = ImGui::GetContentRegionAvail();

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4.0f, 4.0f});

                // Calculate after setup frame padding
                const float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                
                ImGui::Separator();

                bool isOpened = ImGui::TreeNodeEx(reinterpret_cast<void *>(typeid(T).hash_code()), treeNodeFlags, label.c_str());

                ImGui::PopStyleVar();

                ImGui::SameLine(contentAvailable.x - lineHeight * 0.5f);

                if (ImGui::Button("+", ImVec2{lineHeight, lineHeight}))
                {
                    ImGui::OpenPopup("ComponentSettings");
                }

                bool isRemovedComponent{false};

                if (ImGui::BeginPopup("ComponentSettings"))
                {
                    if (ImGui::MenuItem("Remove Component"))
                    {
                        isRemovedComponent = true;
                    }

                    ImGui::EndPopup();
                }

                if (isOpened)
                {
                    func(entity.GetComponent<T>());

                    ImGui::TreePop();
                }

                if (isRemovedComponent)
                {
                    entity.RemoveComponent<T>();
                }
            }
        }
    };


    // Specific for transform component
    template <>
    struct MyDrawComponent<TransformComponent>{
        template <typename UIFunction>
        static void DrawComponent(const std::string& label, Entity& entity, UIFunction&& func) noexcept {
            if (entity.HasComponent<TransformComponent>())
            {
                constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap
                    | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

                // Get content available before set padding
                auto&& contentAvailable = ImGui::GetContentRegionAvail();

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4.0f, 4.0f});

                // Calculate after setup frame padding
                const float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                
                ImGui::Separator();

                bool isOpened = ImGui::TreeNodeEx(reinterpret_cast<void *>(typeid(TransformComponent).hash_code()), treeNodeFlags, label.c_str());

                ImGui::PopStyleVar();

                if (isOpened)
                {
                    func(entity.GetComponent<TransformComponent>());

                    ImGui::TreePop();
                }

            }
        }
    };


    void SceneHierarchyPanel::drawEntityNode(Entity& entity) noexcept {

        auto& tag = entity.GetComponent<TagComponent>();

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;


        if(m_selectedEntity == entity){
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        
        if(ImGui::TreeNodeEx(reinterpret_cast<void*>( static_cast<uint32_t>(entity)), flags, tag.Tag.c_str())) {
            ImGui::TreePop();
        }

        if(ImGui::IsItemClicked()){
            m_selectedEntity = entity;
        }


        bool isDeletedEntity{false};
        // Make Delay Effect When Delete Entity, End of Frame We Delete It
        if(ImGui::BeginPopupContextItem()){
            
            if(ImGui::MenuItem("Delete Entity")){
                isDeletedEntity = true;
            }

            ImGui::EndPopup();

        }

        // Some Code Use Entity In Future


        // Real Delete
        if(isDeletedEntity){
            // Check if current select entity is delete we need to change it
            if(m_selectedEntity == entity){
                m_selectedEntity = {};
            }

            m_scene->DestroyEntity(entity);
        }

    }

    template <typename Comp>
    static void DisplayAddComponent(Entity& entity, const char* label){
        if(!entity.HasComponent<Comp>()){
            if (ImGui::MenuItem(label))
            {
                entity.AddComponent<Comp>();
                ImGui::CloseCurrentPopup();
            }
        }
    }

    void SceneHierarchyPanel::drawEntityProperty(Entity& entity) noexcept {

        if(entity.HasComponent<TagComponent>()) {
            auto& tag = entity.GetComponent<TagComponent>();

            char buffer[256];

            std::ranges::fill(buffer, 0);
            std::ranges::copy(tag.Tag, buffer);

            ImGui::PushID(tag.Tag.c_str());

            ImGui::Columns(2);

            ImGui::SetColumnWidth(0, 100.0f);

            ImGui::Text("Entity Tag");

            ImGui::NextColumn();

            float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;

            ImVec2 buttonSize{lineHeight + 3.0f, lineHeight};

            if(ImGui::InputText("", buffer, std::size(buffer), ImGuiInputTextFlags_EnterReturnsTrue)){
                tag.Tag.assign(buffer);
            }

            ImGui::Columns(1);

            ImGui::PopID();
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1.0f);

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponents"); // open by id
        }

        if (ImGui::BeginPopup("AddComponents"))
        { // sign id

            if(!entity.HasComponent<CameraComponent>()){
                if (ImGui::MenuItem("Camera"))
                {
                    entity.AddComponent<CameraComponent>().IsPrimary = false;
                    ImGui::CloseCurrentPopup();
                }
            }

            // DisplayAddComponent<NativeScriptComponent>(entity, "NativeScript");

            DisplayAddComponent<ScriptComponent>(entity, "Script");
            DisplayAddComponent<SpriteComponent>(entity, "Sprite");
            DisplayAddComponent<CircleSpriteComponent>(entity, "CircleSprite");
            DisplayAddComponent<RigidBody2DComponent>(entity, "RigidBody2D");
            DisplayAddComponent<BoxCollider2DComponent>(entity, "BoxCollider2D");
            DisplayAddComponent<CircleCollider2DComponent>(entity, "CircleCollider2DComponent");
            DisplayAddComponent<MeshComponent>(entity, "MeshComponent");
            

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        MyDrawComponent<TransformComponent>::DrawComponent("Transform", entity, [](TransformComponent& transformComponent){
            drawVec3Control("Position", transformComponent.Position);
            drawVec3Control("Rotation", transformComponent.Rotation);
            drawVec3Control("Scale", transformComponent.Scale);
        });

        MyDrawComponent<CameraComponent>::DrawComponent("Camera", entity, [](CameraComponent& cameraComponent){
            auto& camera = cameraComponent.Camera;
            const char* projectionType[] = {"Orthographic", "Perspective"};
            const char* currentProjection = projectionType[std::to_underlying(camera.GetProjectionType())];
            ImGui::Checkbox("Primary Camera", &cameraComponent.IsPrimary);

            if(ImGui::BeginCombo("Projection Type", currentProjection)) {
                
                for(uint32_t i{0}; auto& projection : projectionType){

                    bool isSelected = currentProjection == projectionType[i];   // Pointer Compare

                    if(ImGui::Selectable(projectionType[i], isSelected)){
                        currentProjection = projectionType[i];
                        camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
                    }

                    if(isSelected){
                        ImGui::SetItemDefaultFocus();
                    }

                    ++i;
                }

                ImGui::EndCombo();
            }

            if(camera.GetProjectionType() == SceneCamera::ProjectionType::ORTHOGRAPHIC){

                float orthoSize = camera.GetOrthographicSize();
                if(ImGui::DragFloat("Size", &orthoSize)){
                    camera.SetOrthographicSize(orthoSize);
                }

                float orthoNear = camera.GetOrthographicNearClip();

                if(ImGui::DragFloat("NearClip", &orthoNear)) {
                    camera.SetOrthographicNearClip(orthoNear);
                }

                float orthoFar = camera.GetOrthographicFarClip();
                if(ImGui::DragFloat("FarClip", &orthoFar)) {
                    camera.SetOrthographicFarClip(orthoFar);
                }
                        
                // Fixed Aspect Ratio Only For Orthographic
                ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
            }
            else if(camera.GetProjectionType() == SceneCamera::ProjectionType::PERSPECTIVE){

                float perspectiveFOV = camera.GetPerspectiveVerticalFOV();
                if(ImGui::DragFloat("VerticalFOV", &perspectiveFOV)){
                    camera.SetPerspectiveVerticalFOV(perspectiveFOV);
                }

                float perspectiveNear = camera.GetPerspectiveNearClip();
                if(ImGui::DragFloat("NearClip", &perspectiveNear)) {
                    camera.SetPerspectiveNearClip(perspectiveNear);
                }

                float perspectiveFar = camera.GetPerspectiveFarClip();
                if(ImGui::DragFloat("FarClip", &perspectiveFar)) {
                    camera.SetPerspectiveFarClip(perspectiveFar);
                }

            }
        });

        MyDrawComponent<ScriptComponent>::DrawComponent("Script", entity, [&entity, this](ScriptComponent& scriptComponent){
            
            bool isScriptClassExits = ScriptEngine::IsClassExits(scriptComponent.ScriptName);

            static char buffer[128];
            std::ranges::fill(buffer, 0);
            std::ranges::copy(scriptComponent.ScriptName, buffer);

            if(!isScriptClassExits){
                ImGui::PushStyleColor(ImGuiCol_Text, {0.8f, 0.2f, 0.3f, 1.0f});
            }

            if(ImGui::InputText("Class", buffer, sizeof(buffer))) {
                scriptComponent.ScriptName = buffer;
            }

            // WARING: If wrong name maybe something happen
            auto&& classType = ScriptEngine::GetEntityClass(scriptComponent.ScriptName);

            if(classType){
                
                const bool isPlay = m_scene->IsScenePlay();

                auto& scriptFieldData = ScriptEngine::GetScriptFieldData();

                auto&& uuid = entity.GetUUID();

                if(!scriptFieldData.contains(uuid)){

                    ScriptEngine::CreateEntityFastInstanceToCopyData(uuid, classType);
                    
                }

                // For Sure
                if(scriptFieldData.contains(uuid)){

                    for(auto&& field : classType->GetFields()){

                        auto&& fieldName = static_cast<std::string>(field.GetName());
                        if(fieldName == "UUID"){
                            continue;
                        }

                        if(scriptFieldData.at(uuid).contains(fieldName)){

                            auto&& fieldMap = scriptFieldData.at(uuid).at(fieldName);

                            auto&& type = fieldMap.DataType;

                            // TODO: Use Switch case to Set More Data Type
                            if(type == ScriptFieldDataType::FLOAT){

                                if(isPlay){

                                    auto&& entityInstance = ScriptEngine::GetEntityInstance(uuid);
                                    float value = entityInstance.GetFieldValue<float>(fieldName);

                                    if(ImGui::DragFloat(fieldName.c_str(), &value)){
                                        entityInstance.SetFieldValue(fieldName, std::move(value));
                                    }

                                }

                                else {

                                    float value = fieldMap.GetValue<float>();

                                    if (ImGui::DragFloat(fieldName.c_str(), &value))
                                    {
                                        fieldMap.SetValue<float>(value);
                                    }

                                }

                            }

                        } 

                        // Solution For Save And Load Script Field Not Save In File But Have Change
                        else {
                            ScriptEngine::CreateEntityFastInstanceToCopyData(uuid, classType);
                        }

                    }
                }

            }

            if(!isScriptClassExits){
                ImGui::PopStyleColor();
            }

        });

        MyDrawComponent<SpriteComponent>::DrawComponent("Sprite Render", entity, [](SpriteComponent& spriteComponent){
            ImGui::ColorEdit4("", glm::value_ptr(spriteComponent.Color));

            ImGui::Button("Texture", {100.0f, 0.0f});
            
            if(ImGui::BeginDragDropTarget()){
            
                // Payload maybe null
                if(auto payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")){
                    
                    auto path = reinterpret_cast<const char*>(payload->Data);

                    std::filesystem::path texturePath{AssetDirectory / path};

                    if(std::filesystem::exists(texturePath) && texturePath.extension().string() == ".png"){
                        
                        spriteComponent.Texture = Texture2D::Create(texturePath.string());

                    }
                    

                }

                ImGui::EndDragDropTarget();
            }

            ImGui::DragFloat("Tiling Factor", &spriteComponent.TilingFactor, 0.1f, 0.0f, 10.0f);

        });


        MyDrawComponent<CircleSpriteComponent>::DrawComponent("Circle Sprite", entity, [](CircleSpriteComponent& circleSprite){
            ImGui::ColorEdit4("Color", glm::value_ptr(circleSprite.Color));
            ImGui::DragFloat("Thickness", &circleSprite.Thickness, 0.025f, 0.0f, 1.0f);
            ImGui::DragFloat("Fade", &circleSprite.Fade, 0.025f, 0.0f, 1.0f);
        });

        MyDrawComponent<RigidBody2DComponent>::DrawComponent("RigidBody 2D", entity, [](RigidBody2DComponent& rigid2DComponent){

            const char* bodyType[] = {"Static", "Dynamic", "Kinematic"};

            const char* currentBodyType = bodyType[std::to_underlying(rigid2DComponent.Type)];

            ImGui::Checkbox("Is Fixed Body", &rigid2DComponent.FixedRotation);

            if(ImGui::BeginCombo("Body Type", currentBodyType)) {
                
                for(uint32_t i{0}; auto& projection : bodyType){

                    bool isSelected = currentBodyType == bodyType[i];   // Pointer Compare

                    if(ImGui::Selectable(bodyType[i], isSelected)){
                        currentBodyType = bodyType[i];
                        rigid2DComponent.Type = static_cast<RigidBody2DComponent::BodyType>(i);
                    }

                    if(isSelected){
                        ImGui::SetItemDefaultFocus();
                    }

                    ++i;
                }

                ImGui::EndCombo();
            }
        });

        MyDrawComponent<BoxCollider2DComponent>::DrawComponent("BoxCollider 2D", entity, [](BoxCollider2DComponent& boxCollider2DComponent){
            ImGui::DragFloat2("Offset", glm::value_ptr(boxCollider2DComponent.Offset), 0.1f);
            ImGui::DragFloat2("Size", glm::value_ptr(boxCollider2DComponent.Size), 0.1f);
            ImGui::DragFloat("Density", &boxCollider2DComponent.Density, 0.1f, 0.0f);
            ImGui::DragFloat("Friction", &boxCollider2DComponent.Friction, 0.1f, 0.0f);
            ImGui::DragFloat("Restitution", &boxCollider2DComponent.Restitution, 0.1f, 0.0f);
            ImGui::DragFloat("RestitutionThreshold", &boxCollider2DComponent.RestitutionThreshold, 0.1f, 0.0f);

        });

        MyDrawComponent<CircleCollider2DComponent>::DrawComponent("CircleCollider 2D", entity, [](CircleCollider2DComponent& circleCollider2DComponent){
            ImGui::DragFloat2("Offset", glm::value_ptr(circleCollider2DComponent.Offset));
            ImGui::DragFloat("Radius", &circleCollider2DComponent.Radius, 1.0f, 0.0f);
            ImGui::DragFloat("Density", &circleCollider2DComponent.Density, 0.1f, 0.0f);
            ImGui::DragFloat("Friction", &circleCollider2DComponent.Friction, 0.1f, 0.0f);
            ImGui::DragFloat("Restitution", &circleCollider2DComponent.Restitution, 0.1f, 0.0f);
            ImGui::DragFloat("RestitutionThreshold", &circleCollider2DComponent.RestitutionThreshold, 0.1f, 0.0f);

        });

        MyDrawComponent<MeshComponent>::DrawComponent("Mesh", entity, [](MeshComponent& meshComponent){
            
            const char* meshType[] = {"None", "Cube", "Sphere", "Capsule", "Cylinder", "Plane", "Cone", "Torus"};
            const char* currentMesh = meshType[std::to_underlying(meshComponent.Type)];

            if(ImGui::BeginCombo("Mesh Type", currentMesh)){
                for(uint32_t i{0}; auto& mesh : meshType){

                    bool isSelected = currentMesh == meshType[i];   // Pointer Compare

                    if(ImGui::Selectable(meshType[i], isSelected)){
                        currentMesh = meshType[i];
                        meshComponent.Type = static_cast<MeshComponent::MeshType>(i);
                    }

                    if(isSelected){
                        ImGui::SetItemDefaultFocus();
                    }

                    ++i;
                }

                ImGui::EndCombo();
            }
        });

    }

    void SceneHierarchyPanel::SetSelectedEntity(Entity& entity) noexcept {
        m_selectedEntity = entity;
    }
}