#pragma once

#include <CreepyEngine.hpp>
#include <Panel/SceneHierarchyPanel.hpp>
#include <Panel/ContentBrowserPanel.hpp>

namespace Creepy {

    enum class SceneState : uint8_t {
        EDIT = 0, PLAY, STOP, SIMULATION
    };
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

            SceneState GetEditorState() const noexcept;

            static EditorLayer& GetInstance() noexcept;

        private:
            bool onKeyPressed(KeyPressedEvent& event) noexcept;
            bool onMouseButtonPressed(MouseButtonPressedEvent& event) noexcept;
            void onOverlayRender() noexcept;
            void onScenePlay() noexcept;
            void onSceneStop() noexcept;
            void onSimulationPlay() noexcept;

            void onDuplicateEntity() noexcept;

            void newScene() noexcept;
            void openScene() noexcept;
            void openScene(const std::filesystem::path& filePath) noexcept;
            void saveScene() noexcept;
            void saveSceneAs() noexcept;

            void drawGizmos() noexcept;
            void drawThemePanel() noexcept;

            // UI Stuff
            void uiDrawToolBar() noexcept;


            // Editor Function Stuff

            bool canMousePicking() noexcept;

            void duplicateEntity(Entity& entity) noexcept;
            
        private:
            Ref<FrameBuffer> m_frameBuffer;
            Ref<Texture2D> m_playIcon;
            Ref<Texture2D> m_stopIcon;
            Ref<Texture2D> m_simulationIcon;
            Ref<Scene> m_activeScene;
            Ref<Scene> m_editorScene;
            EditorCamera m_editorCamera;
            SceneHierarchyPanel m_hierarchyPanel;
            ContentBrowserPanel m_contentBrowserPanel;
            
            Entity m_hoverEntity;
            glm::vec2 m_viewPortSize;
            glm::vec2 m_viewPortBounds[2];

            int m_gizmosType{-1};
            bool m_viewPortFocused;
            bool m_viewPortHovered;
            bool m_showPhysicCollider{true};

            SceneState m_sceneState{SceneState::EDIT};
            
            std::filesystem::path m_editorScenePath;

            static EditorLayer* s_instance;
    };
    
    
}