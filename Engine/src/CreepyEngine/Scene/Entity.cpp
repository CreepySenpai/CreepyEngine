#include <CreepyEngine/Scene/Entity.hpp>

namespace Creepy {

    Entity::Entity() noexcept : m_entityHandle{entt::null}, m_scene{nullptr} {}

    Entity::Entity(entt::entity handle, Scene* scene) noexcept : m_entityHandle{handle}, m_scene{scene} {
        
    }

}