#pragma once

#include <CreepyEngine/Core/Core.hpp>

class b2World;

namespace Creepy{

    class TimeStep;

    class Scene;

}

namespace Creepy::Physic2D {

    class PhysicWorld2D
    {
        public:
            static void InitWorld2D(Scene* scene) noexcept;

            static void UpdateWorld2D(Creepy::TimeStep timeStep, Scene* scene) noexcept;

            static void DestroyWorld2D(Scene* scene) noexcept;

        private:

            static Ref<b2World> s_physicWorld2D;
    };
}