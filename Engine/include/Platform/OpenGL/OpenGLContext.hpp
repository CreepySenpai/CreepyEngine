#pragma once

#include <CreepyEngine/Renderer/GraphicContext.hpp>

class GLFWwindow;

namespace Creepy {

    class TimeStep;

    class OpenGLContext : public GraphicContext
    {
        public:
            OpenGLContext(GLFWwindow* windowHandle) noexcept;

            void Init() noexcept override;
            void SwapBuffers() noexcept override;
            void Update(TimeStep timeStep) noexcept override;
            void ShutDown() noexcept override;
        private:
            GLFWwindow* m_windowHandle;
    };
    
}