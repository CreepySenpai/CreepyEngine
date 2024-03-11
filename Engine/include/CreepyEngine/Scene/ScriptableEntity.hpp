#pragma once

#include <CreepyEngine/Core/TimeStep.hpp>
#include "Entity.hpp"

namespace Creepy {

    class ScriptableEntity
    {
        public:
            friend class Scene;

            template <typename T>
            [[nodiscard]] constexpr decltype(auto) GetComponent() noexcept {
                return m_entity.GetComponent<T>();
            }

            ScriptableEntity() noexcept = default;
            virtual ~ScriptableEntity() noexcept = default;

        protected:
        
            virtual void OnCreate() noexcept {}

            virtual void OnUpdate(TimeStep timeStep) noexcept {}

            virtual void OnDestroy() noexcept {}

        private:
            Entity m_entity;
    };
    
}