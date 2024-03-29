#pragma once


#include "Scene.hpp"
#include "Components.hpp"
#include <entt/entt.hpp>

namespace Creepy {

    class Entity
    {
        public:
            Entity() noexcept;

            template <typename T>
            constexpr std::remove_cvref_t<T>& AddComponent(auto&&... args) noexcept {
                auto& component = m_scene->m_registry.emplace<std::remove_cvref_t<T>>(m_entityHandle, std::forward<decltype(args)>(args)...);
                m_scene->OnComponentAdded<std::remove_cvref_t<T>>(*this, component);
                return component;
            }

            template <typename T>
            constexpr std::remove_cvref_t<T>& AddOrReplaceComponent(auto&&... args) noexcept {
                auto& component = m_scene->m_registry.emplace_or_replace<std::remove_cvref_t<T>>(m_entityHandle, std::forward<decltype(args)>(args)...);
                m_scene->OnComponentAdded<std::remove_cvref_t<T>>(*this, component);
                return component;
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

            constexpr operator uint32_t() const noexcept {
                return static_cast<uint32_t>(m_entityHandle);
            }

            [[nodiscard]] constexpr bool IsExits() const noexcept {
                return m_scene != nullptr;
            }

            [[nodiscard]] UUID& GetUUID() noexcept {
                return this->GetComponent<IDComponent>().ID;
            }

            [[nodiscard]] const std::string& GetName() noexcept{
                return this->GetComponent<TagComponent>().Tag;
            }
            
        private:
            Entity(entt::entity handle, Scene* scene) noexcept;

        private:

            // We only permit Scene create Entity
            friend class Scene;

            // For create to view
            friend class SceneHierarchyPanel;

            // For serializer data
            friend class SceneSerializer;

            // For hovered Entity
            // TODO: Remove
            friend class EditorLayer;

            entt::entity m_entityHandle;
            Scene* m_scene;
    };

    
}