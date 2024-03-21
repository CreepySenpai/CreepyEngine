#pragma once

#include <CreepyEngine.hpp>
#include <Panel/SceneHierarchyPanel.hpp>
#include <Panel/ContentBrowserPanel.hpp>

namespace Creepy {


    class EditorLayer : public Layer
    {
        public:
            EditorLayer() noexcept;

            virtual ~EditorLayer() noexcept;

            void OnAttach() noexcept override;

            void OnDetach() noexcept override;

            void OnUpdate(TimeStep timeStep) noexcept override;

            void OnImGuiRender() noexcept override;

            void OnEvent(Event &event) noexcept override;

        private:
            bool OnKeyPressed(KeyPressedEvent& event) noexcept;
            bool OnMouseButtonPressed(MouseButtonPressedEvent& event) noexcept;

            void newScene() noexcept;
            void openScene() noexcept;
            void openScene(const std::filesystem::path& filePath) noexcept;
            void saveSceneAs() noexcept;

            void drawGizmos() noexcept;
            void drawThemePanel() noexcept;

            bool canMousePicking() noexcept;
        private:
            Ref<FrameBuffer> m_frameBuffer;
            Ref<Texture2D> m_texture;
            Ref<Scene> m_scene;
            EditorCamera m_editorCamera;
            SceneHierarchyPanel m_hierarchyPanel;
            ContentBrowserPanel m_contentBrowserPanel;

            Entity m_selectedEntity;
            glm::vec2 m_viewPortSize;
            glm::vec2 m_viewPortBounds[2];

            int m_gizmosType{-1};
            bool m_viewPortFocused;
            bool m_viewPortHovered;

    };
    
    
}