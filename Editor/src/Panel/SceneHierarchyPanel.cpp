#include <Panel/SceneHierarchyPanel.hpp>
#include <imgui/imgui.h>

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

    void SceneHierarchyPanel::drawEntityProperty(Entity& entity) noexcept {

        if(entity.HasComponent<TagComponent>()) {
            auto& tag = entity.GetComponent<TagComponent>();

            char buffer[256];

            std::fill(std::begin(buffer), std::end(buffer), 0);
            
            for(int i{}; auto& c : tag.Tag){
                buffer[i] = c;
                ++i;
            }

            if(ImGui::InputText("Tag", buffer, std::size(buffer))){
                tag.Tag.assign(buffer);
            }
        }

        if(entity.HasComponent<TransformComponent>()) {
            
            if(ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(TransformComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Transform")){

                auto& transform = entity.GetComponent<TransformComponent>().Transform;

                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.5f);

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

    }
}