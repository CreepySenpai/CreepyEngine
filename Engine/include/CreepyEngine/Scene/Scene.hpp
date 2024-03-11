#pragma once

#include <CreepyEngine/Core/TimeStep.hpp>
#include <entt/entt.hpp>

namespace Creepy {

    class Entity;

    class Scene
    {
        public:
            Scene() noexcept;
            ~Scene() noexcept;

            [[nodiscard]] Entity CreateEntity(const std::string& tag = std::string{"Entity"}) noexcept;

            void OnUpdate(const TimeStep& timeStep) noexcept;

            void OnViewPortResize(uint32_t width, uint32_t height) noexcept;
        private:
            entt::registry m_registry;
            uint32_t m_viewPortWidth{0};
            uint32_t m_viewPortHeight{0};

            friend class Entity;
    };
    
    
}