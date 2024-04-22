#pragma once



namespace Creepy {

    class TimeStep;

    class Scene;

    class PhysicWorld{

        static void InitWorld(Scene *scene) noexcept;

        static void UpdateWorld(TimeStep timeStep, Scene *scene) noexcept;

        static void DestroyWorld(Scene *scene) noexcept;

    };

}