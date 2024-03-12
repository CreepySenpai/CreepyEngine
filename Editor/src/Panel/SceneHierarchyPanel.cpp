#include <Panel/SceneHierarchyPanel.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Creepy {

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) noexcept : m_scene{scene} {

    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene) noexcept {
        m_scene = scene;
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

        ImGui::End();

        ImGui::Begin("Properties");

        if(m_selectedEntity.m_entityHandle != entt::null && m_selectedEntity.m_scene != nullptr){
            drawEntityProperty(m_selectedEntity);
        }

        ImGui::End();
    }


    void SceneHierarchyPanel::drawEntityNode(Entity& entity) noexcept {

        auto& tag = entity.GetComponent<TagComponent>();

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;


        if(m_selectedEntity == entity){
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        
        if(ImGui::TreeNodeEx(reinterpret_cast<void*>( static_cast<uint32_t>(entity)), flags, tag.Tag.c_str())) {
            ImGui::TreePop();
        }

        if(ImGui::IsItemClicked()){
            m_selectedEntity = entity;
        }

    }

    static void drawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) noexcept {
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

        if(ImGui::Button("X", buttonSize)){
            values.x = resetValue;
        }

        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        ImGui::DragFloat("##X", &values.x, 0.1f);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        /*================================================*/
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.4f, 0.0f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.0f, 0.7f, 0.0f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.0f, 1.0f, 0.0f, 1.0f});

        if(ImGui::Button("Y", buttonSize)){
            values.y = resetValue;
        }

        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        ImGui::DragFloat("##Y", &values.y, 0.1f);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        /*================================================*/

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.0f, 0.4f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.0f, 0.0f, 0.7f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.0f, 0.0f, 1.0f, 1.0f});

        if(ImGui::Button("Z", buttonSize)){
            values.z = resetValue;
        }

        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        ImGui::DragFloat("##Z", &values.z, 0.1f);

        ImGui::PopItemWidth();

        ImGui::PopStyleVar();


        ImGui::Columns(1);  // reset

        ImGui::PopID();
    }

    void SceneHierarchyPanel::drawEntityProperty(Entity& entity) noexcept {

        if(entity.HasComponent<TagComponent>()) {
            auto& tag = entity.GetComponent<TagComponent>();

            char buffer[256];

            std::ranges::fill(buffer, 0);
            std::ranges::copy(tag.Tag, buffer);

            if(ImGui::InputText("Tag", buffer, std::size(buffer))){
                tag.Tag.assign(buffer);
            }
        }

        if(entity.HasComponent<TransformComponent>()) {
            
            if(ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(TransformComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Transform")){

                drawVec3Control("Position", entity.GetComponent<TransformComponent>().Position);
                drawVec3Control("Rotation", entity.GetComponent<TransformComponent>().Rotation);
                drawVec3Control("Scale", entity.GetComponent<TransformComponent>().Scale);

                ImGui::TreePop();
            }

            
        }

        if(entity.HasComponent<CameraComponent>()) {

            if(ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(CameraComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Camera")){

                auto& cameraComponent = entity.GetComponent<CameraComponent>();
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

                ImGui::TreePop();
            }

        }


        if(entity.HasComponent<SpriteComponent>()) {
            
            if(ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(SpriteComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Render")) {
                auto& color = entity.GetComponent<SpriteComponent>().Color;

                ImGui::ColorEdit4("", glm::value_ptr(color));

                ImGui::TreePop();
            }

        }
    }
}