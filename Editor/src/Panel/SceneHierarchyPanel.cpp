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

        ImGui::End();
    }


    void SceneHierarchyPanel::drawEntityNode(Entity& entity) noexcept {

        auto& tag = entity.GetComponent<TagComponent>();

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;


        if(m_selectedEntity == entity){
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        
        bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>( static_cast<uint32_t>(entity)), flags, tag.Tag.c_str());

        if(ImGui::IsItemClicked()){
            m_selectedEntity = entity;
        }

        if(opened){
            ImGui::TreePop();
        }

    }
}