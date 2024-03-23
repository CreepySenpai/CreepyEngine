#pragma once

#include <CreepyEngine/Core/TimeStep.hpp>
#include <CreepyEngine/Core/UUID.hpp>
#include <CreepyEngine/Renderer/EditorCamera.hpp>
#include <entt/entt.hpp>

class b2World;

namespace Creepy {

    class Entity;

    class Scene
    {
        public:
            Scene() noexcept;
            ~Scene() noexcept;

            Entity CreateEntity(const std::string& tag = std::string{"Entity"}) noexcept;
            Entity CreateEntity(UUID uuid, const std::string& tag = std::string{"Entity"}) noexcept;

            void DestroyEntity(Entity& entity) noexcept;

            void OnUpdateEditor(TimeStep timeStep, EditorCamera& camera) noexcept;
            void OnUpdateRunTime(TimeStep timeStep) noexcept;

            void OnRuntimePlay() noexcept;
            void OnRuntimeStop() noexcept;

            void OnViewPortResize(uint32_t width, uint32_t height) noexcept;

            // TODO: Remove it
            Entity GetPrimaryCameraEntity() noexcept;

        private:
            template <typename T>
            void OnComponentAdded(Entity& entity, T& component) {

            }
        
        private:
            entt::registry m_registry;
            uint32_t m_viewPortWidth{0};
            uint32_t m_viewPortHeight{0};

            Scope<b2World> m_physicWorld;

            friend class Entity;
            friend class SceneHierarchyPanel;
            friend class SceneSerializer;
    };
    
    
}