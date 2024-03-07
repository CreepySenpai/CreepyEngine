#pragma once

#include <CreepyEngine.hpp>

namespace Creepy {


    class EditorLayer : public Layer
    {
        public:
            EditorLayer() noexcept;

            virtual ~EditorLayer() noexcept;

            void OnAttach() noexcept override;

            void OnDetach() noexcept override;

            void OnUpdate(const TimeStep& timeStep) noexcept override;

            void OnImGuiRender() noexcept override;

            void OnEvent(Event &event) noexcept override;
        private:
            OrthographicCameraController m_cameraController;
            Ref<FrameBuffer> m_frameBuffer;
            Ref<Texture2D> m_texture;
            glm::vec2 m_viewPortSize;
    };
    
    
}