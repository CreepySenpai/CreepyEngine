#pragma once


#include "Scene.hpp"
#include "Components.hpp"
#include <entt/entt.hpp>

namespace Creepy {

    class Entity
    {
        public:
            Entity() noexcept;

            // We only permit Scene create Entity
            friend class Scene;

            template <typename T>
            T& AddComponent(auto&&... args) noexcept {
                return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<decltype(args)>(args)...);
            }

            template <typename T>
            [[nodiscard]] T& GetComponent() noexcept {
                return m_scene->m_registry.get<T>(m_entityHandle);
            }

            template <typename T>
            [[nodiscard]] bool HasComponent() noexcept {
                return m_scene->m_registry.all_of<T>(m_entityHandle);
            }

            template <typename... T>
            [[nodiscard]] bool HasComponents() noexcept {
                return m_scene->m_registry.all_of<T...>(m_entityHandle);
            }

            template <typename T>
            void RemoveComponent() noexcept {
                m_scene->m_registry.remove<T>(m_entityHandle);
            }

        private:
            Entity(entt::entity handle, Scene* scene) noexcept;

        private:
            entt::entity m_entityHandle;
            Scene* m_scene;
    };
    
}