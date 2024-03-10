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
            constexpr std::remove_cvref_t<T>& AddComponent(auto&&... args) noexcept {
                return m_scene->m_registry.emplace<std::remove_cvref_t<T>>(m_entityHandle, std::forward<decltype(args)>(args)...);
            }

            template <typename T>
            [[nodiscard]] constexpr std::remove_cvref_t<T>& GetComponent() noexcept {
                return m_scene->m_registry.get<std::remove_cvref_t<T>>(m_entityHandle);
            }

            template <typename T>
            [[nodiscard]] constexpr bool HasComponent() noexcept {
                return m_scene->m_registry.all_of<std::remove_cvref_t<T>>(m_entityHandle);
            }

            template <typename... T>
            [[nodiscard]] constexpr bool HasComponents() noexcept {
                return m_scene->m_registry.all_of<std::remove_cvref_t<T>...>(m_entityHandle);
            }

            template <typename T>
            constexpr void RemoveComponent() noexcept {
                m_scene->m_registry.remove<std::remove_cvref_t<T>>(m_entityHandle);
            }

        private:
            Entity(entt::entity handle, Scene* scene) noexcept;

        private:
            entt::entity m_entityHandle;
            Scene* m_scene;
    };
    
}