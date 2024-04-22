#pragma once

#include <CreepyEngine/Core/Core.hpp>

// Forward Declare
namespace Creepy{

    class TimeStep;

    class Scene;
}

namespace reactphysics3d{
    class PhysicsCommon;
    class PhysicsWorld;
}


namespace Creepy::Physic3D{

    class PhysicWorld3D
    {
        public:
            static void InitWorld3D(Scene* scene) noexcept;

            static void UpdateWorld3D(Creepy::TimeStep timeStep, Scene* scene) noexcept;

            static void DestroyWorld3D(Scene* scene) noexcept;

        private:
            static reactphysics3d::PhysicsCommon s_physicCommon;

            static Ref<reactphysics3d::PhysicsWorld> s_physicWorld3D;
    };
}