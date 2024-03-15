#pragma once

#include <CreepyEngine/Core/Layer.hpp>
#include <CreepyEngine/Core/KeyCode.hpp>
#include <CreepyEngine/Events/MouseEvent.hpp>
#include <CreepyEngine/Events/ApplicationEvent.hpp>
#include <CreepyEngine/Events/KeyEvent.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <memory>

namespace Creepy
{
    class ImGuiLayer : public Layer
    {
        public:
            ImGuiLayer() noexcept;
            virtual ~ImGuiLayer() = default;

            void OnAttach() noexcept override;
            void OnDetach() noexcept override;
            void OnUpdate(TimeStep timeStep) noexcept override;
            void OnImGuiRender() noexcept override;
            void OnEvent(Event& event) noexcept override;

            void Begin() noexcept;
            void End() noexcept;

            inline void BlockEvents(bool block) noexcept{
                m_blockEvents = block;
            }

            void SetTheme() noexcept;

        private:
            float m_time{0.0f};
            bool m_blockEvents{true};

            struct LayerConfig{
                glm::vec4 WindowBg{0.1f, 0.105f, 0.11f, 1.0f};

                glm::vec4 Header{0.2f, 0.205f, 0.21f, 1.0f};
                glm::vec4 HeaderHovered{0.3f, 0.305f, 0.31f, 1.0f};
                glm::vec4 HeaderActive{0.15f, 0.1505f, 0.151f, 1.0f};

                glm::vec4 Button{0.2f, 0.205f, 0.21f, 1.0f};
                glm::vec4 ButtonHovered{ 0.3f, 0.305f, 0.31f, 1.0f};
                glm::vec4 ButtonActive{0.15f, 0.1505f, 0.151f, 1.0f};

                glm::vec4 FrameBg{0.2f, 0.205f, 0.21f, 1.0f};
                glm::vec4 FrameBgHovered{0.3f, 0.305f, 0.31f, 1.0f};
                glm::vec4 FrameBgActive{0.15f, 0.1505f, 0.151f, 1.0f};

                glm::vec4 Tab{0.15f, 0.1505f, 0.151f, 1.0f};
                glm::vec4 TabHovered{0.38f, 0.3805f, 0.381f, 1.0f};
                glm::vec4 TabActive{0.28f, 0.2805f, 0.281f, 1.0f};
                glm::vec4 TabUnfocused{0.15f, 0.1505f, 0.151f, 1.0f};
                glm::vec4 TabUnfocusedActive{0.2f, 0.205f, 0.21f, 1.0f};

                glm::vec4 TitleBg{0.15f, 0.1505f, 0.151f, 1.0f};
                glm::vec4 TitleBgActive{0.15f, 0.1505f, 0.151f, 1.0f};
                glm::vec4 TitleBgCollapse{0.15f, 0.1505f, 0.151f, 1.0f};
            };

            LayerConfig m_editorConfig{};
    };

}