#pragma once

#include <CreepyEngine/Core/Layer.hpp>
#include <CreepyEngine/Core/KeyCode.hpp>
#include <CreepyEngine/Events/MouseEvent.hpp>
#include <CreepyEngine/Events/ApplicationEvent.hpp>
#include <CreepyEngine/Events/KeyEvent.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

namespace Creepy
{
    class ImGuiLayer : public Layer
    {
        public:
            ImGuiLayer() noexcept;
            virtual ~ImGuiLayer() = default;

            void OnAttach() noexcept override;
            void OnDetach() noexcept override;
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
    };

}