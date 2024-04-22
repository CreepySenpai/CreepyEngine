#include <CreepyEngine/Core/TimeStep.hpp>
#include <CreepyEngine/Scene/Components.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Physics/3D/PhysicWorld3D.hpp>

#include <reactphysics3d/reactphysics3d.h>

namespace Creepy::Physic3D {

    namespace react3D = reactphysics3d;

    reactphysics3d::PhysicsCommon PhysicWorld3D::s_physicCommon;

    Ref<reactphysics3d::PhysicsWorld> PhysicWorld3D::s_physicWorld3D = nullptr;

    void PhysicWorld3D::InitWorld3D(Scene* scene) noexcept {

        // TODO: Test custom deleter
        // s_physicWorld3D = std::make_shared<react3D::PhysicsWorld>(s_physicCommon.createPhysicsWorld(), [](react3D::PhysicsWorld* del){
        //     s_physicCommon.destroyPhysicsWorld(del);
        // });

        // s_physicWorld3D = std::make_shared<react3D::PhysicsWorld>(s_physicCommon.createPhysicsWorld());
    }

    void PhysicWorld3D::UpdateWorld3D(Creepy::TimeStep timeStep, Scene* scene) noexcept {
        
    }

    void PhysicWorld3D::DestroyWorld3D(Scene* scene) noexcept {

        // s_physicCommon.destroyPhysicsWorld(s_physicWorld3D.get());

        // s_physicWorld3D.reset();
        // s_physicWorld3D = nullptr;
    }
     
    
}