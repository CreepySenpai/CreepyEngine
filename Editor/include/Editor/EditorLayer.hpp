#pragma once

#include <CreepyEngine.hpp>
#include <Panel/SceneHierarchyPanel.hpp>

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

            void NewScene() noexcept;
            void OpenScene() noexcept;
            void SaveSceneAs() noexcept;
        private:
            OrthographicCameraController m_cameraController;
            Ref<FrameBuffer> m_frameBuffer;
            Ref<Texture2D> m_texture;
            Ref<Scene> m_scene;
            Entity m_camera;
            SceneHierarchyPanel m_hierarchyPanel;
            glm::vec2 m_viewPortSize;
            bool m_viewPortFocused;
            bool m_viewPortHovered;

    };
    
    
}