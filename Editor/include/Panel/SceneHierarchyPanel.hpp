#pragma once

#include <CreepyEngine.hpp>

namespace Creepy {

    class SceneHierarchyPanel
    {
        public:
            SceneHierarchyPanel() noexcept = default;
            SceneHierarchyPanel(const Ref<Scene>& scene) noexcept;

            void SetScene(const Ref<Scene>& scene) noexcept;

            void OnImGuiRender() noexcept;

        private:

            void drawEntityNode(Entity& entity) noexcept;

            void drawEntityProperty(Entity& entity) noexcept;
        private:
            Ref<Scene> m_scene{nullptr};
            Entity m_selectedEntity;
    };
}