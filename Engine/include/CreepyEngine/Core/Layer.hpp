#pragma once

#include <CreepyEngine/Events/Event.hpp>
#include "TimeStep.hpp"

namespace Creepy {
    class Layer
    {
        public:
            Layer(const std::string& name = "Layer") noexcept;
            constexpr virtual ~Layer() noexcept = default;
            
            constexpr virtual void OnAttach() noexcept {}
            constexpr virtual void OnDetach() noexcept {}
            constexpr virtual void OnUpdate(TimeStep timeStep) noexcept {}
            constexpr virtual void OnImGuiRender() noexcept {}
            constexpr virtual void OnEvent(Event& event) noexcept {}

            constexpr inline const std::string& GetDebugName() const noexcept {
                return m_debugName;
            }

        protected:
            std::string m_debugName;
    };
}