#include <Panel/SceneHierarchyPanel.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Creepy {

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) noexcept : m_scene{scene} {

    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene) noexcept {
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

    template <typename T, typename UIFunction>
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
            ImGui::OpenPopup("AddComponets"); // open by id
        }

        if (ImGui::BeginPopup("AddComponets"))
        { // sign id

            if (ImGui::MenuItem("Camera"))
            {
                m_selectedEntity.AddComponent<CameraComponent>().IsPrimary = false;
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Sprite"))
            {
                m_selectedEntity.AddComponent<SpriteComponent>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& transformComponent){
            drawVec3Control("Position", transformComponent.Position);
            drawVec3Control("Rotation", transformComponent.Rotation);
            drawVec3Control("Scale", transformComponent.Scale);
        });

        DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& cameraComponent){
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

        DrawComponent<SpriteComponent>("Sprite Render", entity, [](SpriteComponent& spriteComponent){
            ImGui::ColorEdit4("", glm::value_ptr(spriteComponent.Color));
        });

    }
}