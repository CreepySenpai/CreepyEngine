#pragma once
#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Core/UUID.hpp>
#include <entt/entt.hpp>

class b2World;

namespace Creepy {
    class Entity;
    class TimeStep;
    class EditorCamera;

    namespace Physic2D{
        class PhysicWorld2D;
    }

    namespace Physic3D{
        class PhysicWorld3D;
    }

    class Scene
    {
        public:
            Scene() noexcept;
            ~Scene() noexcept;

            Entity CreateEntity(const std::string& tag = std::string{"Entity"}) noexcept;
            Entity CreateEntity(UUID uuid, const std::string& tag = std::string{"Entity"}) noexcept;

            [[nodiscard]] Entity GetEntity(UUID uuid) noexcept;

            [[nodiscard]] Entity GetEntity(std::string_view entityName) noexcept;

            void DestroyEntity(Entity& entity) noexcept;

            void OnUpdateEditor(TimeStep timeStep, EditorCamera& camera) noexcept;
            void OnUpdateRunTime(TimeStep timeStep) noexcept;
            void OnUpdateSimulation(TimeStep timeStep, EditorCamera& camera) noexcept;

            void OnRuntimePlay() noexcept;
            void OnRuntimeStop() noexcept;
            void OnSimulationPlay() noexcept;
            void OnSimulationStop() noexcept;

            void OnViewPortResize(uint32_t width, uint32_t height) noexcept;

            // TODO: Use it to change gizmos camera
            Entity GetPrimaryCameraEntity() noexcept;

            void DuplicateEntity(Entity& entity) noexcept;

            bool IsScenePlay() const noexcept;
            bool IsScenePause() const noexcept;

            constexpr void SetPause(bool pause) noexcept {
                m_isScenePause = pause;
            }

            void Step(int frame) noexcept;

            template <typename... Comps>
            [[nodiscard]] decltype(auto) GetAllEntitiesType() noexcept{
                return m_registry.view<Comps...>();
            }

            [[nodiscard]] static Ref<Scene> Copy(const Ref<Scene>& scene) noexcept;

        private:
            template <typename T>
            void OnComponentAdded(Entity& entity, T& component) {

            }

            void onPhysic2DStart() noexcept;

            void onPhysic2DStop() noexcept;

            void renderScene(EditorCamera& editorCamera) noexcept;

        private:
            void onScriptUpdate(TimeStep timeStep) noexcept;
            void onPhysicUpdate(TimeStep timeStep) noexcept;
            void onRenderUpdate() noexcept;
        
        private:
            entt::registry m_registry;
            uint32_t m_viewPortWidth{0};
            uint32_t m_viewPortHeight{0};

            // Need to copy so we can't use std::unique_ptr
            b2World* m_physicWorld{nullptr};

            std::unordered_map<UUID, entt::entity> m_entityMap;

            bool m_isScenePlay{false};
            bool m_isScenePause{false};
            int m_stepFrame{0};

            friend class Entity;
            friend class SceneHierarchyPanel;
            friend class SceneSerializer;
            
            friend class Creepy::Physic2D::PhysicWorld2D;
            friend class Creepy::Physic3D::PhysicWorld3D;
    };
    
    
}