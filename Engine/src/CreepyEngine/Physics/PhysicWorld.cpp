#include <CreepyEngine/Physics/PhysicWorld.hpp>
#include <CreepyEngine/Core/TimeStep.hpp>
#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Physics/2D/PhysicWorld2D.hpp>
#include <CreepyEngine/Physics/3D/PhysicWorld3D.hpp>
namespace Creepy {

    void InitWorld(Scene *scene) noexcept {
        Physic2D::PhysicWorld2D::InitWorld2D(scene);
        Physic3D::PhysicWorld3D::InitWorld3D(scene);
    }

    void UpdateWorld(TimeStep timeStep, Scene *scene) noexcept {
        Physic2D::PhysicWorld2D::UpdateWorld2D(timeStep, scene);
        Physic3D::PhysicWorld3D::UpdateWorld3D(timeStep, scene);
    }

    void DestroyWorld(Scene *scene) noexcept {
        Physic2D::PhysicWorld2D::DestroyWorld2D(scene);
        Physic3D::PhysicWorld3D::DestroyWorld3D(scene);
    }

}