#pragma once

#include <CreepyEngine/Core/TimeStep.hpp>
#include <entt/entt.hpp>
#include "Components.hpp"

namespace Creepy {

    class Entity;

    class Scene
    {
        public:
            Scene() noexcept;
            ~Scene() noexcept;

            [[nodiscard]] Entity CreateEntity(const std::string& tag = std::string{"Entity"}) noexcept;

            void OnUpdate(const TimeStep& timeStep) noexcept;
        private:
            entt::registry m_registry;

            friend class Entity;
    };
    
    
}